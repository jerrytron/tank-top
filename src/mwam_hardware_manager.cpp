#include "mwam_hardware_manager.h"
#include "mwam_manager.h"

namespace mwam
{

/* Public Methods */

HardwareManager::HardwareManager() {}

void HardwareManager::initialize(StateController *aStateController) {
	_stateController = aStateController;

	_ledSet = NULL;
	_button = NULL;

	setupHardwarePins();
	initHardware();
}

// Calls each setup interval timer.
void HardwareManager::updateIntervalTimers() {
	_ledSet->updateState();
	_button->updateState();
}

/* Accessors */

LedSet* HardwareManager::ledSet() {
	return _ledSet;
}

MomentaryButton* HardwareManager::button() {
	return _button;
}

/* Private Methods */

void HardwareManager::setupHardwarePins() {
	// Pins for RGB LED
}

void HardwareManager::initHardware() {
	// Setup Led Set
	_ledSet = new LedSet();
	_ledSet->initialize(kLedCount, kLedPin, kLedType, kIntervalLedSetMillis);

	// Setup Button
	_button = new MomentaryButton();
	_button->initialize(kButtonPin, kButtonLedPin, kIntervalButtonMillis);
}

}