#include "mwam_data_manager.h"
#include "mwam_constants.h"
#include "mwam_state_controller.h"

extern char* itoa(int a, char* buffer, unsigned char radix);

namespace mwam
{

/* Public Methods */

DataManager::DataManager() {}

bool DataManager::initialize(StateController *aStateController) {
	_stateController = aStateController;
	_metadata = {0};
	this->runState = true;
	this->hasCredentials = false;

	_flashData = Flashee::Devices::createAddressErase();

	if (_flashData == NULL) {
		ERROR("Flash NULL!");
		return false;
	}

	if (!loadMetadata()) {
		return false;
	}

	//Flashee::Devices::userFlash().eraseAll();

	return true;
}

void DataManager::logScoreData() {
	/*ScoreData scoreData;
	//DEBUG("Meta Size: %d, Score Size: %d", kMetadataSize, kScoreDataSize);
	//DEBUG("Real Meta Size: %d, Score Size: %d", sizeof(_metadata), sizeof(scoreData));
	DEBUG("Total Scores: %d", _metadata.totalScores);
	uint8_t highScoreCount = (_metadata.totalScores < kHighScoreCount) ? _metadata.totalScores : kHighScoreCount;
	for (uint8_t i = 0; i < highScoreCount; ++i) {
		readScoreData(&scoreData, _metadata.highScores[i]);
		DEBUG("%d:%d. [%s] - %d : %s", i + 1, scoreData.standing,
		      scoreData.initials, scoreData.finalScore,
		      scoreData.hashId);
	}
	DEBUG("All Scores:");
	for (uint16_t i = kHighScoreCount; i < (kHighScoreCount + _metadata.totalScores); ++i) {
		readScoreData(&scoreData, i);
		DEBUG("%d:%d. [%s] - %d : %s", i - kHighScoreCount + 1, scoreData.standing,
		      scoreData.initials, scoreData.finalScore,
		      scoreData.hashId);
	}*/
}

void DataManager::handleSerialData() {
	if (!SPARK_FLASH_UPDATE && (_serialElapsed >= kIntervalSerialMillis)) {
		if (Serial.available()) {
			uint8_t cmd = Serial.read();

			if (cmd == kSerialCmdMode) {
				Serial.println("Command Mode");
				_serialTimeout = 0;
				while (!Serial.available()) {
					if (_serialTimeout >= kTimeoutSerialMillis) {
						//Serial.println("Timeout");
						return;
					}
				}
				cmd = Serial.read();

				switch (cmd)
				{
					case kSerialCmdListeningMode: {
						this->hasCredentials = WiFi.hasCredentials();
						WiFi.listen();
						if (this->hasCredentials != WiFi.hasCredentials()) {
							this->hasCredentials = WiFi.hasCredentials();
							_metadata.flags.offline = this->hasCredentials ? 0 : 1;
						}
						break;
					}
					case kSerialCmdClearWiFi: {
						//Serial.println("Clearing WiFi creds.");
						WiFi.disconnect();
						WiFi.clearCredentials();
						WiFi.off();
						break;
					}
					case kSerialCmdSetFlag: {
						uint8_t flagIndex = Serial.read();
						uint8_t bitIndex = Serial.read();
						bool value = Serial.read();
						setFlag(flagIndex, bitIndex, value);
						break;
					}
					case kSerialCmdSetValue: {
						uint8_t index = Serial.read();
						uint16_t value = (Serial.read() << 8) | Serial.read();
						setValue(index, value);
						break;
					}
					case kSerialCmdResetMetadata: {
						resetMetadata();
						stateController()->changeState(STATE_INIT);
						break;
					}
					case kSerialCmdEraseFlash: {
						eraseFlash();
						stateController()->changeState(STATE_INIT);
						break;
					}
					case kSerialCmdRebootUnit: {
						stateController()->changeState(STATE_INIT);
						break;
					}
					case kSerialCmdDfuMode: {
						FLASH_OTA_Update_SysFlag = 0x0000;
						Save_SystemFlags();
						BKP_WriteBackupRegister(BKP_DR10, 0x0000);
						USB_Cable_Config(DISABLE);
						NVIC_SystemReset();
						break;
					}
					case kSerialCmdGetFlag: {
						uint8_t index = Serial.read();
						char* flags = (char*)&_metadata.flags;
						Serial.write(flags[index]);
						break;
					}
					case kSerialCmdGetValue: {
						uint8_t index = Serial.read();
						uint8_t* values = (uint8_t*)&_metadata.values;
						Serial.write(values[index * 2]);
						Serial.write(values[(index * 2) + 1]);
						break;
					}
				}
			}
		}
	}
}

bool DataManager::saveScoreData(ScoreData *aScoreData) {
	aScoreData->standing = highScoreStanding(aScoreData);
	return saveScoreData(aScoreData, aScoreData->standing);
}

bool DataManager::saveScoreData(ScoreData *aScoreData, uint8_t aStanding) {
	// Standing is place on high score list, not index.
	if (_metadata.flags.saveScores &&  writeScoreData(aScoreData, _metadata.scoreIndex)) {
		if (aStanding > 0) {
			if (_metadata.flags.tweetScores) {
				//DEBUG("Will tweet score.");
				aScoreData->tweeted = tweetHighScore(aScoreData, aScoreData->standing);
			}
			// Insert into the high score table, give index not standing.
			if (_metadata.flags.saveHighScores) {
				insertHighScore(&_metadata, aScoreData, aStanding - 1);
			}
		}
		if (_metadata.flags.uploadScores) {
			aScoreData->tweeted = uploadScore(aScoreData);
		}
		_metadata.scoreIndex++;
		// Max 2000 scores (not counting top ten).
		if (_metadata.scoreIndex >= kMaxStoredScores) {
			_metadata.scoreIndex = kHighScoreCount;
		}
		if (_metadata.totalScores < kMaxStoredScores) {
			_metadata.totalScores++;
		}
		return writeMetadata(&_metadata);
	}
	return false;
}

uint8_t DataManager::highScoreStanding(ScoreData *aScoreData) {
	aScoreData->finalScore = calculateScore(aScoreData);
	//DEBUG("Final Score: %d", aScoreData->finalScore);

	// Check each existing high score to see if this score beats it.
	ScoreData highScore;
	uint8_t highScoreCount = (_metadata.totalScores < kHighScoreCount) ? _metadata.totalScores : kHighScoreCount;
	for (uint8_t i = 0; i < highScoreCount; ++i) {
		if (readScoreData(&highScore, _metadata.highScores[i])) {
			if (aScoreData->finalScore > highScore.finalScore) {
				return i + 1;
			}
		}
	}
	if (_metadata.totalScores < kHighScoreCount) {
		return _metadata.totalScores + 1;
	}
	return 0;
}

bool DataManager::getHighScore(ScoreData *aScoreData, uint8_t aStanding) {
	return readScoreData(aScoreData, aStanding - 1);
}

/* API Calls */

bool DataManager::setFlag(uint8_t aFlagIndex, uint8_t aBitIndex, bool aValue) {
	if (aValue) {
		// Access the flag byte at the appropriate offset and set it.
		(*((uint8_t *) &_metadata + kMetadataFlagsOffset + aFlagIndex)) |= 1 << aBitIndex;
	} else {
		// Access the flag byte at the appropriate offset and clear it.
		(*((uint8_t *) &_metadata + kMetadataFlagsOffset + aFlagIndex)) &= ~(1 << aBitIndex);
	}
	uint8_t flag = (*((uint8_t *)&_metadata + kMetadataFlagsOffset + aFlagIndex));
	bool result = _flashData->writeEraseByte(flag, kMetadataFlagsOffset + aFlagIndex);
	return result;
}

bool DataManager::setValue(uint8_t aIndex, uint16_t aValue) {
	uint16_t* values = (uint16_t*)&_metadata.values;
	values[aIndex] = aValue;
	bool result = _flashData->write(&values[aIndex], kMetadataValuesOffset + (aIndex * 2), 2);
	return result;
}

bool DataManager::resetMetadata() {
	return initializeMetadata(&_metadata);
}

bool DataManager::eraseFlash() {
	return Flashee::Devices::userFlash().eraseAll();
}

/* Accessors */

StateController* DataManager::stateController() {
	return _stateController;
}

Metadata* DataManager::metadata() {
	return &_metadata;
}

/* Private Methods */

bool DataManager::uploadScore(ScoreData *aScoreData) {
	// TODO
	if (Spark.connected()) {
		//char buffer[] =
		//return Spark.publish("upload-high-score", buffer, 60, PRIVATE);
	}
	return false;
}

bool DataManager::tweetHighScore(ScoreData *aScoreData, uint8_t aStanding) {
	if (Spark.connected()) {
		//DEBUG("Tweeting score!");
		char buffer[] = "XXX, NNNN rank! K\0\0\0\0\0";
		strncpy(buffer, aScoreData->initials, 3);
		itoa(aScoreData->finalScore, buffer + 16, 10);
		if (aStanding == 10) {
			strncpy(buffer + 5, "10th", 4);
		} else {
			for (uint8_t i = 9; i < sizeof(buffer); ++i) {
				buffer[i - 1] = buffer[i];
			}
			buffer[5] = aStanding + '0';
			if (aStanding == 1) {
				strncpy(buffer + 6, "st", 2);
			} else if (aStanding == 2) {
				strncpy(buffer + 6, "nd", 2);
			} else if (aStanding == 3) {
				strncpy(buffer + 6, "rd", 2);
			} else {
				strncpy(buffer + 6, "th", 2);
			}
		}
		return Spark.publish("post-high-score", buffer, 60, PRIVATE);
	}
	return false;
}

uint16_t DataManager::calculateScore(ScoreData *aScoreData) {
	// TODO: Calculate a score.
	uint16_t finalScore = aScoreData->kills;
	finalScore -= aScoreData->bDisruptorKills;
	finalScore -= aScoreData->bMissileKills;

	return finalScore;
}

bool DataManager::insertHighScore(Metadata *aMetadata, ScoreData *aScoreData, uint8_t aTableIndex) {
	uint8_t writeIndex = aMetadata->highScores[kHighScoreCount - 1];
	if (aMetadata->totalScores < kHighScoreCount) {
		writeIndex = aMetadata->totalScores;
	}

	if (writeScoreData(aScoreData, writeIndex)) {
		for (uint8_t i = (kHighScoreCount - 1); i >= 0; --i) {
			if (aTableIndex == i) {
				aMetadata->highScores[i] = writeIndex;
				break;
			} else {
				aMetadata->highScores[i] = aMetadata->highScores[i - 1];
			}
		}
		return true;
	}

	return false;
}

bool DataManager::loadMetadata() {
	// Check for SOH
	if (_flashData->readByte(kMetadataSohOffset) != kAsciiHeaderByte) {
		if (!initializeMetadata(&_metadata)) {
			return false;
		}
	} else {
		// Data exists. Read it!
		if (!readMetadata(&_metadata)) {
			return false;
		}
	}
	return true;
}

bool DataManager::initializeMetadata(Metadata *aMetadata) {
	// Set the default values for a fresh Choosatron.
	*aMetadata = {0};

	aMetadata->soh = kAsciiHeaderByte;
	aMetadata->scoreIndex = kHighScoreCount;
	aMetadata->flags.saveScores = 1;
	aMetadata->flags.saveHighScores = 1;
	aMetadata->flags.uploadScores = 1;
	aMetadata->flags.tweetScores = 1;

	// TODO: Do I want to preset the highs core table?
	/*if (!initializeHighScores(aMetadata)) {
		return false;
	}*/

	if (!writeMetadata(aMetadata)) {
		return false;
	}
	return true;
}

bool DataManager::initializeHighScores(Metadata *aMetadata) {
	aMetadata->totalScores = 0;
	aMetadata->scoreIndex = kHighScoreCount;
	uint16_t finalScore = 50;
	ScoreData scoreData;
	for (uint8_t i = 0; i < kHighScoreCount; ++i) {
		scoreData = {0};
		scoreData.tweeted = true;
		scoreData.uploaded = true;
		scoreData.standing = kHighScoreCount - i;
		//scoreData.initials = {'J', 'D', 'B', '\0'};
		strncpy(scoreData.initials, "JDB", 3);
		scoreData.finalScore = finalScore;
		finalScore -= 5;
		if (writeScoreData(&scoreData, aMetadata->totalScores)) {
			return false;
		}
		aMetadata->highScores[i] = aMetadata->totalScores;
		aMetadata->totalScores++;
	}
	return true;
}

bool DataManager::readMetadata(Metadata *aMetadata) {
	bool result = false;
	if (_flashData->readByte(kMetadataSohOffset) == kAsciiHeaderByte) {
		result = _flashData->read(aMetadata, kMetadataSohOffset, kMetadataSize);
	}
	return result;
}

bool DataManager::writeMetadata(Metadata *aMetadata) {
	bool result = _flashData->write(aMetadata, kMetadataSohOffset, kMetadataSize);
	return result;
}

bool DataManager::readScoreData(ScoreData *aScoreData, uint8_t aIndex) {
	uint32_t offset = kMetadataSize + (aIndex * kScoreDataSize);
	return _flashData->read(aScoreData, offset, kMetadataSize);
}

bool DataManager::writeScoreData(ScoreData *aScoreData, uint8_t aIndex) {
	uint32_t offset = kMetadataSize + (aIndex * kScoreDataSize);
	return _flashData->write(aScoreData, offset, kScoreDataSize);
}

}