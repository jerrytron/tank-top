#include "mwam_piezo.h"

namespace mwam
{

/* Public Methods */

Piezo::Piezo() {
}

void Piezo::initialize(uint32_t aUpdateFreq) {
	_updateFreq = aUpdateFreq;
	_timeElapsed = 0;
}

void Piezo::updateState() {
	if (this->active && (_timeElapsed >= _updateFreq)) {
		_timeElapsed = 0;
	}
}


/* Private Methods */

}