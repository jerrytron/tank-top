#ifndef MWAM_HARDWARE_MANAGER_H
#define MWAM_HARDWARE_MANAGER_H

#include "mwam_constants.h"
#include "mwam_led_set.h"
#include "mwam_momentary_button.h"
#include "mwam_joystick.h"
#include "mwam_piezo.h"
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
		void resetHardware();
		// Call once per loop, check intervals.
		void updateIntervalTimers();

		// Accessors
		LedSet* ledSet();
		MomentaryButton* button();
		Joystick* joystickOne();
		Joystick* joystickTwo();
		Piezo* piezoOne();
		Piezo* piezoTwo();

		/* Public Variables */

	private:
		/* Private Methods */

		// Setup hardware communication objects.
		void initHardware();

		/* Private Variables */
		StateController* _stateController;
		LedSet* _ledSet;
		MomentaryButton* _button;
		Joystick* _joystickOne;
		Joystick* _joystickTwo;
		Piezo* _piezoOne;
		Piezo* _piezoTwo;
};

}

#endif