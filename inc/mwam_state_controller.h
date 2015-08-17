#ifndef MWAM_STATE_CONTROLLER_H
#define MWAM_STATE_CONTROLLER_H

#include "mwam_constants.h"

#include "tone_tests.h"

namespace mwam
{

class DataManager;
class GameManager;
class HardwareManager;

class StateController {

public:
/* Public Methods */
	StateController();

	void initialize();
	GameState getState();
	const char* stateString();
	void changeState(GameState aState);
	void updateState();

/* Public Variables */

protected:

private:
/* Private Methods */
	void initState(GameState aState);
	void loopState(GameState aState);
	void endState(GameState aState);

/* Private Variables */
	DataManager* _dataManager;
	GameManager* _gameManager;
	HardwareManager* _hardwareManager;
	GameState _state;
	uint32_t _seed;

	ToneTests* _toneTests;
};

}

#endif