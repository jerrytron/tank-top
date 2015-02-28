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
	_joystickOne = NULL;
	_joystickTwo = NULL;
	_piezoOne = NULL;
	_piezoTwo = NULL;

	initHardware();
}

// Calls each setup interval timer.
void HardwareManager::updateIntervalTimers() {
	_ledSet->updateState();
	_button->updateState();
	_joystickOne->updateState();
	_joystickTwo->updateState();
	_piezoOne->updateState();
	_piezoTwo->updateState();
}

/* Accessors */

LedSet* HardwareManager::ledSet() {
	return _ledSet;
}

MomentaryButton* HardwareManager::button() {
	return _button;
}

Joystick* HardwareManager::joystickOne() {
	return _joystickOne;
}

Joystick* HardwareManager::joystickTwo() {
	return _joystickTwo;
}

Piezo* HardwareManager::piezoOne() {
	return _piezoOne;
}

Piezo* HardwareManager::piezoTwo() {
	return _piezoTwo;
}

/* Private Methods */

void HardwareManager::initHardware() {
	// Setup Led Set
	_ledSet = new LedSet();
	_ledSet->initialize(kLedCount, kLedPin, kLedType, kIntervalLedSetMillis);

	// Setup Button
	_button = new MomentaryButton();
	_button->initialize(kButtonPin, kButtonLedPin, kIntervalButtonMillis);

	// Setup Joystick One
	_joystickOne = new Joystick();
	_joystickOne->initialize(kJoystickOneX, kJoystickOneY, kIntervalJoystickMillis);

	// Setup Joystick Two
	_joystickTwo = new Joystick();
	_joystickTwo->initialize(kJoystickTwoX, kJoystickTwoY, kIntervalJoystickMillis);
	// TODO
	_joystickTwo->active = false;

	// Setup Piezo One
	_piezoOne = new Piezo();
	_piezoOne->initialize(kPiezoOnePin, kIntervalPiezoMillis);

	// Setup Piezo Two
	_piezoTwo = new Piezo();
	_piezoTwo->initialize(kPiezoTwoPin, kIntervalPiezoMillis);
}

}