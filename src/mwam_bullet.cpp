#include "mwam_bullet.h"

namespace mwam
{

/* Public Methods */

Bullet::Bullet() {
}

void Bullet::initialize(uint32_t aUpdateFreq) {
	_updateFreq = aUpdateFreq;
	_timeElapsed = 0;
}

void Bullet::updateState() {
	if (this->active && (_timeElapsed >= _updateFreq)) {
		_timeElapsed = 0;
	}
}


/* Private Methods */

}