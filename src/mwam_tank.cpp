#include "mwam_tank.h"

namespace mwam
{

/* Public Methods */

Tank::Tank() {
}

void Tank::initialize(uint32_t aUpdateFreq) {
	_updateFreq = aUpdateFreq;
	_timeElapsed = 0;
}

void Tank::updateState() {
	if (this->active && (_timeElapsed >= _updateFreq)) {
		_timeElapsed = 0;
	}
}

/* Private Methods */

}