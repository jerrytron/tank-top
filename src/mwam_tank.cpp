#include "mwam_tank.h"
#include "mwam_level.h"

namespace mwam
{

/* Public Methods */

Tank::Tank() {
}

void Tank::initialize(TankNumber aTankNum, Level* aLevel, uint8_t aStartIndex, uint32_t aMovementFreq) {
	_tankNumber = aTankNum;
	_level = aLevel;
	_startIndex = aStartIndex;
	//_movementFreq = aMovementFreq;
	reset();
	_timeElapsed = 0;
}

void Tank::reset() {
	_index = _startIndex;
	_lastIndex = _index;
	_direction = DIR_DOWN_LEFT;
	_turretIndex = _index - kLedDiagDownLeft;
	_lastTurretIndex = _turretIndex;
	_health = kHealthTotal;
	_bulletCount = 0;
	memset(_bullets, 0, kMaxBulletsLive);
}

void Tank::updateState(Direction aDirection, uint16_t aMovementFreq) {
	_direction = aDirection;
	if ((_direction == DIR_NONE) && (_direction != DIR_NONE)) {
		_timeElapsed = _movementFreq;
	}
	if (this->active && (_timeElapsed >= aMovementFreq)) {
		_timeElapsed = 0;
		TileType collisionTile = TILE_BACKGROUND;
		_lastIndex = _index;
		_index = _level->updatePosition(_index, _direction, collisionTile);
		DEBUG("Tank Index: %d", _index);
		if (collisionTile == TILE_BULLET) {
			// TODO: DESTROY!!!!
			DEBUG("Tank collided with tile: %d", collisionTile);
		}
		//_turretIndex = getTurretIndex();
		_level->setTankAtIndex(this);
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

TankNumber Tank::getTankNumber() {
	return _tankNumber;
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

uint16_t Tank::getTurretIndex() {
	TileType collisionTile = TILE_BACKGROUND;
	uint16_t index = _level->updatePosition(_index, _direction, collisionTile);
	if (collisionTile == TILE_BOUNDARY) {
		if (_direction == DIR_DOWN_LEFT) {
			_direction = DIR_UP_LEFT;
			index = _index + kLedDiagUpLeft;
		} else if (_direction == DIR_DOWN_RIGHT) {
			_direction = DIR_UP_RIGHT;
			index = _index + kLedDiagUpRight;
		} else if (_direction == DIR_UP_LEFT) {
			_direction = DIR_DOWN_LEFT;
			index = _index - kLedDiagDownLeft;
		} else if (_direction == DIR_UP_RIGHT) {
			_direction = DIR_DOWN_RIGHT;
			index = _index - kLedDiagDownRight;
		}
	}
	return index;
}

/* Private Methods */

void Tank::updateBullets() {
	for (uint8_t i = 0; i < _bulletCount; ++i) {
		_bullets[i].updateState();
	}
}

}