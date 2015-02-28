#include "mwam_bullet.h"

namespace mwam
{

/* Public Methods */

Bullet::Bullet() {
}

void Bullet::initialize(uint16_t aIndex, Direction aDir, uint32_t aMovementDelay) {
	_movementDelay = aMovementDelay;
	_timeElapsed = 0;
	_index = aIndex;
	_lastIndex = aIndex;
	_direction = aDir;
}

void Bullet::updateState() {
	if (this->active && (_timeElapsed >= _movementDelay)) {
		_timeElapsed = 0;

	}
}

uint16_t Bullet::getIndex() {
	return _index;
}

void Bullet::setIndex(uint16_t aIndex) {
	_index = aIndex;
}

uint16_t Bullet::getLastIndex() {
	return _lastIndex;
}

/* Private Methods */

}