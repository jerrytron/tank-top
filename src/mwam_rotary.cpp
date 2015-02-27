#include "mwam_rotary.h"

namespace mwam
{

/* Public Methods */

Rotary::Rotary() {
}

void Rotary::initialize(uint8_t aPinDialing, uint8_t aPinPulsing, uint32_t aUpdateFreq) {
	_updateFreq = aUpdateFreq;
	_pinDialing = aPinDialing;
	_pinPulsing = aPinPulsing;
	pinMode(_pinDialing, INPUT);
	pinMode(_pinPulsing, INPUT);

	this->active = true;
	resetRotary();
}

void Rotary::updateState() {
	if (this->active && (_timeElapsed >= _updateFreq)) {
		updateRotary();
		_timeElapsed = 0;
	}
}

void Rotary::resetRotary() {
	_dialEvent = false;
	_dialing = false;
	_pulseCount = 0;
	_lastDialed = -1;
	_lastNotDialingVal = 1;
	_lastPulsingVal = 0;
}

bool Rotary::newDialEvent() {
	return _dialEvent;
}

bool Rotary::dialing() {
	return _dialing;
}

uint8_t Rotary::pulseCount() {
	return _pulseCount;
}

int8_t Rotary::lastDialed() {
	_dialEvent = false;
	return _lastDialed;
}

/* Private Methods */

void Rotary::updateRotary() {
	// HIGH when open, not dialing. LOW when closed and dialing.
	uint8_t notDialingVal = digitalRead(_pinDialing);
	// HIGH when open, during a pulse. LOW when closed, not pulsing.
	uint8_t pulsingVal = digitalRead(_pinPulsing);

	// Handle the start and stop states of dialing.
	if (notDialingVal != _lastNotDialingVal) {
		if (!_dialing) { // Just started dialing.
			//Serial.println("Dial Start");
			_dialing = true;
		} else { // Just finished dialing.
			_lastDialed = _pulseCount % 10;
			_pulseCount = 0;
			_dialing = false;
			_dialEvent = true;
			//Serial.print("Dialed: ");
			//Serial.println(_lastDialed);
		}
	}

	// Handle pulse counting.
	if (_dialing) {
		if (pulsingVal != _lastPulsingVal) {
			if (pulsingVal) {
				//Serial.print("Pulse: ");
				//Serial.println(_pulseCount);
				_pulseCount++;
			}
		}
	}

	_lastNotDialingVal = notDialingVal;
	_lastPulsingVal = pulsingVal;
}

}