#include "mwam_momentary_button.h"

namespace mwam
{

const uint16_t kHoldThreshold = 2000;

/* Public Methods */

MomentaryButton::MomentaryButton() {
}

void MomentaryButton::initialize(uint8_t aBtnPin, uint8_t aLedPin, uint32_t aUpdateFreq) {
	_btnPin = aBtnPin;
	_ledPin = aLedPin;
	_updateFreq = aUpdateFreq;
	pinMode(_btnPin, INPUT_PULLUP);
	pinMode(_ledPin, OUTPUT);
	//digitalWrite(_btnPin, HIGH);
	this->active = true;
	reset();
}

void MomentaryButton::updateState() {
	if (this->active && (_timeElapsed >= _updateFreq)) {
		updateButton();
		_timeElapsed = 0;
	}
}

void MomentaryButton::reset() {
	_wasOpen = 1;
	_clicked = false;
	_clickReported = false;
	_held = false;
	_holdReported = false;
	turnLedOn();
}

bool MomentaryButton::pressed() {
	return !_wasOpen;
}

bool MomentaryButton::wasClicked() {
	if (_clicked && !_clickReported) {
		_clickReported = true;
		return _clicked;
	}
	return false;
}

bool MomentaryButton::wasHeld() {
	if (_held && !_holdReported) {
		_holdReported = true;
		return _held;
	}
	return false;
}

void MomentaryButton::turnLedOn() {
	digitalWrite(_ledPin, HIGH);
}

void MomentaryButton::turnLedOff() {
	digitalWrite(_ledPin, LOW);
}

/* Private Methods */

void MomentaryButton::updateButton() {
	uint32_t currentTimeMillis = millis();
	uint8_t isOpen = digitalRead(_btnPin);

	if (isOpen != _wasOpen) {
		if (!isOpen) {
			_closeTimeMillis = currentTimeMillis;
		} else {
			_holdReported = false;
			_clickReported = false;
		}
	}

	_clicked = false;
	_held = false;
	bool overHoldThreshold = (currentTimeMillis - _closeTimeMillis) >= kHoldThreshold;
	if (!isOpen && overHoldThreshold) {
		_held = true;
	} else if (isOpen && !_wasOpen && !overHoldThreshold) {
		_clicked = true;
	}

	_wasOpen = isOpen;
}

}