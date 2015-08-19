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
	{ STATE_SELECT, "select" },
	{ STATE_PLAY, "play" },
	{ STATE_GAME_OVER, "game_over" },
	{ STATE_GAME_WON, "game_won" }
};

/* Public Methods */

StateController::StateController() {
}

void StateController::initialize() {
	_state = STATE_BOOTING;
	initState(_state);

	//_toneTests = new ToneTests();
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
		_gameManager->reset();
		if (Spark.connected()) {
			Spark.syncTime();
		}
	} else if (aState == STATE_WAITING) {
		_gameManager->initWaiting();
		//_hardwareManager->ledSet()->updateLeds(_gameManager->getLevel());
	} else if (aState == STATE_TUTORIAL) {

	} else if (aState == STATE_SELECT) {
		_seed = millis();
		randomSeed(_seed);
		_gameManager->initSelect();
	} else if (aState == STATE_PLAY) {
		_gameManager->initPlay();
	} else if (aState == STATE_GAME_OVER) {

	} else if (aState == STATE_GAME_WON) {

	} else if (aState == STATE_ERROR) {
		ERROR("*** ERROR STATE ***");
	}
}

void StateController::loopState(GameState aState) {
	// Handle special case where user sets up WiFi credentials
	// while firmware is running, so gameplay isn't disrupted.
	/*if (!_dataManager->metadata()->flags.offline &&
	    !_dataManager->hasCredentials &&
	    WiFi.hasCredentials() &&
	    (aState != STATE_BOOTING)) {
		_dataManager->hasCredentials = true;
		if (!Spark.connected()) {
			Spark.connect();
		}
	}*/

	if (aState == STATE_BOOTING) {
		// Have ! to not connect by default, remove it to connect by default.
		/*if (_hardwareManager->button()->pressed()) {
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
		}*/
		changeState(STATE_INIT);
	} else if (aState == STATE_INIT) {
		changeState(STATE_WAITING);
	} else if (aState == STATE_WAITING) {
		_gameManager->updateWaiting();

		if (_hardwareManager->joystickOne()->clickUp() ||
		    _hardwareManager->joystickTwo()->clickUp()) {
			changeState(STATE_SELECT);
		}
		//if (_hardwareManager->button()->wasClicked()) {
	} else if (aState == STATE_TUTORIAL) {

	} else if (aState == STATE_SELECT) {
		_gameManager->updateSelect();
	} else if (aState == STATE_PLAY) {
		_gameManager->updatePlay();
	} else if (aState == STATE_GAME_OVER) {

	} else if (aState == STATE_GAME_WON) {

	} else if (aState == STATE_ERROR) {

	}
}


void StateController::endState(GameState aState) {
	LOG("End State: %s", stateString());

	if (aState == STATE_ERROR) {

	} else if (aState == STATE_BOOTING) {

	} else if (aState == STATE_INIT) {

	} else if (aState == STATE_WAITING) {
		_gameManager->endWaiting();
	} else if (aState == STATE_TUTORIAL) {

	} else if (aState == STATE_SELECT) {
		_gameManager->endSelect();
	} else if (aState == STATE_PLAY) {
		_gameManager->endPlay();
	} else if (aState == STATE_GAME_OVER) {

	} else if (aState == STATE_GAME_WON) {

	}
}

}