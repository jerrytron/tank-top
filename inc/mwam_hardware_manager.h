#ifndef MWAM_HARDWARE_MANAGER_H
#define MWAM_HARDWARE_MANAGER_H

#include "mwam_constants.h"
#include "mwam_led_set.h"
#include "mwam_rotary.h"
#include "mwam_momentary_button.h"
#include "mwam_bubble_display.h"
#include "mwam_printer.h"
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
		//SparkButton* sparkButton();
		LedSet* ledSet();
		Rotary* rotary();
		MomentaryButton* button();
		BubbleDisplay* bubbleDisplay();
		Printer* printer();

		// Call once per loop, check intervals.
		void updateIntervalTimers();

		/* Public Variables */

	private:
		/* Private Methods */

		// Assign the Spark Core pins.
		void setupHardwarePins();
		// Check start byte and load firmware version.
		bool loadFirmwareVersion();
		// Load flags, values, and basic story info.
		bool loadMetadata();
		// Setup hardware communication objects.
		void initHardware();

		/* Private Variables */
		StateController* _stateController;
		//SparkButton* _sparkButton;
		LedSet* _ledSet;
		Rotary* _rotary;
		MomentaryButton* _button;
		BubbleDisplay* _bubbleDisplay;
		Printer* _printer;
};

}

#endif