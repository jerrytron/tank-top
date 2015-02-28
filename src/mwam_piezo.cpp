#include "mwam_piezo.h"

namespace mwam
{

/* Public Methods */

Piezo::Piezo() {
}

void Piezo::initialize(uint8_t aPin, uint32_t aUpdateFreq) {
	_pin = aPin;
	_updateFreq = aUpdateFreq;
	_timeElapsed = 0;
	this->active = true;
}

void Piezo::updateState() {
	if (this->active && (_timeElapsed >= _updateFreq)) {
		_timeElapsed = 0;
	}
}


/* Private Methods */

}