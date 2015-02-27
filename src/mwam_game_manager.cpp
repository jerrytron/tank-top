#include "mwam_game_manager.h"
#include "mwam_manager.h"

extern char* itoa(int a, char* buffer, unsigned char radix);

namespace mwam
{

const Color kColorGreen = Color(0, 255, 0);
const Color kColorYellow = Color(255, 255, 0);
const Color kColorOrange = Color(255, 140, 0);
const Color kColorRed = Color(255, 0, 0);
const Color kColorBlack = Color(0, 0, 0);

/* Public Methods */

GameManager::GameManager() {
}

void GameManager::initialize(StateController *aStateController) {
	_stateController = aStateController;
	_dataManager = Manager::getInstance().dataManager;
	_hardwareManager = Manager::getInstance().hardwareManager;
}

void GameManager::reset() {
	_playerScore = {0};
	_initialDialed = 0;
	_initialIndex = 0;
	_initialChar = '\0';
	_charOffset = 0;
	_introStep = 0;
	_tutorialStep = 0;
	_gameOverStep = 0;
	_highScoreStep = 0;
	_playerHealth = kDefaultPlayerHealth;
	_currentWave = 0;
	_currentSet = 0;
	_bogeysLeft = 0;
	_bombs = 1;
	_bombing = false;
	_takingHit = false;
	_disruptorHit = false;
	_disruptorElapsed = 0;

	_hardwareManager->button()->resetButton();
	_hardwareManager->rotary()->resetRotary();
	_hardwareManager->bubbleDisplay()->resetDisplays();
	_hardwareManager->ledSet()->resetLeds();

	resetBogeys();
	scoreDataStart();
}

/*void GameManager::updateState() {
	if (_stateController->getState() == STATE_INTRO) {
		updateIntro();
	}

	if (_stateController->getState() == STATE_PLAY) {
		updatePlay();
	}
}*/

void GameManager::scoreDataStart() {
	if (Spark.connected()) {
		_playerScore.hasRealTime = true;
	}
	_playerScore.startTime = Time.now();
	Utils::GetBase62(kScoreHashIdLength, _playerScore.hashId);
}

void GameManager::scoreDataEnd(bool aGameWon) {
	_playerScore.endTime = Time.now();
	_playerScore.wonGame = aGameWon;
	_playerScore.lifeRemaining = _playerHealth;
	_playerScore.waveReached = currentWave();
}

void GameManager::updateIntro() {
	uint8_t scrollIndex = _hardwareManager->bubbleDisplay()->scrollIndex();
	bool scrolling = _hardwareManager->bubbleDisplay()->scrolling();
	if (_hardwareManager->button()->wasClicked()) {
		_hardwareManager->ledSet()->resetLeds();
		_introStep = 4;
	}
	if (_introStep == 0) {
		// Stop the scrolling text.
		_hardwareManager->bubbleDisplay()->resetDisplays();
		_introStep++;
	} else if (_introStep == 1) {
		if (!scrolling && (scrollIndex == 0)) {
			_hardwareManager->bubbleDisplay()->setData(kMsgIntroOne, kMsgIntroOneLen);
			_hardwareManager->bubbleDisplay()->setScrollStyle(SCROLL_LEFT, 8, kBubbleDisplayJumpFreq, kMsgIntroOneLen);
			_hardwareManager->bubbleDisplay()->startScrolling();

			Animation a = sweepGrnAnim();
			a.repeats = 1;
			a.repeatDelay = 500;

			_hardwareManager->ledSet()->animateAll(a, false, kRadarAnimStaggerMillis);
		} else if (!scrolling) {
			// Stop the scrolling text.
			_hardwareManager->bubbleDisplay()->resetDisplays();
			_introStep++;
		}
	} else if (_introStep == 2) {
		if (!scrolling && (scrollIndex == 0)) {
			_hardwareManager->bubbleDisplay()->setData(kMsgIntroTwo, kMsgIntroTwoLen);
			_hardwareManager->bubbleDisplay()->setScrollStyle(SCROLL_LEFT, 8, kBubbleDisplayJumpFreq, kMsgIntroTwoLen);
			_hardwareManager->bubbleDisplay()->startScrolling();

			//_hardwareManager->ledSet()->animateAll(_sweepGrnAnim, false, kRadarAnimStaggerMillis);
		} else if (!scrolling) {
			// Stop the scrolling text.
			_hardwareManager->bubbleDisplay()->resetDisplays();
			_introStep++;
		}
	} else if (_introStep == 3) {
		if (!scrolling && (scrollIndex == 0)) {
			_hardwareManager->bubbleDisplay()->setData(kMsgIntroThree, kMsgIntroThreeLen);
			_hardwareManager->bubbleDisplay()->setScrollStyle(SCROLL_LEFT, 8, kBubbleDisplayJumpFreq, kMsgIntroThreeLen);
			_hardwareManager->bubbleDisplay()->startScrolling();

			Animation a = sweepRedAnim();
			a.repeatDelay = 500;

			_hardwareManager->ledSet()->animateAll(a, false, kRadarAnimStaggerMillis);
		} else if (!scrolling) {
			// Stop the scrolling text.
			_hardwareManager->bubbleDisplay()->resetDisplays();
			_introStep++;
		}
	} else if (_introStep == 4) {
		// Setup the first wave to play.
		DEBUG("Setup Wave");
		_stateController->changeState(STATE_SETUP_WAVE);
	}
}

void GameManager::updateTutorial() {
	//uint8_t scrollIndex = _hardwareManager->bubbleDisplay()->scrollIndex();
	bool scrolling = _hardwareManager->bubbleDisplay()->scrolling();
	if (_tutorialStep == 0) {
		// Stop the scrolling text.
		_hardwareManager->bubbleDisplay()->resetDisplays();
		_tutorialStep++;
	} else if (_tutorialStep == 1) {
		if (!scrolling) {
			_bogeys[6] = Bogey(B_LIGHT);
			_hardwareManager->bubbleDisplay()->setData(kMsgTutorialDial, kMsgTutorialDialLen);
			_hardwareManager->bubbleDisplay()->setScrollStyle(SCROLL_LEFT_WRAP, 1, kBubbleDisplayScrollFreq, kMsgTutorialDialLen);
			_hardwareManager->bubbleDisplay()->startScrolling();
		}

		_bogeys[6].travelElapsed = 0;
		if (_hardwareManager->rotary()->newDialEvent()) {
			uint8_t dialValue = _hardwareManager->rotary()->lastDialed();
			if (dialValue == kRadarDialValue) {
				_hardwareManager->bubbleDisplay()->resetDisplays();
				playRadarAnim();
				_tutorialStep++;
			}
		}
	} else if (_tutorialStep == 2) {
		if ((_hardwareManager->ledSet()->getLedState(0) == LED_IDLE) && !scrolling) {
			_hardwareManager->bubbleDisplay()->setData(kMsgTutorialCloser, kMsgTutorialCloserLen);
			_hardwareManager->bubbleDisplay()->setScrollStyle(SCROLL_LEFT_WRAP, 1, kBubbleDisplayScrollFreq, kMsgTutorialCloserLen);
			_hardwareManager->bubbleDisplay()->startScrolling();
		}

		_bogeys[6].travelElapsed = 7500;
		if (_hardwareManager->rotary()->newDialEvent()) {
			uint8_t dialValue = _hardwareManager->rotary()->lastDialed();
			if (dialValue == kRadarDialValue) {
				//_hardwareManager->bubbleDisplay()->resetDisplays();
				_hardwareManager->bubbleDisplay()->setScrollStyle(SCROLL_LEFT);
				playRadarAnim();
				_tutorialStep++;
			}
		}
	} else if (_tutorialStep == 3) {
		if (_hardwareManager->ledSet()->allLedsIdle() && !scrolling) {
			_hardwareManager->bubbleDisplay()->resetDisplays();
			_hardwareManager->bubbleDisplay()->setData(kMsgTutorialFire, kMsgTutorialFireLen);
			_hardwareManager->bubbleDisplay()->setScrollStyle(SCROLL_LEFT_WRAP, 1, kBubbleDisplayScrollFreq, kMsgTutorialFireLen);
			_hardwareManager->bubbleDisplay()->startScrolling();
		}

		_bogeys[6].travelElapsed = 7500;
		if (_hardwareManager->rotary()->newDialEvent()) {
			uint8_t dialValue = _hardwareManager->rotary()->lastDialed();
			if (dialValue == kRadarDialValue) {
				playRadarAnim();
			} else {
				if (fireWeapon(dialValue)) {
					_hardwareManager->bubbleDisplay()->resetDisplays();
					_tutorialStep++;
				}
			}
		}
	} else if (_tutorialStep == 4) {
		if (_hardwareManager->ledSet()->allLedsIdle() && !scrolling) {
			_bogeys[1] = Bogey(B_LIGHT);
			_bogeys[4] = Bogey(B_LIGHT);
			_bogeys[6] = Bogey(B_LIGHT);
			_hardwareManager->bubbleDisplay()->setData(kMsgTutorialBomb, kMsgTutorialBombLen);
			_hardwareManager->bubbleDisplay()->setScrollStyle(SCROLL_LEFT_WRAP, 1, kBubbleDisplayScrollFreq, kMsgTutorialBombLen);
			_hardwareManager->bubbleDisplay()->startScrolling();
		}
		_bogeys[1].travelElapsed = 1500;
		_bogeys[4].travelElapsed = 5000;
		_bogeys[6].travelElapsed = 7500;
		if (_hardwareManager->rotary()->newDialEvent()) {
			uint8_t dialValue = _hardwareManager->rotary()->lastDialed();
			if (dialValue == kRadarDialValue) {
				playRadarAnim();
			}
		}
		if (_hardwareManager->button()->wasClicked()) {
			detonateBomb();
			_hardwareManager->bubbleDisplay()->resetDisplays();
			_tutorialStep++;
		}
	} else if (_tutorialStep == 5) {
		if (_hardwareManager->ledSet()->allLedsIdle()) {
			_hardwareManager->bubbleDisplay()->setData(kMsgTutorialHealth, kMsgTutorialHealthLen);
			_hardwareManager->bubbleDisplay()->setScrollStyle(SCROLL_LEFT, 1, kBubbleDisplayScrollFreq, kMsgTutorialHealthLen);
			_hardwareManager->bubbleDisplay()->startScrolling();
			_tutorialStep++;
		}
	} else if (_tutorialStep == 6) {
		if (!scrolling) {
			reset();
			_stateController->changeState(STATE_INTRO);
		}
	}
}

void GameManager::updatePlay() {
	if (_bombing || _takingHit) {
		if (_hardwareManager->ledSet()->allLedsIdle()) {
			// Should never happen at the same time.
			_bombing = false;
			_takingHit = false;
		}
		return;
	}
	if (_bogeysLeft == 0) {
		//if (_hardwareManager->ledSet()->allLedsIdle()) {
			DEBUG("New Set, Score: %d", _playerScore.kills);
			setupNextSet();
		//}
		return;
	}
	if (_disruptorHit) {
		if (_disruptorElapsed >= 1500) {
			_disruptorHit = false;
		}
	}
	for (uint8_t i = kBogeyIndexMin; i <= kBogeyIndexMax; ++i) {
		if (_bogeys[i].deployDelay > 0) {
			if (_bogeys[i].delayElapsed >= _bogeys[i].deployDelay) {
				//DEBUG("Bogey Delay Complete: %d", i);
				// Delay over, setup for travel.
				_bogeys[i].deployDelay = 0;
				_bogeys[i].travelElapsed = 0;
			}
		} else {
			if (_bogeys[i].type >= B_LIGHT) {
				if (timeToArrive(_bogeys[i].type) > 0) {
					if (_bogeys[i].travelElapsed >= timeToArrive(_bogeys[i].type)) {
						//DEBUG("Bogey Arrived: %d", i);
						// Bogey has arrived.
						if (_bogeys[i].type == B_DISRUPTOR) {
							//DEBUG("Disruptor hit!");
							_disruptorHit = true;
							_playerScore.disrupted++;
							_disruptorElapsed = 0;
							playRadarDisruptor();
						} else {
							_playerHealth--;
							if (_playerHealth > 0) {
								//DEBUG("Player hit!");
								playPlayerHitAnim();
								// This assumes always having 3 health total.
								if (_playerHealth == 2) {
									_playerScore.firstHitBy = _bogeys[i].type;
									_playerScore.firstHitWave = currentWave();
								} else if (_playerHealth == 1) {
									_playerScore.secondHitBy = _bogeys[i].type;
									_playerScore.secondHitWave = currentWave();
								}
								_takingHit = true;
							} else {
								playPlayerDeathAnim();
								_playerScore.killedBy = _bogeys[i].type;
								scoreDataEnd(false);
								_stateController->changeState(STATE_GAME_OVER);
								return;
							}
						}
						_bogeys[i] = Bogey();
						_bogeysLeft--;
					}
				}
				// Weapons and movement both need to check neighboring spaces.
				uint16_t newIndex = i;
				bool clockAvailable = false;
				bool counterAvailable = false;
				if ((i < kBogeyIndexMax) && (_bogeys[i + 1].type == B_NONE)) {
					clockAvailable = true;
				}
				if ((i > kBogeyIndexMin) && (_bogeys[i - 1].type == B_NONE)) {
					counterAvailable = true;
				}

				// Check if bogey is going to fire a weapon.
				if (weaponRefresh(_bogeys[i].type) > 0) {
					if (_bogeys[i].weaponElapsed >= weaponRefresh(_bogeys[i].type)) {
						// Weapon is ready to fire.
						DEBUG("Weapon ready.");
						Bogey weapon;
						if ((_bogeys[i].type == B_MEDIUM) ||
							(_bogeys[i].type == B_HEAVY)) {
							weapon = Bogey(B_DISRUPTOR);
							_bogeysLeft++;
						} else if (_bogeys[i].type == B_BOSS) {
							weapon = Bogey(B_MISSILE);
							_bogeysLeft++;
						}

						if (clockAvailable && counterAvailable) {
							// Our coin flip...
							if (_bogeys[i].weaponElapsed % 2 == 0) {
								newIndex = i + 1;
							} else {
								newIndex = i - 1;
							}
						} else if (clockAvailable) {
							// Can only move clockwise.
							newIndex++;
							clockAvailable = false;
						} else if (counterAvailable) {
							// Can only move counter-clockwise.
							newIndex--;
							counterAvailable = false;
						}
						// Only move if the index changed.
						if (i != newIndex) {
							_bogeys[newIndex] = weapon;
							DEBUG("Weapon launched at %d", i);
						}

						_bogeys[i].weaponElapsed = 0;
					}
				}

				// If movement > 0, bogey might shift led indexes.
				if (_bogeys[i].movement > 0) {
					// Enough time has passed for the bogey to consider moving.
					if (_bogeys[i].movementElapsed >= _bogeys[i].movementInterval) {
						// Random # 1 - 10, second value is exclusive.
						uint8_t randVal = random(1, 11);
						if (randVal <= _bogeys[i].movement) {
							// Can move either direction.
							if (clockAvailable && counterAvailable) {
								// Our coin flip...
								if (_bogeys[i].movementElapsed % 2 == 0) {
									newIndex = i + 1;
								} else {
									newIndex = i - 1;
								}
							} else if (clockAvailable) {
								// Can only move clockwise.
								newIndex++;
							} else if (counterAvailable) {
								// Can only move counter-clockwise.
								newIndex--;
							}
							// Only move if the index changed.
							if (i != newIndex) {
								_bogeys[newIndex] = _bogeys[i];
								_bogeys[i] = Bogey();
								//DEBUG("Bogey %d moved to %d", i, newIndex);
							}
						}
						_bogeys[i].movementElapsed = 0;
					}
				}
			} else if (_bogeys[i].type == B_BURNING) {
				if (_hardwareManager->ledSet()->getLedState(i) == LED_IDLE) {
					//DEBUG("Done burning: %d", i);
					_bogeys[i] = Bogey();
					_bogeysLeft--;
				}
			}
		}
	}
}

void GameManager::updateGameOver(bool aWonGame) {
	uint8_t scrollIndex = _hardwareManager->bubbleDisplay()->scrollIndex();
	bool scrolling = _hardwareManager->bubbleDisplay()->scrolling();
	if (_gameOverStep == 0) {
		// Stop the scrolling text.
		_hardwareManager->bubbleDisplay()->resetDisplays();
		_gameOverStep++;
	} else if (_gameOverStep == 1) {
		if (!scrolling && (scrollIndex == 0)) {
			uint8_t length = 0;
			if (aWonGame) {
				_hardwareManager->bubbleDisplay()->setData(kMsgGameWon, kMsgGameWonLen);
				length = kMsgGameWonLen;
			} else {
				uint8_t randVal = random(1, 4);
				if (randVal == 1) {
					_hardwareManager->bubbleDisplay()->setData(kMsgDeadOne, kMsgDeadOneLen);
					length = kMsgDeadOneLen;
				} else if (randVal == 2) {
					_hardwareManager->bubbleDisplay()->setData(kMsgDeadTwo, kMsgDeadTwoLen);
					length = kMsgDeadTwoLen;
				} else if (randVal == 3) {
					_hardwareManager->bubbleDisplay()->setData(kMsgDeadThree, kMsgDeadThreeLen);
					length = kMsgDeadThreeLen;
				}
			}
			_hardwareManager->bubbleDisplay()->setScrollStyle(SCROLL_LEFT, 1, kBubbleDisplayScrollFreq, length);
			_hardwareManager->bubbleDisplay()->startScrolling();
		} else if (!scrolling || _hardwareManager->button()->wasClicked()) {
			// Stop the scrolling text.
			_hardwareManager->bubbleDisplay()->resetDisplays();
			_gameOverStep++;
		}
	} else if (_gameOverStep == 2) {
		if (!scrolling && (scrollIndex == 0)) {
			_hardwareManager->bubbleDisplay()->setScrollStyle(SCROLL_LEFT, 1, kBubbleDisplayScrollFreq, kMsgFinalScoreLen);
			_hardwareManager->bubbleDisplay()->setData(kMsgFinalScore, kMsgFinalScoreLen);
			_hardwareManager->bubbleDisplay()->setInteger(_playerScore.kills);
			_hardwareManager->bubbleDisplay()->startScrolling();
		} else if (!scrolling || _hardwareManager->button()->wasClicked()) {
			// Stop the scrolling text.
			_hardwareManager->bubbleDisplay()->resetDisplays();
			_playerScore.standing = _dataManager->highScoreStanding(&_playerScore);
			DEBUG("Standing: %d", _playerScore.standing);
			if (_playerScore.standing > 0) {
				_stateController->changeState(STATE_HIGH_SCORE);
			} else {
				_dataManager->saveScoreData(&_playerScore);
				_hardwareManager->printer()->printScoreData(&_playerScore);
				_gameOverStep++;
			}
		}
	} else if (_gameOverStep == 3) {
		if (!scrolling && (scrollIndex == 0)) {
			_hardwareManager->bubbleDisplay()->setData(kMsgGameOver, kMsgGameOverLen);
			_hardwareManager->bubbleDisplay()->setScrollStyle(SCROLL_LEFT, 8, kBubbleDisplayJumpFreq, kMsgGameOverLen);
			_hardwareManager->bubbleDisplay()->startScrolling();
		} else if (!scrolling) {
			// Stop the scrolling text.
			_hardwareManager->bubbleDisplay()->resetDisplays();
			_gameOverStep++;
		}
	} else if (_gameOverStep == 4) {
		_stateController->changeState(STATE_INIT);
	}
}

void GameManager::updateHighScore() {
	uint8_t scrollIndex = _hardwareManager->bubbleDisplay()->scrollIndex();
	bool scrolling = _hardwareManager->bubbleDisplay()->scrolling();
	if (_highScoreStep == 0) {
		// Stop the scrolling text.
		//_hardwareManager->rotary()->active = false;
		_hardwareManager->bubbleDisplay()->resetDisplays();
		_highScoreStep++;
	} else if (_highScoreStep == 1) {
		if (!scrolling && (scrollIndex == 0)) {
			_hardwareManager->bubbleDisplay()->setData(kMsgHighScoreHelp, kMsgHighScoreHelpLen);
			_hardwareManager->bubbleDisplay()->setScrollStyle(SCROLL_LEFT, 1, kBubbleDisplayScrollFreq, kMsgHighScoreHelpLen);
			_hardwareManager->bubbleDisplay()->startScrolling();
		} else if (!scrolling || _hardwareManager->button()->wasClicked()) {
			// Stop the scrolling text.
			_hardwareManager->bubbleDisplay()->resetDisplays();
			_highScoreStep++;
		}
	} else if (_highScoreStep == 2) {
		if (!scrolling && (scrollIndex == 0)) {
			_initialsBuffer = new char[17]();
			strncpy(_initialsBuffer, "        ", 8);
			itoa(_playerScore.standing, _initialsBuffer, 10);
			if (_playerScore.standing < 10) {
				_initialsBuffer[1] = '.';
				//_initialsBuffer[0] = _initialsBuffer[0] | 0x80;
			} else {
				_initialsBuffer[2] = '.';
				//_initialsBuffer[1] = _initialsBuffer[1] | 0x80;
			}
			memcpy(_initialsBuffer + 8, _initialsBuffer, 8);
			_initialsBuffer[5] = '_';

			_hardwareManager->bubbleDisplay()->setData(_initialsBuffer, 16);
			_hardwareManager->bubbleDisplay()->setScrollStyle(SCROLL_LEFT_WRAP, 8, kBubbleDisplayJumpFreq, 16);
			_hardwareManager->bubbleDisplay()->startScrolling();
			//_hardwareManager->rotary()->active = true;
			_hardwareManager->rotary()->resetRotary();
			_hardwareManager->button()->resetButton();
			_highScoreStep++;
		}
	} else if (_highScoreStep == 3) {
		if (scrolling && _hardwareManager->rotary()->newDialEvent()) {
			uint8_t dialValue = _hardwareManager->rotary()->lastDialed();

			if (_initialIndex > 0) {
				DEBUG("I Index: %d", _initialIndex);
				_charOffset = 0;
				_playerScore.initials[_initialIndex - 1] = _initialChar;
				_initialsBuffer[12 + _initialIndex] = _initialChar;
				if (_initialIndex == 3) {
					_hardwareManager->bubbleDisplay()->resetDisplays();
					_highScoreStep++;
					return;
				}
			}
			_initialDialed = dialValue;
			switch (dialValue) {
				case 2: {
					_initialChar = 'A';
					break;
				}
				case 3: {
					_initialChar = 'D';
					break;
				}
				case 4: {
					_initialChar = 'G';
					break;
				}
				case 5: {
					_initialChar = 'J';
					break;
				}
				case 6: {
					_initialChar = 'M';
					break;
				}
				case 7: {
					_initialChar = 'P';
					break;
				}
				case 8: {
					_initialChar = 'T';
					break;
				}
				case 9: {
					_initialChar = 'W';
					break;
				}
				case 0: {
					_initialChar = 'Z';
					break;
				}
			}
			_initialsBuffer[5 + _initialIndex] = _initialChar;
			_hardwareManager->bubbleDisplay()->setCursorPos(0);
			_hardwareManager->bubbleDisplay()->setData(_initialsBuffer, 16);
			_initialIndex++;
		} else if (_hardwareManager->button()->wasClicked() && (_initialIndex > 0)) {
			DEBUG("Clicked!");
			if (_initialDialed == 7) { // Special case for PQRS, others have 3
				if (_charOffset < 3) {
					_charOffset++;
					_initialChar++;
				} else {
					_charOffset = 0;
					_initialChar -= 3;
				}
			} else if (_initialDialed == 1) {
				// TODO: Special symbols.
			} else if (_initialDialed != 0) { // 0 is Z, only one char
				if (_charOffset < 2) {
					_charOffset++;
					_initialChar++;
				} else {
					_charOffset = 0;
					_initialChar -= 2;
				}
			}
			_initialsBuffer[4 + _initialIndex] = _initialChar;
			_hardwareManager->bubbleDisplay()->setCursorPos(0);
			DEBUG("B: %s", _initialsBuffer);
			_hardwareManager->bubbleDisplay()->setData(_initialsBuffer, 16);
		}
	} else if (_highScoreStep == 4) {
		if (!scrolling && (scrollIndex == 0)) {
			_hardwareManager->bubbleDisplay()->setScrollStyle(SCROLL_LEFT, 8, kBubbleDisplayJumpFreq, 16);
			_hardwareManager->bubbleDisplay()->setData(_initialsBuffer, 16);
			_hardwareManager->bubbleDisplay()->startScrolling();
		} else if (!scrolling) {
			_hardwareManager->bubbleDisplay()->resetDisplays();
			_highScoreStep++;
		}
	} else if (_highScoreStep == 5) {
		if (!scrolling && (scrollIndex == 0)) {
			//_playerScore.kills = random(10, 5001);
			//DEBUG("Kills: %d", _playerScore.kills);
			_dataManager->saveScoreData(&_playerScore);
			_hardwareManager->printer()->printScoreData(&_playerScore);
			strncpy(_initialsBuffer, "CONGRATS  XXX!  ", 16);
			strncpy(_initialsBuffer + 10, _playerScore.initials, 3);
			_hardwareManager->bubbleDisplay()->setScrollStyle(SCROLL_LEFT, 8, kBubbleDisplayJumpFreq, 16);
			_hardwareManager->bubbleDisplay()->setData(_initialsBuffer, 16);
			_hardwareManager->bubbleDisplay()->startScrolling();
		} else if (!scrolling) {
			_hardwareManager->bubbleDisplay()->resetDisplays();
			_highScoreStep++;
		}
	} else if (_highScoreStep == 6) {
		delete[] _initialsBuffer;
		_stateController->changeState(STATE_INIT);
	}
}

/*void GameManager::updateGameWon() {
	uint8_t scrollIndex = _hardwareManager->bubbleDisplay()->scrollIndex();
	bool scrolling = _hardwareManager->bubbleDisplay()->scrolling();
	if (_gameWonStep == 0) {
		// Stop the scrolling text.
		_hardwareManager->bubbleDisplay()->resetDisplays();
		_gameWonStep++;
	} else if (_gameWonStep == 1) {
		if (!scrolling && (scrollIndex == 0)) {
			_hardwareManager->bubbleDisplay()->setData(kMsgGameWon, kMsgGameWonLen);
			_hardwareManager->bubbleDisplay()->setScrollStyle(SCROLL_LEFT, 1, kBubbleDisplayScrollFreq, kMsgGameWonLen);
			_hardwareManager->bubbleDisplay()->startScrolling();
		} else if (!scrolling) {
			// Stop the scrolling text.
			_hardwareManager->bubbleDisplay()->resetDisplays();
			_gameWonStep++;
		}
	} else if (_gameWonStep == 2) {
		if (!scrolling && (scrollIndex == 0)) {
			_hardwareManager->bubbleDisplay()->setData(kMsgFinalScore, kMsgFinalScoreLen);
			_hardwareManager->bubbleDisplay()->setScrollStyle(SCROLL_LEFT, 1, kBubbleDisplayScrollFreq, kMsgFinalScoreLen);
			_hardwareManager->bubbleDisplay()->setInteger(_score);
			_hardwareManager->bubbleDisplay()->startScrolling();
		} else if (!scrolling) {
			// Stop the scrolling text.
			_hardwareManager->bubbleDisplay()->resetDisplays();
			_gameOverStep++;
		}
	} else if (_gameOverStep == 3) {
		if (!scrolling && (scrollIndex == 0)) {
			_hardwareManager->bubbleDisplay()->setData(kMsgGameOver, kMsgGameOverLen);
			_hardwareManager->bubbleDisplay()->setScrollStyle(SCROLL_LEFT, 8, kBubbleDisplayJumpFreq, kMsgGameOverLen);
			_hardwareManager->bubbleDisplay()->startScrolling();
		} else if (!scrolling) {
			// Stop the scrolling text.
			_hardwareManager->bubbleDisplay()->resetDisplays();
			if (hasHighScore()) {
				_stateController->changeState(STATE_HIGH_SCORE);
			} else {
				_stateController->changeState(STATE_INIT);
			}
		}
}*/

/*void GameManager::nextIntroStep() {
	_introStep++;
}*/

uint8_t GameManager::dialToIndex(uint8_t aDialed) {
	uint8_t val = 0;
	if (kRadarDialValue == 0) {
		val = 9 - aDialed;
	} else if (kRadarDialValue == 1) {
		if (aDialed != 0) {
			val = 10 - aDialed;
		}
	}
	return val;
}

uint8_t GameManager::indexToDial(uint8_t aLedIndex) {
	uint8_t val = 0;
	if (kRadarDialValue == 0) {
		val = 9 - aLedIndex;
	} else if (kRadarDialValue == 1) {
		if (aLedIndex != 0) {
			val = 10 - aLedIndex;
		}
	}
	return val;
}

uint16_t GameManager::currentWave() {
	// Return actual wave, not index.
	return _currentWave + 1;
}

uint8_t GameManager::currentSet() {
	return _currentSet;
}

// TODO: Needed???
void GameManager::refreshRadar() {
	//_radarBrightness = 0;
	//_radarElapsed = 0;
}

// TODO: Needed???
bool GameManager::radarOff() {
	//return (_radarBrightness == 0);
	return false;
}

void GameManager::displayScore() {
	_hardwareManager->bubbleDisplay()->resetDisplays();
	_hardwareManager->bubbleDisplay()->setData("KILLS ", 6);
	_hardwareManager->bubbleDisplay()->setInteger(_playerScore.kills);
}

uint8_t GameManager::bogeyCount() {
	uint8_t bogeyCount = 0;
	for (uint8_t i = 0; i < 9; ++i) {
		if (_bogeys[i].type != B_NONE) {
			if (_bogeys[i].type == B_BURNING) {
				DEBUG("Burning: %d", i);
				//_bogeys[i] = Bogey();
			} else {
				bogeyCount++;
			}
		}
	}
	return bogeyCount;
}

void GameManager::resetBogeys() {
	for (uint8_t i = 0; i < 9; ++i) {
		_bogeys[i] = Bogey();
	}
	_bogeysLeft = 0;
}

bool GameManager::fireWeapon(int8_t aDialValue) {
	if (_bombing || _takingHit) {
		return false;
	}
	bool hit = false;
	_playerScore.shotsFired++;
	uint8_t index = dialToIndex(aDialValue);
	if (_bogeys[index].type == B_NONE) {
		//DEBUG("Missed: %d", index);
		_playerScore.misses++;
		playMissAnim(index);
	} else {
		if (_bogeys[index].type >= B_LIGHT) {
			_bogeys[index].health--;
			//DEBUG("Hit: %d, Health: %d", index, _bogeys[index].health);
			if (_bogeys[index].health > 0) {
				//DEBUG("Play hit");
				playHitAnim(index);
			} else {
				incrementKills(_bogeys[index].type);
				displayScore();
				_bogeys[index].type = B_BURNING;
				DEBUG("Hit I: %d, T: %d", index, _bogeys[index].type);
				//_bogeysLeft--;
				playDestroyAnim(index);
			}
			hit = true;
		}
	}
	return hit;
}

void GameManager::detonateBomb() {
	if (_bombing || _takingHit) {
		return;
	}
	if (_bombs > 0) {
		_bombs--;
		_playerScore.bombsUsed++;
		if (_bombs == 0) {
			_hardwareManager->button()->turnLedOff();
		}

		for (uint8_t i = 0; i < 9; ++i) {
			if (_bogeys[i].type >= B_LIGHT) {
				_bogeys[i].health--;
				if (_bogeys[i].health == 0) {
					incrementKills(_bogeys[i].type);
					_bogeys[i] = Bogey();
					_bogeysLeft--;
				}
			}
		}
		displayScore();
		_bombing = true;
		playBombAnim();
	}
}

void GameManager::setupNextSet() {
	//_hardwareManager->ledSet()->resetLeds();
	_currentSet++;
	uint8_t itr = _currentWave / 10;
	uint8_t wave = _currentWave % 10;
	bool startingSet = false;
	switch(wave) {
		case 0:
			startingSet = setupWaveOne(_currentSet, itr);
			break;
		case 1:
			startingSet = setupWaveTwo(_currentSet, itr);
			break;
		case 2:
			startingSet = setupWaveThree(_currentSet, itr);
			break;
		case 3:
			startingSet = setupWaveFour(_currentSet, itr);
			break;
		case 4:
			startingSet = setupWaveFive(_currentSet, itr);
			break;
		case 5:
			startingSet = setupWaveSix(_currentSet, itr);
			break;
		case 6:
			startingSet = setupWaveSeven(_currentSet, itr);
			break;
		case 7:
			startingSet = setupWaveEight(_currentSet, itr);
			break;
		case 8:
			startingSet = setupWaveNine(_currentSet, itr);
			break;
		case 9:
			startingSet = setupWaveTen(_currentSet, itr);
			break;
		default:
			//ERROR("Invalid Wave");
			_stateController->changeState(STATE_ERROR);
	}
	if (startingSet == false) {
		_stateController->changeState(STATE_END_WAVE);
	} else {
		_bogeysLeft = bogeyCount();
		DEBUG("Left: %d", _bogeysLeft);
	}
}

void GameManager::endWave() {
	resetBogeys();
	_hardwareManager->button()->resetButton();
	_hardwareManager->rotary()->resetRotary();
	_hardwareManager->bubbleDisplay()->resetDisplays();
	_hardwareManager->ledSet()->resetLeds();
	_currentWave++;
	_currentSet = 0;
	if (_currentWave >= kMaxWaves) {
		scoreDataEnd(true);
		_stateController->changeState(STATE_GAME_WON);
	} else {
		_stateController->changeState(STATE_SETUP_WAVE);
	}
}

void GameManager::playRadarAnim() {
	if (_bombing || _takingHit || _disruptorHit) {
		return;
	}
	//_radarBrightness = 0;
	//_radarElapsed = 0;
	_playerScore.scans++;
	uint16_t delay = 0;
	for (int16_t i = _hardwareManager->ledSet()->ledCount() - 1; i >= 0; --i) {
		// Make sure we are within the range of possible bogeys, then check if one is at that index.
		if ((i >= kBogeyIndexMin) && (i <= kBogeyIndexMax) && (_bogeys[i].type >= B_LIGHT) && (_bogeys[i].deployDelay == 0)) {
			// There are more LEDs than phone dial #s, so stay within range. Make sure it is a bogey type, and that it isn't waiting to appear.
			if (timeToArrive(_bogeys[i].type) == 0) {
				// At this point, we can differentiate color if
				// we decide to have more than one stationary bogey.
				Animation a = Animation();
				a.startColor = Color(255, 0, 255);
				a.tweenTime = kBogeyFadeAnimMillis;
				a.delayTime = delay;
				_hardwareManager->ledSet()->animateLed(i, a);
			} else if (_bogeys[i].deployDelay == 0) {
				//DEBUG("Bogey Index: %d", i);
				float percent = (float)_bogeys[i].travelElapsed / (float)timeToArrive(_bogeys[i].type);
				Color change;
				Color color;

				float redChange;
				float greenChange;
				float blueChange;

				Animation a = Animation();
				a.ease = EASE_LINEAR;
				a.tweenTime = kBogeyFadeAnimMillis;

				if (percent <= 0.3f) { // Green to yellow.
					redChange = kColorYellow.red - kColorGreen.red;
					greenChange = kColorYellow.green - kColorGreen.green;
					blueChange = kColorYellow.blue - kColorGreen.blue;
					color = kColorGreen;
				} else if (percent <= 0.6f) { // Yellow to orange.
					redChange = kColorOrange.red - kColorYellow.red;
					greenChange = kColorOrange.green - kColorYellow.green;
					blueChange = kColorOrange.blue - kColorYellow.blue;
					color = kColorYellow;
				} else if (percent <= 0.9f) { // Orange to red.
					redChange = kColorRed.red - kColorOrange.red;
					greenChange = kColorRed.green - kColorOrange.green;
					blueChange = kColorRed.blue - kColorOrange.blue;
					color = kColorOrange;
				} else { // Red blinking.
					redChange = kColorBlack.red - kColorRed.red;
					greenChange = kColorBlack.green - kColorRed.green;
					blueChange = kColorBlack.blue - kColorRed.blue;
					color = kColorRed;
					a.ease = EASE_BLINK;
					a.repeats = 3;
					a.tweenTime = kBogeyClosingBlink;
				}
				if (_bogeys[i].type == B_DISRUPTOR) {
					// TODO: Something unique with color?
					//redChange = 0;
					//color.red = 0;
				} else if (_bogeys[i].type == B_MISSILE) {
					// TODO: Something unique with color?
					//blueChange = 0;
					//color.blue = 0;
				}
				color.red = _hardwareManager->ledSet()->easeLinear(_bogeys[i].travelElapsed, timeToArrive(_bogeys[i].type), color.red, redChange);
				color.green = _hardwareManager->ledSet()->easeLinear(_bogeys[i].travelElapsed, timeToArrive(_bogeys[i].type), color.green, greenChange);
				color.blue = _hardwareManager->ledSet()->easeLinear(_bogeys[i].travelElapsed, timeToArrive(_bogeys[i].type), color.blue, blueChange);

				/*if (_bogeys[i].type == B_DISRUPTOR) {
					color.red = 0;
				} else if (_bogeys[i].type == B_MISSILE) {
					color.blue = 0;
				}*/
				a.startColor = color;
				a.endColor = kColorBlack;
				a.delayTime = delay;
				/*Serial.print("R: ");
				Serial.print(color.red);
				Serial.print(", G: ");
				Serial.print(color.green);
				Serial.print(", B: ");
				Serial.println(color.blue);*/
				_hardwareManager->ledSet()->animateLed(i, a);
			}
		} else if ((i >= kBogeyIndexMax) || (_bogeys[i].type == B_NONE) || (_bogeys[i].deployDelay > 0)) {
			DEBUG("2I: %d, T: %d", i, _bogeys[i].type);
			//DEBUG("No Bogey Index: %d", i);
			//Animation a = _sweepGrnAnim;
			//a.delayTime = delay;
			Animation a = sweepGrnAnim();
			a.delayTime = delay;
			_hardwareManager->ledSet()->animateLed(i, a);
		} else { // _bogeys[i].type == B_BURNING
			DEBUG("I: %d, T: %d", i, _bogeys[i].type);
			//DEBUG("Burning Index: %d", i);
			/*_bogeys[i].type = B_NONE;
			_hardwareManager->ledSet()->killAnim(i);
			Animation a = sweepRedAnim();
			a.repeatDelay = 500;
			a.delayTime = delay;
			_hardwareManager->ledSet()->animateLed(i, a, true);*/
		}
		delay += kRadarAnimStaggerMillis;
	}
}

void GameManager::playMissAnim(uint16_t aLedIndex) {
	Animation a = Animation();
	a.endColor = Color(0, 0, 127);
	a.repeats = 1;
	a.yoyo = true;
	a.tweenTime = kFireMissAnimMillis;
	a.ease = EASE_QUAD_IN_OUT;
	_hardwareManager->ledSet()->animateLed(aLedIndex, a, false);
}

void GameManager::playHitAnim(uint16_t aLedIndex) {
	Animation a = Animation();
	a.endColor = Color(255, 255, 255);
	a.repeats = 1;
	a.yoyo = true;
	a.tweenTime = kFireHitAnimMillis;
	a.ease = EASE_QUAD_OUT;
	_hardwareManager->ledSet()->animateLed(aLedIndex, a, true);
}

void GameManager::playDestroyAnim(uint16_t aLedIndex) {
	Animation destroy1 = Animation();
	destroy1.startColor = Color(0, 0, 0);
	destroy1.endColor = Color(200, 0, 0); // Red
	destroy1.tweenTime = 200;
	destroy1.ease = EASE_LINEAR;

	Animation destroy2 = Animation();
	destroy2.startColor = Color(200, 0, 0);
	destroy2.endColor = Color(200, 200, 0); // Yellow
	destroy2.tweenTime = 400;
	destroy2.ease = EASE_QUAD_OUT;

	Animation destroy3 = Animation();
	destroy3.startColor = Color(200, 200, 0);
	destroy3.endColor = Color(255, 102, 0); // Orange
	destroy3.yoyo = false;
	destroy3.repeats = 2;
	destroy3.tweenTime = 200;
	destroy3.ease = EASE_LINEAR;

	Animation destroy4 = Animation();
	destroy4.startColor = Color(200, 102, 0);
	destroy4.endColor = Color(0, 0, 0);
	destroy4.tweenTime = 200;
	destroy4.ease = EASE_QUAD_IN_OUT;

	_destroyAnimSeries[0] = destroy1;
	_destroyAnimSeries[1] = destroy2;
	_destroyAnimSeries[2] = destroy3;
	_destroyAnimSeries[3] = destroy4;

	//Animation destroyAnimSeries[kDestroyAnimLength] = { destroy1, destroy2, destroy3, destroy4 };

	_hardwareManager->ledSet()->animateSeries(aLedIndex, _destroyAnimSeries, kDestroyAnimLength, 0, true);
}

void GameManager::playBombAnim() {
	Animation bomb1 = Animation();
	bomb1.endColor = Color(200, 0, 0); // Red
	bomb1.tweenTime = 500;
	bomb1.ease = EASE_LINEAR;

	Animation bomb2 = Animation();
	bomb2.startColor = Color(200, 0, 0);
	bomb2.endColor = Color(200, 200, 0); // Yellow
	bomb2.tweenTime = 250;
	bomb2.ease = EASE_QUAD_IN_OUT;
	bomb2.yoyo = true;
	bomb2.repeats = 2;

	Animation bomb3 = Animation();
	bomb3.startColor = Color(200, 200, 0);
	bomb3.endColor = Color(255, 102, 0); // Orange
	bomb3.tweenTime = 250;
	bomb3.ease = EASE_QUAD_IN_OUT;
	bomb3.yoyo = true;
	bomb3.repeats = 2;

	Animation bomb4 = Animation();
	bomb4.startColor = Color(255, 102, 0);
	bomb4.endColor = Color(255, 255, 255); // White
	bomb4.tweenTime = 500;
	bomb4.ease = EASE_QUAD_IN_OUT;
	bomb4.repeats = 0;

	Animation bomb5 = Animation();
	bomb5.startColor = Color(255, 255, 255);
	bomb5.endColor = Color(0, 0, 0);
	bomb5.tweenTime = 700;
	bomb5.ease = EASE_LINEAR;

	//Animation bombAnimSeries[kBombAnimLength] = { bomb1, bomb2, bomb3, bomb4, bomb5 };

	_bombAnimSeries[0] = bomb1;
	_bombAnimSeries[1] = bomb2;
	_bombAnimSeries[2] = bomb3;
	_bombAnimSeries[3] = bomb4;
	_bombAnimSeries[4] = bomb5;

	for (int16_t i = 0; i < _hardwareManager->ledSet()->ledCount(); ++i) {
		_bombAnimSeries[0].delayTime = random(0, 500);
		_bombAnimSeries[1].repeats = random(2, 6);
		_bombAnimSeries[2].repeats = random(2, 6);
		_hardwareManager->ledSet()->animateSeries(i, _bombAnimSeries, kBombAnimLength, 0);
	}
}

void GameManager::playPlayerHitAnim() {
	Animation playerHit1 = Animation();
	playerHit1.endColor = Color(127, 0, 0);
	playerHit1.tweenTime = 300;
	playerHit1.ease = EASE_QUAD_OUT;

	Animation playerHit2 = Animation();
	playerHit2.startColor = Color(127, 0, 0);
	playerHit2.tweenTime = 300;
	playerHit2.ease = EASE_QUAD_IN;

	//Animation playerHitAnimSeries[kPlayHitAnimLength] = { playerHit1, playerHit2 };

	_playerHitAnimSeries[0] = playerHit1;
	_playerHitAnimSeries[1] = playerHit2;

	for (int16_t i = 0; i < _hardwareManager->ledSet()->ledCount(); ++i) {
		_hardwareManager->ledSet()->animateSeries(i, _playerHitAnimSeries, kPlayHitAnimLength, 0, true);
	}
}

void GameManager::playPlayerDeathAnim() {
	_hardwareManager->ledSet()->animateAll(sweepRedAnim(), false, kRadarAnimStaggerMillis);
}

void GameManager::playRadarDisruptor() {
	Animation disruptor1 = Animation();
	disruptor1.endColor = Color(0, 100, 0);
	disruptor1.tweenTime = 100;

	Animation disruptor2 = Animation();
	disruptor2.startColor = Color(0, 100, 0);
	disruptor2.tweenTime = 200;

	Animation disruptor3 = Animation();
	disruptor3.startColor = Color(0, 100, 0);
	disruptor3.tweenTime = 100;

	//Animation disruptorAnimSeries[kDisruptorAnimLength] = { disruptor1, disruptor2, disruptor3 };

	_disruptorAnimSeries[0] = disruptor1;
	_disruptorAnimSeries[1] = disruptor2;
	_disruptorAnimSeries[2] = disruptor3;

	for (int16_t i = 0; i < _hardwareManager->ledSet()->ledCount(); ++i) {
		_disruptorAnimSeries[0].delayTime = random(0, 250);
		_disruptorAnimSeries[1].ease = (EaseType)random(0, 2);
		_disruptorAnimSeries[1].tweenTime = random(kDisruptorMinAnimMillis, (kDisruptorMaxAnimMillis - _disruptorAnimSeries[0].delayTime));
		_disruptorAnimSeries[1].repeats = random(1, 4);
		_disruptorAnimSeries[2].ease = (EaseType)random(0, 2);
		_disruptorAnimSeries[2].tweenTime = kDisruptorMinAnimMillis;
		_hardwareManager->ledSet()->animateSeries(i, _disruptorAnimSeries, kDisruptorAnimLength, 0, true);
	}
}

/* Animation Accessors */

Animation GameManager::sweepGrnAnim() {
	Animation sweepGrnAnim = Animation();
	sweepGrnAnim.startColor = Color(0, 255, 0);
	sweepGrnAnim.tweenTime = kRadarAnimMillis;
	return sweepGrnAnim;
}

Animation GameManager::sweepRedAnim() {
	Animation sweepRedAnim = Animation();
	sweepRedAnim.startColor = Color(255, 0, 0);
	sweepRedAnim.tweenTime = kRadarAnimMillis;
	return sweepRedAnim;
}

/* Private Methods */

// New wave 1 for testing different enemy types
/*bool GameManager::setupWaveOne(uint8_t aSet, uint8_t aIteration) {
	_currentSet = aSet;
	if (aSet == 1) {
		_bogeys[6] = Bogey(B_LIGHT, 0, 8, 2000);
		_bogeys[5] = Bogey(B_LIGHT, 0, 8, 2000);
		_bogeys[4] = Bogey(B_LIGHT, 0, 2, 2000);
	} else if (aSet == 2) {
		_bogeys[5] = Bogey(B_MEDIUM, 0, 0, 0);
	} else if (aSet == 3) {
		_bogeys[4] = Bogey(B_HEAVY, 0, 0, 0);
	} else if (aSet == 4) {
		Serial.println("Boss time");
		_bogeys[3] = Bogey(B_BOSS, 0, 0, 0);
	} else {
		return false;
	}
	return true;
}*/

bool GameManager::setupWaveOne(uint8_t aSet, uint8_t aIteration) {
	_currentSet = aSet;
	if (aSet == 1) {
		_bogeys[6] = Bogey(B_LIGHT);
	} else if (aSet == 2) {
		_bogeys[5] = Bogey(B_LIGHT);
		_bogeys[7] = Bogey(B_LIGHT, 1500);
	} else if (aSet == 3) {
		_bogeys[randomPosition()] = Bogey(B_LIGHT);
		_bogeys[randomPosition()] = Bogey(B_LIGHT, 1500);
	} else {
		return false;
	}
	return true;
}

bool GameManager::setupWaveTwo(uint8_t aSet, uint8_t aIteration) {
	_currentSet = aSet;
	if (aSet == 1) {
		_bogeys[randomPosition()] = Bogey(B_LIGHT);
		_bogeys[randomPosition()] = Bogey(B_LIGHT, 1000);
	} else if (aSet == 2) {
		uint16_t pos = randomPosition(1, 7);
		_bogeys[pos] = Bogey(B_LIGHT);
		_bogeys[pos - 1] = Bogey(B_LIGHT, 1000);
		_bogeys[pos + 1] = Bogey(B_LIGHT, 1000);
	} else if (aSet == 3) {
		uint16_t pos = randomPosition(4, 8);
		_bogeys[pos] = Bogey(B_LIGHT);
		_bogeys[pos - 2] = Bogey(B_LIGHT, 1000);
		_bogeys[pos + 4] = Bogey(B_LIGHT, 2000);
	} else {
		return false;
	}
	return true;
}

bool GameManager::setupWaveThree(uint8_t aSet, uint8_t aIteration) {
	_currentSet = aSet;
	if (aSet == 1) {
		_bogeys[randomPosition()] = Bogey(B_LIGHT);
		_bogeys[randomPosition()] = Bogey(B_LIGHT, 1000);
		_bogeys[randomPosition()] = Bogey(B_LIGHT, 2000);
	} else if (aSet == 2) {
		uint16_t pos = randomPosition(4, 8);
		_bogeys[pos] = Bogey(B_MEDIUM);
		_bogeys[pos - 2] = Bogey(B_LIGHT, 1000);
		_bogeys[pos + 2] = Bogey(B_LIGHT, 1000);
	} else if (aSet == 3) {
		uint16_t pos = randomPosition(1, 7);
		_bogeys[pos] = Bogey(B_BOSS, 0, 3, 3000);
	} else {
		return false;
	}
	return true;
}

bool GameManager::setupWaveFour(uint8_t aSet, uint8_t aIteration) {
	_currentSet = aSet;
	if (aSet == 1) {

	} else if (aSet == 2) {

	} else if (aSet == 3) {

	} else {
		return false;
	}
	return true;
}

bool GameManager::setupWaveFive(uint8_t aSet, uint8_t aIteration) {
	_currentSet = aSet;
	if (aSet == 1) {

	} else if (aSet == 2) {

	} else if (aSet == 3) {

	} else {
		return false;
	}
	return true;
}

bool GameManager::setupWaveSix(uint8_t aSet, uint8_t aIteration) {
	_currentSet = aSet;
	if (aSet == 1) {

	} else if (aSet == 2) {

	} else if (aSet == 3) {

	} else {
		return false;
	}
	return true;
}

bool GameManager::setupWaveSeven(uint8_t aSet, uint8_t aIteration) {
	_currentSet = aSet;
	if (aSet == 1) {

	} else if (aSet == 2) {

	} else if (aSet == 3) {

	} else {
		return false;
	}
	return true;
}

bool GameManager::setupWaveEight(uint8_t aSet, uint8_t aIteration) {
	_currentSet = aSet;
	if (aSet == 1) {

	} else if (aSet == 2) {

	} else if (aSet == 3) {

	} else {
		return false;
	}
	return true;
}

bool GameManager::setupWaveNine(uint8_t aSet, uint8_t aIteration) {
	_currentSet = aSet;
	if (aSet == 1) {

	} else if (aSet == 2) {

	} else if (aSet == 3) {

	} else {
		return false;
	}
	return true;
}

bool GameManager::setupWaveTen(uint8_t aSet, uint8_t aIteration) {
	_currentSet = aSet;
	if (aSet == 1) {

	} else if (aSet == 2) {

	} else if (aSet == 3) {

	} else {
		return false;
	}
	return true;
}

void GameManager::incrementKills(BogeyType aBogeyType) {
	_playerScore.kills++;
	switch (aBogeyType) {
		case B_LIGHT:
			_playerScore.bLightKills++;
			break;
		case B_MEDIUM:
			_playerScore.bMediumKills++;
			break;
		case B_HEAVY:
			_playerScore.bHeavyKills++;
			break;
		case B_BOSS:
			_playerScore.bBossKills++;
			break;
		case B_DISRUPTOR:
			_playerScore.bDisruptorKills++;
			break;
		case B_MISSILE:
			_playerScore.bMissileKills++;
			break;
	}
}

uint16_t GameManager::maxHealth(BogeyType aType) {
	return BogeySpecs[aType].maxHealth;
}

uint16_t GameManager::timeToArrive(BogeyType aType) {
	return BogeySpecs[aType].timeToArrive;
}

uint16_t GameManager::weaponRefresh(BogeyType aType) {
	return BogeySpecs[aType].weaponRefresh;
}

uint16_t GameManager::randomPosition() {
	return randomPosition(0, 8);
}

uint16_t GameManager::randomPosition(uint16_t aLower, uint16_t aHigher) {
	uint16_t position = random(aLower, aHigher + 1);
	while (_bogeys[position].type != B_NONE) {
		position = random(aLower, aHigher + 1);
	}
	return position;
}

//void GameManager::initAnimations() {
	/*_sweepGrnAnim.startColor = Color(0, 255, 0);
	_sweepGrnAnim.endColor = Color(0, 0, 0);
	_sweepGrnAnim.repeats = 0;
	_sweepGrnAnim.yoyo = false;
	_sweepGrnAnim.tweenTime = kRadarAnimMillis;
	_sweepGrnAnim.delayTime = 0;
	_sweepGrnAnim.repeatDelay = 0;
	_sweepGrnAnim.ease = EASE_LINEAR;*/

	/*_sweepRedAnim.startColor = Color(255, 0, 0);
	_sweepRedAnim.endColor = Color(0, 0, 0);
	_sweepRedAnim.repeats = 0;
	_sweepRedAnim.yoyo = false;
	_sweepRedAnim.tweenTime = kRadarAnimMillis;
	_sweepRedAnim.delayTime = 0;
	_sweepRedAnim.repeatDelay = 500;
	_sweepRedAnim.ease = EASE_LINEAR;*/

	/*_bossShipAnim = Animation();
	_bossShipAnim.startColor = Color(255, 0, 255);
	_bossShipAnim.endColor = Color(0, 0, 0);
	_bossShipAnim.repeats = 0;
	_bossShipAnim.yoyo = false;
	_bossShipAnim.tweenTime = kBogeyFadeAnimMillis;
	_bossShipAnim.delayTime = 0;
	_bossShipAnim.repeatDelay = 0;
	_bossShipAnim.ease = EASE_LINEAR;*/

	/*_missAnim = Animation();
	_missAnim.startColor = Color(0, 0, 0);
	_missAnim.endColor = Color(0, 0, 127);
	_missAnim.repeats = 1;
	_missAnim.yoyo = true;
	_missAnim.tweenTime = kFireMissAnimMillis;
	_missAnim.ease = EASE_QUAD_IN_OUT;*/

	/*_hitAnim = Animation();
	_hitAnim.startColor = Color(0, 0, 0);
	_hitAnim.endColor = Color(255, 255, 255);
	_hitAnim.repeats = 1;
	_hitAnim.yoyo = true;
	_hitAnim.tweenTime = kFireHitAnimMillis;
	_hitAnim.ease = EASE_QUAD_OUT;*/

	/*Animation playerHit1 = Animation();
	playerHit1.startColor = Color(0, 0, 0);
	playerHit1.endColor = Color(127, 0, 0);
	playerHit1.tweenTime = 300;
	playerHit1.ease = EASE_QUAD_OUT;

	Animation playerHit2 = Animation();
	playerHit2.startColor = Color(127, 0, 0);
	playerHit2.endColor = Color(0, 0, 0);
	playerHit2.tweenTime = 300;
	playerHit2.ease = EASE_QUAD_IN;

	_playerHitAnimSeries[0] = playerHit1;
	_playerHitAnimSeries[1] = playerHit2;*/

	/*Animation disruptor1 = Animation();
	disruptor1.startColor = Color(0, 0, 0);
	disruptor1.endColor = Color(0, 100, 0);
	disruptor1.tweenTime = 100;
	disruptor1.ease = EASE_LINEAR;

	Animation disruptor2 = Animation();
	disruptor2.startColor = Color(0, 100, 0);
	disruptor2.endColor = Color(0, 0, 0);
	disruptor2.tweenTime = 200;
	disruptor2.ease = EASE_LINEAR;

	Animation disruptor3 = Animation();
	disruptor3.startColor = Color(0, 100, 0);
	disruptor3.endColor = Color(0, 0, 0);
	disruptor3.tweenTime = 100;
	disruptor3.ease = EASE_LINEAR;

	_disruptorAnimSeries[0] = disruptor1;
	_disruptorAnimSeries[1] = disruptor2;
	_disruptorAnimSeries[2] = disruptor3;*/

	/*Animation destroy1 = Animation();
	destroy1.startColor = Color(0, 0, 0);
	destroy1.endColor = Color(200, 0, 0); // Red
	destroy1.tweenTime = 200;
	destroy1.ease = EASE_LINEAR;

	Animation destroy2 = Animation();
	destroy2.startColor = Color(200, 0, 0);
	destroy2.endColor = Color(200, 200, 0); // Yellow
	destroy2.tweenTime = 400;
	destroy2.ease = EASE_QUAD_OUT;

	Animation destroy3 = Animation();
	destroy3.startColor = Color(200, 200, 0);
	destroy3.endColor = Color(255, 102, 0); // Orange
	destroy3.yoyo = false;
	destroy3.repeats = 2;
	destroy3.tweenTime = 200;
	destroy3.ease = EASE_LINEAR;

	Animation destroy4 = Animation();
	destroy4.startColor = Color(200, 102, 0);
	destroy4.endColor = Color(0, 0, 0);
	destroy4.tweenTime = 200;
	destroy4.ease = EASE_QUAD_IN_OUT;

	_destroyAnimSeries[0] = destroy1;
	_destroyAnimSeries[1] = destroy2;
	_destroyAnimSeries[2] = destroy3;
	_destroyAnimSeries[3] = destroy4;*/

	/*Animation bomb1 = Animation();
	bomb1.startColor = Color(0, 0, 0);
	bomb1.endColor = Color(200, 0, 0); // Red
	bomb1.tweenTime = 500;
	bomb1.ease = EASE_LINEAR;

	Animation bomb2 = Animation();
	bomb2.startColor = Color(200, 0, 0);
	bomb2.endColor = Color(200, 200, 0); // Yellow
	bomb2.tweenTime = 250;
	bomb2.ease = EASE_QUAD_IN_OUT;
	bomb2.yoyo = true;
	bomb2.repeats = 2;

	Animation bomb3 = Animation();
	bomb3.startColor = Color(200, 200, 0);
	bomb3.endColor = Color(255, 102, 0); // Orange
	bomb3.tweenTime = 250;
	bomb3.ease = EASE_QUAD_IN_OUT;
	bomb3.yoyo = true;
	bomb3.repeats = 2;

	Animation bomb4 = Animation();
	bomb4.startColor = Color(255, 102, 0);
	bomb4.endColor = Color(255, 255, 255); // White
	bomb4.tweenTime = 500;
	bomb4.ease = EASE_QUAD_IN_OUT;
	bomb4.yoyo = false;
	bomb4.repeats = 0;

	Animation bomb5 = Animation();
	bomb5.startColor = Color(255, 255, 255);
	bomb5.endColor = Color(0, 0, 0);
	bomb5.tweenTime = 700;
	bomb5.ease = EASE_LINEAR;

	_bombAnimSeries[0] = bomb1;
	_bombAnimSeries[1] = bomb2;
	_bombAnimSeries[2] = bomb3;
	_bombAnimSeries[3] = bomb4;
	_bombAnimSeries[4] = bomb5;*/
//}

}