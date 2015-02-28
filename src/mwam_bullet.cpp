#include "mwam_bullet.h"

namespace mwam
{

/* Public Methods */

Bullet::Bullet() {
}

void Bullet::initialize(Level* aLevel, uint32_t aMovementDelay) {
	_level = aLevel;
	_movementDelay = aMovementDelay;
	_timeElapsed = 0;
	reset(0, DIR_NONE);
}

void Bullet::reset(uint16_t aIndex, Direction aDir) {
	_index = aIndex;
	_lastIndex = aIndex;
	_direction = aDir;
	_overlapTile = _level->getTileAtIndex(aIndex);
	_lastOverlapTile = TILE_BACKGROUND;
	_bouncesLeft = kMaxBulletBounces;
	this->collided = false;
	this->endOfLife = false;
}

void Bullet::updateState() {
	if (!this->endOfLife && (_timeElapsed >= _movementDelay)) {
		_timeElapsed = 0;
		_lastIndex = _index;
		_lastOverlapTile = _overlapTile;
		_index = _level->getNewPosition(_index, _direction, _overlapTile);
		if ((_overlapTile == TILE_BOUNDARY) || (_overlapTile == TILE_WALL)) {
			_overlapTile = _lastOverlapTile;
			_index = _lastIndex;
			DEBUG("Bullet hit barrier!");
			if (!bounceBullet()) {
				this->endOfLife = true;
			}
		} else if ((_overlapTile == TILE_TANK_ONE) || (_overlapTile == TILE_TANK_TWO)) {
			DEBUG("Hit tank!");
			this->collided = true;
			this->endOfLife = true;
		} else {
			_level->setTileAtIndex(_lastOverlapTile, _lastIndex);
			_level->setTileAtIndex(TILE_BULLET, _index);
		}
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

TileType Bullet::getOverlapTile() {
	return _overlapTile;
}

TileType Bullet::getLastOverlapTyle() {
	return _lastOverlapTile;
}

/* Private Methods */

bool Bullet::bounceBullet() {
	if (_bouncesLeft > 0) {
		switch(_direction) {
			case DIR_UP_LEFT: {
				_direction = DIR_DOWN_LEFT;
				break;
			}
			case DIR_LEFT: {
				_direction = DIR_RIGHT;
				break;
			}
			case DIR_DOWN_LEFT: {
				_direction = DIR_UP_LEFT;
				break;
			}
			case DIR_UP: {
				_direction = DIR_DOWN;
				break;
			}
			case DIR_UP_RIGHT: {
				_direction = DIR_DOWN_RIGHT;
				break;
			}
			case DIR_RIGHT: {
				_direction = DIR_LEFT;
				break;
			}
			case DIR_DOWN_RIGHT: {
				_direction = DIR_UP_RIGHT;
				break;
			}
			case DIR_DOWN: {
				_direction = DIR_UP;
				break;
			}
		}

		TileType collisionTile = TILE_BACKGROUND;
		_level->getNewPosition(_index, _direction, collisionTile);
		bool collision = false;
		if ((collisionTile == TILE_BOUNDARY) || (collisionTile == TILE_WALL)) {
			collision = true;
		}
		if (collision) {
			switch(_direction) {
				case DIR_UP_LEFT: {
					_direction = DIR_DOWN_RIGHT;
					break;
				}
				case DIR_LEFT: {
					_direction = DIR_LEFT;
					break;
				}
				case DIR_DOWN_LEFT: {
					_direction = DIR_UP_RIGHT;
					break;
				}
				case DIR_UP: {
					_direction = DIR_UP;
					break;
				}
				case DIR_UP_RIGHT: {
					_direction = DIR_DOWN_LEFT;
					break;
				}
				case DIR_RIGHT: {
					_direction = DIR_RIGHT;
					break;
				}
				case DIR_DOWN_RIGHT: {
					_direction = DIR_UP_LEFT;
					break;
				}
				case DIR_DOWN: {
					_direction = DIR_DOWN;
					break;
				}
			}
		}

		_bouncesLeft--;
		_timeElapsed = _movementDelay;
		return true;
	}
	return false;
}

}