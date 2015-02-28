#include "mwam_tank.h"

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
	_lastIndex = 0;
	_lastCollision = TILE_BACKGROUND;
	_direction = DIR_DOWN_LEFT;
	_turretIndex = _index - kLedDiagDownLeft;
	_lastTurretIndex = 0;
	_turretOverlap = TILE_BACKGROUND;
	_lastTurretOverlap = TILE_BACKGROUND;
	_health = kHealthTotal;
	_bulletCount = 0;
	memset(_bullets, 0, kMaxBulletsLive);
}

void Tank::updateState(Direction aDirection, uint16_t aMovementFreq) {
	if (this->active && (_timeElapsed >= aMovementFreq)) {
		_timeElapsed = 0;
		if ((_direction == DIR_NONE) && (aDirection != DIR_NONE)) {
			_timeElapsed = _movementFreq;
		}
		_direction = aDirection;
		//TileType collisionTile = TILE_BACKGROUND;
		_lastIndex = _index;
		_index = _level->getNewPosition(_index, _direction, _lastCollision);
		if (_lastIndex != _index) {
			DEBUG("Tank Index: %d", _index);
			_lastTurretIndex = _turretIndex;
			_turretIndex = findTurretIndex();
			_level->setTankAtIndex(this);
		}
		if (_lastCollision == TILE_BULLET) {
			// TODO: DESTROY!!!!
			DEBUG("Ran into bullet!");
		}
		if (_lastCollision != TILE_BACKGROUND) {
			DEBUG("Tank collided with tile: %d", _lastCollision);
		}

		updateBullets();
	}
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
	return _turretIndex;
}

uint16_t Tank::getLastTurretIndex() {
	return _lastTurretIndex;
}

TileType Tank::getTurretOverlapTile() {
	return _turretOverlap;
}

TileType Tank::getLastTurretOverlapTile() {
	return _lastTurretOverlap;
}

uint16_t Tank::findTurretIndex() {
	TileType collisionTile = TILE_BACKGROUND;
	uint16_t index = _level->getNewPosition(_index, _direction, collisionTile);
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
	_lastTurretOverlap = _turretOverlap;
	if (_turretOverlap == TILE_TANK_ONE) {
		DEBUG("Last turret layer is tank, bad");
	}
	_turretOverlap = _level->getTileAtIndex(index);
	return index;
}

/* Private Methods */

void Tank::updateBullets() {
	for (uint8_t i = 0; i < _bulletCount; ++i) {
		_bullets[i].updateState();
	}
}

}