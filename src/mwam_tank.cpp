#include "mwam_tank.h"

namespace mwam
{

/* Public Methods */

Tank::Tank() {
}

void Tank::initialize(uint32_t aUpdateFreq, uint8_t aStartIndex) {
	_updateFreq = aUpdateFreq;
	_startIndex = aStartIndex;
	_timeElapsed = 0;
}

void Tank::reset() {
	_index = _startIndex;
	_health = kHealthTotal;
	_bulletCount = 0;
	memset(_bullets, 0, kMaxBulletsLive);
}

void Tank::updateState() {
	if (this->active && (_timeElapsed >= _updateFreq)) {
		_timeElapsed = 0;
	}
}

/* Private Methods */

}