#include "mwam_bullet.h"
#include "mwam_manager.h"

namespace mwam
{

/* Public Methods */

Bullet::Bullet() {
}

void Bullet::initialize(uint32_t aMovementDelay) {
	_gameManager = Manager::getInstance().gameManager;
	_movementDelay = aMovementDelay;
	_timeElapsed = 0;

	_state = BULLET_INIT;
	initState(BULLET_INIT);
}

void Bullet::reset(uint16_t aIndex, Direction aDir) {
	_index = aIndex;
	_lastIndex = aIndex;
	_direction = aDir;
	_bouncesLeft = kMaxBulletBounces;
}

BulletState Bullet::getState() {
	return _state;
}

void Bullet::changeState(BulletState aState) {
	endState(_state);
	_state = aState;
	initState(_state);
}

void Bullet::updateState() {
	loopState(_state);
}

/*void Bullet::updateState() {
	if (_timeElapsed >= _movementDelay) {
		_timeElapsed = 0;
		_lastIndex = _index;
		_lastOverlapTile = _overlapTile;
		_index = _gameManager->getLevel()->getNewPosition(_index, _direction, _overlapTile);
		if (_overlapTile == TILE_BULLET) {
			_overlapTile = _lastOverlapTile;
			if (_lastOverlapTile == TILE_BULLET) {
				DEBUG("Old overlap is bullet");
			}
		} else if ((_overlapTile == TILE_BOUNDARY) || (_overlapTile == TILE_WALL)) {
			_overlapTile = _lastOverlapTile;
			_index = _lastIndex;
			if (!bounceBullet()) {
				//this->endOfLife = true;
			}
		} else if ((_overlapTile == TILE_TANK_ONE) || (_overlapTile == TILE_TANK_TWO)) {
			DEBUG("Hit tank!");
			//this->collided = true;
			//this->endOfLife = true;
		} else {
			_gameManager->getLevel()->setTileAtIndex(_lastOverlapTile, _lastIndex);
			_gameManager->getLevel()->setTileAtIndex(TILE_BULLET, _index);
		}
	}
}*/

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

void Bullet::initState(BulletState aState) {
	if (aState == BULLET_INIT) {
		reset(0, DIR_NONE);
		changeState(BULLET_AVAILABLE);
	} else if (aState == BULLET_AVAILABLE) {

	} else if (aState == BULLET_ACTIVE) {
		_timeElapsed = 0;
	} else if (aState == BULLET_BOUNCE) {
		if (!bounceBullet()) {
			changeState(BULLET_EXPIRE);
		} else {
			changeState(BULLET_ACTIVE);
		}
	} else if (aState == BULLET_HIT) {
		// Animation?
	} else if (aState == BULLET_EXPLODE) {
		// Animation?
	} else if (aState == BULLET_EXPIRE) {
		// Animation?
	}
}

void Bullet::loopState(BulletState aState) {
	if (aState == BULLET_INIT) {

	} else if (aState == BULLET_AVAILABLE) {

	} else if (aState == BULLET_ACTIVE) {
		updateMovement();
	} else if (aState == BULLET_BOUNCE) {

	} else if (aState == BULLET_HIT) {
		if (1) {
			changeState(BULLET_INIT);
		}
	} else if (aState == BULLET_EXPLODE) {
		// Wait until animation ends.
		if (1) {
			changeState(BULLET_INIT);
		}
	} else if (aState == BULLET_EXPIRE) {
		if (1) {
			changeState(BULLET_INIT);
		}
	}
}

void Bullet::endState(BulletState aState) {
	if (aState == BULLET_INIT) {

	} else if (aState == BULLET_AVAILABLE) {

	} else if (aState == BULLET_ACTIVE) {

	} else if (aState == BULLET_BOUNCE) {

	} else if (aState == BULLET_HIT) {

	} else if (aState == BULLET_EXPLODE) {

	} else if (aState == BULLET_EXPIRE) {
		// Animation?
	}
}

void Bullet::updateMovement() {
	if (_timeElapsed >= _movementDelay) {
		_timeElapsed = 0;
		//_lastIndex = _index;

		TileType tile = TILE_BACKGROUND;
		uint16_t newIndex = _gameManager->getLevel()->getNewPosition(_index, _direction, tile);

		if ((tile == TILE_BOUNDARY) || (tile == TILE_WALL)) {
			changeState(BULLET_BOUNCE);
		} else {
			tile = TILE_BACKGROUND;
			// Check for a collision.
			_gameManager->getTankOne()->checkCollisionAtIndex(TILE_BULLET, newIndex, tile);

			if (tile == TILE_BACKGROUND) {
				_gameManager->getTankTwo()->checkCollisionAtIndex(TILE_BULLET, newIndex, tile);
			}

			if (tile == TILE_TANK_ONE) {
				if (_gameManager->getTankOne()->isInvulnerable()) {
					changeState(BULLET_BOUNCE);
				} else {
					changeState(BULLET_HIT);
				}
			} else if (tile == TILE_TANK_TWO) {
				if (_gameManager->getTankTwo()->isInvulnerable()) {
					changeState(BULLET_BOUNCE);
				} else {
					changeState(BULLET_HIT);
				}
			} else if (tile == TILE_BULLET) {
				changeState(BULLET_BOUNCE);
			} else if ((tile == TILE_TURRET_ONE) || (tile == TILE_TURRET_TWO)) {
				_index = newIndex;
			} else {
				_index = newIndex;
			}
		}

		/*if (_overlapTile == TILE_BULLET) {
			_overlapTile = _lastOverlapTile;

		} else if ((_overlapTile == TILE_BOUNDARY) || (_overlapTile == TILE_WALL)) {

			_index = _lastIndex;
			if (!bounceBullet()) {
				//this->endOfLife = true;
			}
		} else if ((_overlapTile == TILE_TANK_ONE) || (_overlapTile == TILE_TANK_TWO)) {
			DEBUG("Hit tank!");
			//this->collided = true;
			//this->endOfLife = true;
		} else {
			_gameManager->getLevel()->setTileAtIndex(_lastOverlapTile, _lastIndex);
			_gameManager->getLevel()->setTileAtIndex(TILE_BULLET, _index);
		}*/
	}
}

bool Bullet::bounceBullet() {
	if (_bouncesLeft > 1) {
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
		_gameManager->getLevel()->getNewPosition(_index, _direction, collisionTile);
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