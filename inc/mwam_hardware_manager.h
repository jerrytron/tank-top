#ifndef MWAM_HARDWARE_MANAGER_H
#define MWAM_HARDWARE_MANAGER_H

#include "mwam_constants.h"
#include "mwam_led_set.h"
#include "mwam_momentary_button.h"
#include "mwam_nunchuk.h"
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
#ifdef CYLINDRUS
		Nunchuk* joystickOne();
#else
		Joystick* joystickOne();
#endif
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
#ifdef CYLINDRUS
		Nunchuk* _joystickOne;
#else
		Joystick* _joystickOne;
#endif
		Joystick* _joystickTwo;
		Piezo* _piezoOne;
		Piezo* _piezoTwo;
};

}

#endif