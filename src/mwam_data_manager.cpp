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

	//_flashData = Flashee::Devices::createAddressErase();

	/*if (_flashData == NULL) {
		ERROR("Flash NULL!");
		return false;
	}

	if (!loadMetadata()) {
		return false;
	}*/

	//Flashee::Devices::userFlash().eraseAll();

	return true;
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
						//uint8_t flagIndex = Serial.read();
						//uint8_t bitIndex = Serial.read();
						//bool value = Serial.read();
						//setFlag(flagIndex, bitIndex, value);
						break;
					}
					case kSerialCmdSetValue: {
						//uint8_t index = Serial.read();
						//uint16_t value = (Serial.read() << 8) | Serial.read();
						//setValue(index, value);
						break;
					}
					case kSerialCmdResetMetadata: {
						//resetMetadata();
						stateController()->changeState(STATE_INIT);
						break;
					}
					case kSerialCmdEraseFlash: {
						//eraseFlash();
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

/* API Calls */

/*bool DataManager::setFlag(uint8_t aFlagIndex, uint8_t aBitIndex, bool aValue) {
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
}*/

/*bool DataManager::resetMetadata() {
	return initializeMetadata(&_metadata);
}

bool DataManager::eraseFlash() {
	return Flashee::Devices::userFlash().eraseAll();
}*/

/* Accessors */

StateController* DataManager::stateController() {
	return _stateController;
}

Metadata* DataManager::metadata() {
	return &_metadata;
}

/* Private Methods */

}