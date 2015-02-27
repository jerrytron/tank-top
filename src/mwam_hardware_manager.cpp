#include "mwam_hardware_manager.h"
#include "mwam_manager.h"

namespace mwam
{

/* Public Methods */

HardwareManager::HardwareManager() {}

void HardwareManager::initialize(StateController *aStateController) {
	_stateController = aStateController;

	_ledSet = NULL;
	_rotary = NULL;
	_button = NULL;
	_bubbleDisplay = NULL;
	_printer = NULL;

	setupHardwarePins();
	initHardware();
}

// Calls each setup interval timer.
void HardwareManager::updateIntervalTimers() {
	_ledSet->updateState();
	_rotary->updateState();
	_button->updateState();
	_bubbleDisplay->updateState();
	_printer->updateState();
}

/* Accessors */

LedSet* HardwareManager::ledSet() {
	return _ledSet;
}

Rotary* HardwareManager::rotary() {
	return _rotary;
}

MomentaryButton* HardwareManager::button() {
	return _button;
}

BubbleDisplay* HardwareManager::bubbleDisplay() {
	return _bubbleDisplay;
}

Printer* HardwareManager::printer() {
	return _printer;
}

/* Private Methods */

void HardwareManager::setupHardwarePins() {
	// Pins for RGB LED
	/*pinMode(PIN_RED_LED, OUTPUT);
	pinMode(PIN_GREEN_LED, OUTPUT);
	pinMode(PIN_BLUE_LED, OUTPUT);
	digitalWrite(PIN_RED_LED, HIGH);
	digitalWrite(PIN_GREEN_LED, HIGH);
	digitalWrite(PIN_BLUE_LED, HIGH);*/
}

void HardwareManager::initHardware() {
	// Setup Led Set
	_ledSet = new LedSet();
	_ledSet->initialize(kLedCount, kLedPin, kLedType, kIntervalLedSetMillis);

	// Setup Rotary
	_rotary = new Rotary();
	_rotary->initialize(kRotaryDialingPin, kRotaryPulsingPin, kIntervalRotaryMillis);

	// Setup Button
	_button = new MomentaryButton();
	_button->initialize(kButtonPin, kButtonLedPin, kIntervalButtonMillis);

	// Setup Seven Segment Display
	_bubbleDisplay = new BubbleDisplay();
	_bubbleDisplay->initialize(kBubbleDisplayCount, kIntervalBubbleDisplayMicros);

	// Setup Printer
	_printer = new Printer();
	_printer->initialize();
	_printer->begin(kPrinterHeatingTime);
	_printer->setTimes(7000, 300);
	_printer->setASB(true);
}

}