#ifndef MWAM_DATA_MANAGER_H
#define MWAM_DATA_MANAGER_H

#include "mwam_constants.h"
#include "mwam_state_controller.h"
//#include "flashee-eeprom.h"
#include "ElapsedTime.h"

namespace mwam
{

class StateController;

class DataManager
{
	public:
		/* Public Methods */
		DataManager();

		bool initialize(StateController *aStateController);
		void handleSerialData();

		// Accessor Functions
		StateController *stateController();

		/* Public Variables */
		// Whether or not the state machine should execute it's update loop.
		bool runState;
		bool hasCredentials;

	private:
		/* Private Methods */

		/* Private Variables */
		StateController* _stateController;

		ElapsedMillis _serialElapsed;
		ElapsedMillis _serialTimeout;
};

}

#endif