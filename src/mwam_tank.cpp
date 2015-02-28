#include "mwam_tank.h"
#include "mwam_level.h"

namespace mwam
{

/* Public Methods */

Tank::Tank() {
}

void Tank::initialize(Level* aLevel, uint8_t aStartIndex, uint32_t aMovementFreq) {
	_level = aLevel;
	_startIndex = aStartIndex;
	_movementFreq = aMovementFreq;
	_timeElapsed = 0;
}

void Tank::reset() {
	_index = _startIndex;
	_lastIndex = _index;
	_health = kHealthTotal;
	_bulletCount = 0;
	memset(_bullets, 0, kMaxBulletsLive);
}

void Tank::updateState(Direction aDirection, uint16_t aDelayReduction) {
	_direction = aDirection;
	if ((_direction == DIR_NONE) && (_direction != DIR_NONE)) {
		_timeElapsed = _movementFreq;
	}
	if (this->active && (_timeElapsed >= (_movementFreq - aDelayReduction))) {
		_timeElapsed = 0;
		TileType collisionTile = TILE_EMPTY;
		_lastIndex = _index;
		_index = _level->updatePosition(_index, _direction, collisionTile);

		if (collisionTile == TILE_BULLET) {
			// TODO: DESTROY!!!!
			DEBUG("Tank collided with tile: %d", collisionTile);
		}

	}
	updateBullets();
}

bool Tank::fireBullet() {
	if (_bulletCount < kMaxBulletsLive) {
		_bullets[_bulletCount] = Bullet();
		_bullets[_bulletCount].initialize(_index, _direction, kBulletMovementDelay);
		_bulletCount++;
		return true;
	}
	return false;
}

uint16_t Tank::getIndex() {
	return _index;
}

void Tank::setIndex(uint16_t aIndex) {
	_index = aIndex;
}

uint16_t Tank::getLastIndex() {
	return _lastIndex;
}

uint8_t Tank::getBulletCount() {
	return _bulletCount;
}

Bullet* Tank::getBulletAtIndex(uint8_t aIndex) {
	return &_bullets[aIndex];
}

/* Private Methods */

void Tank::updateBullets() {
	for (uint8_t i = 0; i < _bulletCount; ++i) {
		_bullets[i].updateState();
	}
}

}