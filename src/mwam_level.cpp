#include "mwam_level.h"

namespace mwam
{

/* Public Methods */

Level::Level() {
}

void Level::initialize(uint32_t aUpdateFreq) {
	_updateFreq = aUpdateFreq;
	_timeElapsed = 0;
}

void Level::updateState() {
	if (this->active && (_timeElapsed >= _updateFreq)) {
		_timeElapsed = 0;
	}
}


/* Private Methods */

}