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

void HardwareManager::resetHardware() {
	//_ledSet->resetLeds();
	_button->reset();
	_joystickOne->reset();
	_joystickTwo->reset();
	//_piezoOne->reset();
	//_piezoTwo->reset();
}

// Calls each setup interval timer.
void HardwareManager::updateIntervalTimers() {
	_ledSet->updateState(Manager::getInstance().gameManager->getLevel());
	_button->updateState();
	_joystickOne->updateState();
	_joystickTwo->updateState();
	//_piezoOne->updateState();
	//_piezoTwo->updateState();
}

/* Accessors */

LedSet* HardwareManager::ledSet() {
	return _ledSet;
}

MomentaryButton* HardwareManager::button() {
	return _button;
}

#ifdef CYLINDRUS
Nunchuk* HardwareManager::joystickOne() {
	return _joystickOne;
}
#else
Joystick* HardwareManager::joystickOne() {
	return _joystickOne;
}
#endif

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
	_button->initialize(kButtonPin, kIntervalButtonMillis);

	// Setup Joystick One
#ifdef CYLINDRUS
	_joystickOne = new Nunchuk();
	_joystickOne->initialize(DIR_SET_FOUR_DIAG, kIntervalNunchukMillis);
#else
	_joystickOne = new Joystick();
	_joystickOne->initialize(kJoystickOneXPin, kJoystickOneYPin, kJoystickOneBtnPin, DIR_SET_FOUR_DIAG, kIntervalJoystickMillis);
#endif
	//_joystickOne->active = false;

	// Setup Joystick Two
	_joystickTwo = new Joystick();
	_joystickTwo->initialize(kJoystickTwoXPin, kJoystickTwoYPin, kJoystickTwoBtnPin, DIR_SET_FOUR_DIAG, kIntervalJoystickMillis);
	//_joystickTwo->active = false;

	// Setup Piezo One
	//_piezoOne = new Piezo();
	//_piezoOne->initialize(kPiezoOnePin, kIntervalPiezoMillis);

	// Setup Piezo Two
	//_piezoTwo = new Piezo();
	//_piezoTwo->initialize(kPiezoTwoPin, kIntervalPiezoMillis);
}

}