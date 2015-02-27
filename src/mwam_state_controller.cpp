#include "mwam_state_controller.h"
#include "mwam_manager.h"

namespace mwam {

struct GameStateStr_t {
	GameState state;
	const char *stateDesc;
} GameStateDesc[] = {
	{ STATE_NONE, "none" }, // Only before actual state is set.
	{ STATE_ERROR, "error" }, // Problem, report if possible.
	{ STATE_BOOTING, "booting" }, // Only occurs during power cycle.
	{ STATE_INIT, "init" },
	{ STATE_WAITING, "waiting" },
	{ STATE_TUTORIAL, "tutorial" },
	{ STATE_INTRO, "intro" },
	{ STATE_SETUP_WAVE, "setup_wave" },
	{ STATE_PLAY, "play" },
	{ STATE_END_WAVE, "end_wave" },
	{ STATE_GAME_OVER, "game_over" },
	{ STATE_GAME_WON, "game_won" },
	{ STATE_HIGH_SCORE, "high_score" }
};

/* Public Methods */

StateController::StateController() {
}

void StateController::initialize() {
	changeState(STATE_BOOTING);
}

GameState StateController::getState() {
	return _state;
}

const char* StateController::stateString() {
	return GameStateDesc[_state].stateDesc;
}

void StateController::changeState(GameState aState) {
	endState(_state);
	_state = aState;
	initState(_state);
}

void StateController::updateState() {
	loopState(_state);
}

/* Private Methods */

void StateController::initState(GameState aState) {
	LOG("Init State: %s", stateString());

	if (aState == STATE_BOOTING) {
		if (!Manager::getInstance().initialize(this)) {
			changeState(STATE_ERROR);
		} else {
			_dataManager = Manager::getInstance().dataManager;
			_gameManager = Manager::getInstance().gameManager;
			_hardwareManager = Manager::getInstance().hardwareManager;
		}
	} else if (aState == STATE_INIT) {
		_dataManager->logScoreData();
		_gameManager->reset();
		if (Spark.connected()) {
			Spark.syncTime();
		}
	} else if (aState == STATE_WAITING) {
		// Start looping text "Hit Bomb To Play".
		_hardwareManager->bubbleDisplay()->setScrollStyle(SCROLL_LEFT_WRAP, 1, kBubbleDisplayScrollFreq, kMsgWaitingToStartLen);
		_hardwareManager->bubbleDisplay()->setData(kMsgWaitingToStart, kMsgWaitingToStartLen);
		_hardwareManager->bubbleDisplay()->startScrolling();

		// Start looping button light blink.

	} else if (aState == STATE_TUTORIAL) {

	} else if (aState == STATE_INTRO) {
		_seed = millis();
		randomSeed(_seed);
	} else if (aState == STATE_SETUP_WAVE) {
		_hardwareManager->bubbleDisplay()->resetDisplays();
		_hardwareManager->bubbleDisplay()->setData("WAVE ", 5);
		_hardwareManager->bubbleDisplay()->setInteger(_gameManager->currentWave());
		_hardwareManager->bubbleDisplay()->setCursorPos(8);
		_hardwareManager->bubbleDisplay()->setData(" BEGIN  ", 8);
		_hardwareManager->bubbleDisplay()->setScrollStyle(SCROLL_LEFT, 8, 750, 16);
		_hardwareManager->bubbleDisplay()->startScrolling();
		DEBUG("Wave %d", _gameManager->currentWave());
	} else if (aState == STATE_PLAY) {
		_gameManager->displayScore();
	} else if (aState == STATE_END_WAVE) {

	} else if (aState == STATE_GAME_OVER) {

	} else if (aState == STATE_GAME_WON) {

	} else if (aState == STATE_HIGH_SCORE) {

	} else if (aState == STATE_ERROR) {
		ERROR("*** ERROR STATE ***");
	}
}

void StateController::loopState(GameState aState) {
	// Handle special case where user sets up WiFi credentials
	// while firmware is running, so gameplay isn't disrupted.
	if (!_dataManager->metadata()->flags.offline &&
	    !_dataManager->hasCredentials &&
	    WiFi.hasCredentials() &&
	    (aState != STATE_BOOTING)) {
		_dataManager->hasCredentials = true;
		if (!Spark.connected()) {
			Spark.connect();
		}
	}

	if (aState == STATE_BOOTING) {
		// Have ! to not connect by default, remove it to connect by default.
		if (_hardwareManager->button()->pressed()) {
			_dataManager->metadata()->flags.offline = !_dataManager->metadata()->flags.offline;
		}
		if (!_dataManager->metadata()->flags.offline) {
			WiFi.on();
			if (WiFi.hasCredentials()) {
				_dataManager->hasCredentials = true;
				if (!Spark.connected()) {
					Spark.connect();
				}
			}
		}
		changeState(STATE_INIT);
	} else if (aState == STATE_INIT) {
		changeState(STATE_WAITING);
	} else if (aState == STATE_WAITING) {
		// Player must hit the bomb button to start a new game.
		if (_hardwareManager->button()->wasClicked()) {
			changeState(STATE_INTRO);
		} else if (_hardwareManager->rotary()->newDialEvent()) {
			uint8_t dialValue = _hardwareManager->rotary()->lastDialed();
			if (dialValue == kRadarDialValue) {
				changeState(STATE_TUTORIAL);
			}
		}
	} else if (aState == STATE_TUTORIAL) {
		_gameManager->updateTutorial();
	} else if (aState == STATE_INTRO) {
		/*if (_hardwareManager->button()->wasClicked()) {
			_gameManager->nextIntroStep();
		}*/
		_gameManager->updateIntro();
	} else if (aState == STATE_SETUP_WAVE) {
		if (!_hardwareManager->bubbleDisplay()->scrolling()) {
			_hardwareManager->bubbleDisplay()->resetDisplays();
			_hardwareManager->button()->resetButton();
			_hardwareManager->rotary()->resetRotary();
			changeState(STATE_PLAY);
		}
	} else if (aState == STATE_PLAY) {
		if (_hardwareManager->rotary()->newDialEvent()) {
			uint8_t dialValue = _hardwareManager->rotary()->lastDialed();
			if (dialValue == kRadarDialValue) {
				_gameManager->playRadarAnim();
			} else {
				_gameManager->fireWeapon(dialValue);
			}
		}
		if (_hardwareManager->button()->wasClicked()) {
			_gameManager->detonateBomb();
		}
		_gameManager->updatePlay();
	} else if (aState == STATE_END_WAVE) {
		if (_hardwareManager->ledSet()->allAnimsDone()) {
			_gameManager->endWave();
		}
	} else if (aState == STATE_GAME_OVER) {
		_gameManager->updateGameOver(false);
	} else if (aState == STATE_GAME_WON) {
		_gameManager->updateGameOver(true);
	} else if (aState == STATE_HIGH_SCORE) {
		_gameManager->updateHighScore();
	} else if (aState == STATE_ERROR) {

	}
}


void StateController::endState(GameState aState) {
	LOG("End State: %s", stateString());

	/*if (aState == STATE_ERROR) {

	} else if (aState == STATE_BOOTING) {

	} else if (aState == STATE_INIT) {

	} else if (aState == STATE_WAITING) {

	} else if (aState == STATE_TUTORIAL) {

	} else if (aState == STATE_INTRO) {

	} else if (aState == STATE_SETUP_WAVE) {

	} else if (aState == STATE_PLAY) {

	} else if (aState == STATE_END_WAVE) {

	} else if (aState == STATE_GAME_OVER) {

	} else if (aState == STATE_GAME_WON) {

	} else if (aState == STATE_HIGH_SCORE) {

	}*/
}

}