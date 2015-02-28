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
	_overlap = TILE_BACKGROUND;
	_lastOverlap = TILE_BACKGROUND;
	_direction = DIR_DOWN_LEFT;
	_turretIndex = _index - kLedDiagDownLeft;
	_lastTurretIndex = 0;
	_turretOverlap = TILE_BACKGROUND;
	_lastTurretOverlap = TILE_BACKGROUND;
	_health = kHealthTotal;
	_bulletCount = 0;
	for (uint8_t i = 0; i < kMaxBulletsLive; ++i) {
		_bullets[i].initialize(_level, kBulletMovementDelay);
		_bullets[i].endOfLife = true;
	}
}

void Tank::updateState(Direction aDirection, uint16_t aMovementFreq) {
	if (this->active && (_timeElapsed >= aMovementFreq)) {
		_timeElapsed = 0;

		if (_health == 0) {
			DEBUG("Tank destroyed!");
			return;
		}

		if ((_direction == DIR_NONE) && (aDirection != DIR_NONE)) {
			_timeElapsed = _movementFreq;
		}
		_direction = aDirection;
		//TileType collisionTile = TILE_BACKGROUND;
		_lastIndex = _index;
		_lastOverlap = _overlap;
		_index = _level->getNewPosition(_index, _direction, _overlap);
		DEBUG("Tank Overlap: %d", _overlap);
		if ((_overlap == TILE_BACKGROUND) || (_overlap == TILE_BULLET) ||
		    (_overlap == TILE_TURRET_ONE) || (_overlap == TILE_TURRET_TWO)) {
			DEBUG("Tank Index: %d", _index);
			_lastTurretIndex = _turretIndex;
			_turretIndex = findTurretIndex();
			_level->setTankAtIndex(this);
		} else if ((_overlap == TILE_WALL) || (_overlap == TILE_TANK_ONE) ||
		    (_overlap == TILE_TANK_TWO)) {
			_index = _lastIndex;
		}
		if (_overlap == TILE_BULLET) {
			// TODO: DESTROY!!!!
			DEBUG("Ran into bullet!");
			_health--;
		}

	}
}

void Tank::updateBullets() {
	for (uint8_t i = 0; i < _bulletCount; ++i) {
		_bullets[i].updateState();
		if (_bullets[i].collided) {
			DEBUG("Bullet hit tank!");
			_health--;
		}
		if (_bullets[i].endOfLife) {
			_level->setTileAtIndex(_bullets[i].getOverlapTile(), _bullets[i].getIndex());
		}
	}
}

bool Tank::fireBullet() {
	if (_bulletCount < kMaxBulletsLive) {
		uint8_t i;
		for (i = 0; i < kMaxBulletsLive; ++i) {
			if (_bullets[i].endOfLife) {
				//delete _bullets[i];
				_bullets[i].reset(_turretIndex, _direction);
				break;
			}
		}
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

TileType Tank::getLastOverlapTile() {
	return _lastOverlap;
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
	TileType collisionTile = TILE_BOUNDARY;
	//uint16_t index = _level->getNewPosition(_index, _direction, collisionTile);
	uint16_t index = _index;
	Direction tryDir = _direction;
	bool searching = true;
	while (searching) {
		index = _level->getNewPosition(_index, tryDir, collisionTile);
		if ((collisionTile == TILE_BOUNDARY) || (collisionTile == TILE_WALL)) {
			if (tryDir == DIR_DOWN_LEFT) {
				tryDir = DIR_UP_LEFT;
			} else if (tryDir == DIR_UP_LEFT) {
				tryDir = DIR_UP_RIGHT;
			} else if (tryDir == DIR_UP_RIGHT) {
				tryDir = DIR_DOWN_RIGHT;
			} else if (tryDir == DIR_DOWN_RIGHT) {
				tryDir = DIR_DOWN_LEFT;
			}
		} else {
			searching = false;
		}
	}
	_direction = tryDir;

	if ((_turretOverlap != TILE_TURRET_ONE) && (_turretOverlap != TILE_TURRET_TWO) &&
	    (_turretOverlap != TILE_TANK_ONE) && (_turretOverlap != TILE_TANK_TWO)) {
		_lastTurretOverlap = _turretOverlap;
	}
	_turretOverlap = _level->getTileAtIndex(index);
	return index;
}

/* Private Methods */

}