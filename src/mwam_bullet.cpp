#include "mwam_bullet.h"
#include "mwam_manager.h"

namespace mwam
{

struct BulletStateStr_t {
	BulletState state;
	const char *stateDesc;
} BulletStateDesc[] = {
	{ BULLET_INIT, "init" },
	{ BULLET_AVAILABLE, "available" },
	{ BULLET_ACTIVE, "active" },
	{ BULLET_BOUNCE, "bounce" },
	{ BULLET_HIT, "hit" },
	{ BULLET_EXPLODE, "explode" },
	{ BULLET_EXPIRE, "expire" }
};

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

const char* Bullet::stateString() {
	return BulletStateDesc[_state].stateDesc;
}

void Bullet::reset(uint16_t aIndex, Direction aDir) {
	_index = aIndex;
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

uint16_t Bullet::getIndex() {
	return _index;
}

void Bullet::setIndex(uint16_t aIndex) {
	_index = aIndex;
}

/* Private Methods */

void Bullet::initState(BulletState aState) {
	//LOG("Init State: %s", stateString());

	if (aState == BULLET_INIT) {
		reset(0, DIR_NONE);
		changeState(BULLET_AVAILABLE);
	} else if (aState == BULLET_AVAILABLE) {

	} else if (aState == BULLET_ACTIVE) {

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
		Animation a;
		a.endColor = kColorBlack;
		a.repeats = 0;
		a.ease = EASE_QUAD_IN_OUT;
		a.tweenTime = 500;
		Manager::getInstance().hardwareManager->ledSet()->animateLed(_index, a, true);
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
	//LOG("End State: %s", stateString());

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
					_gameManager->getTankOne()->changeState(TANK_HIT);
				}
			} else if (tile == TILE_TANK_TWO) {
				if (_gameManager->getTankTwo()->isInvulnerable()) {
					changeState(BULLET_BOUNCE);
				} else {
					changeState(BULLET_HIT);
					_gameManager->getTankTwo()->changeState(TANK_HIT);
				}
			} else if (tile == TILE_BULLET) {
				changeState(BULLET_BOUNCE);
			} else if ((tile == TILE_TURRET_ONE) || (tile == TILE_TURRET_TWO)) {
				_index = newIndex;
			} else {
				_index = newIndex;
			}
		}
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
			collisionTile = TILE_BACKGROUND;
			_gameManager->getLevel()->getNewPosition(_index, _direction, collisionTile);
			if ((collisionTile == TILE_BOUNDARY) || (collisionTile == TILE_WALL)) {
				return false;
			}
		}

		_bouncesLeft--;
		_timeElapsed = _movementDelay;
		return true;
	}
	return false;
}

}