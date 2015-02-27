#ifndef MWAM_HARDWARE_MANAGER_H
#define MWAM_HARDWARE_MANAGER_H

#include "mwam_constants.h"
#include "mwam_led_set.h"
#include "mwam_momentary_button.h"
#include "ElapsedTime.h"

namespace mwam
{

class StateController;

class HardwareManager
{
	public:
		/* Public Methods */
		HardwareManager();

		void initialize(StateController* aStatecontroller);

		// Accessors
		LedSet* ledSet();
		MomentaryButton* button();

		// Call once per loop, check intervals.
		void updateIntervalTimers();

		/* Public Variables */

	private:
		/* Private Methods */

		// Assign the Spark Core pins.
		void setupHardwarePins();
		// Setup hardware communication objects.
		void initHardware();

		/* Private Variables */
		StateController* _stateController;
		LedSet* _ledSet;
		MomentaryButton* _button;
};

}

#endif