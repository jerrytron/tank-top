#include "mwam_tank.h"
#include "mwam_manager.h"
#include "mwam_joystick.h"

namespace mwam
{

struct TankStateStr_t {
	TankState state;
	const char *stateDesc;
} TankStateDesc[] = {
	{ TANK_INIT, "init" },
	{ TANK_RESETTING, "resetting" },
	{ TANK_ACTIVE, "active" },
	{ TANK_HIT, "hit" },
	{ TANK_DESTROYED, "destroyed" },
	{ TANK_GAME_OVER, "game_over" }
};

/* Public Methods */

Tank::Tank() {
}

void Tank::initialize(TankNumber aTankNum, uint8_t aStartIndex) {
	_tankNumber = aTankNum;
	_startIndex = aStartIndex;
	_gameManager = Manager::getInstance().gameManager;
	_hardwareManager = Manager::getInstance().hardwareManager;

	if (_tankNumber == TANK_ONE) {
		_joystick = Manager::getInstance().hardwareManager->joystickOne();
	} else if (_tankNumber == TANK_TWO) {
		_joystick = Manager::getInstance().hardwareManager->joystickTwo();
	}

	//_movementFreq = aMovementFreq;
	_moveElapsed = 0;
	_state = TANK_INIT;
	initState(TANK_INIT);
}

const char* Tank::stateString() {
	return TankStateDesc[_state].stateDesc;
}

void Tank::reset() {
	_resetElapsed = 0;
	_index = _startIndex;
	_lastIndex = _startIndex;
	_blinks = 0;
	_visible = true;
	_invulnerable = true;
	_direction = DIR_DOWN_LEFT;
	_turretIndex = _index + kLedDiagDownLeft;
	_lastTurretIndex = 0;
	_health = kHealthTotal;
	_lives = kLivesTotal;
	for (uint8_t i = 0; i < kMaxBulletsLive; ++i) {
		_bullets[i].initialize(kBulletMovementDelay);
	}
}

TankState Tank::getState() {
	return _state;
}

void Tank::changeState(TankState aState) {
	endState(_state);
	_state = aState;
	initState(_state);
}

void Tank::updateState() {
	loopState(_state);
}

/* Accessors */

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

/*TileType Tank::getLastOverlapTile() {
	return _lastOverlap;
}*/

uint8_t Tank::getBulletCount() {
	uint8_t total = 0;
	for (uint8_t i = 0; i < kMaxBulletsLive; ++i) {
		if (_bullets[i].getState() == BULLET_ACTIVE) {
			total++;
		}
	}
	return total;
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

bool Tank::isVisible() {
	return _visible;
}

/*TileType Tank::getTurretOverlapTile() {
	return _turretOverlap;
}

TileType Tank::getLastTurretOverlapTile() {
	return _lastTurretOverlap;
}*/

/* Private Methods */

void Tank::initState(TankState aState) {
	LOG("Init State: %s", stateString());

	if (aState == TANK_INIT) {
		reset();
	} else if (aState == TANK_RESETTING) {
		_resetElapsed = 0;
		_index = _startIndex;
		_lastIndex = _startIndex;
		_blinks = 0;
		_visible = true;
		_invulnerable = true;
		_direction = DIR_DOWN_LEFT;
		_turretIndex = _index + kLedDiagDownLeft;
		_lastTurretIndex = 0;
		_health = kHealthTotal;
	} else if (aState == TANK_ACTIVE) {

	} else if (aState == TANK_HIT) {
		_health--;

		_anim.endColor = kColorRed;
		_anim.yoyo = true;
		_anim.repeats = 1;
		_anim.ease = EASE_QUAD_IN_OUT;
		_anim.tweenTime = 1000;
		_hardwareManager->ledSet()->animateLed(_index, _anim, false);
		_anim.delayTime = 250;
		_hardwareManager->ledSet()->animateLed(_index + kLedDiagUpLeft, _anim, false);
		_hardwareManager->ledSet()->animateLed(_index + kLedDiagUpRight, _anim, false);
		_hardwareManager->ledSet()->animateLed(_index + kLedDiagDownLeft, _anim, false);
		_hardwareManager->ledSet()->animateLed(_index + kLedDiagDownRight, _anim, false);
	} else if (aState == TANK_DESTROYED) {
		_lives--;

		_anim.endColor = kColorRed;
		_anim.yoyo = true;
		_anim.repeats = 1;
		_anim.ease = EASE_QUAD_IN_OUT;
		_anim.tweenTime = 1000;
		_hardwareManager->ledSet()->animateLed(_index, _anim, false);
		_anim.delayTime = 250;
		_hardwareManager->ledSet()->animateLed(_index + kLedDiagUpLeft, _anim, false);
		_hardwareManager->ledSet()->animateLed(_index + kLedDiagUpRight, _anim, false);
		_hardwareManager->ledSet()->animateLed(_index + kLedDiagDownLeft, _anim, false);
		_hardwareManager->ledSet()->animateLed(_index + kLedDiagDownRight, _anim, false);
	} else if (aState == TANK_GAME_OVER) {

	}
}

void Tank::loopState(TankState aState) {
	if (aState == TANK_INIT) {
		changeState(TANK_ACTIVE);
	} else if (aState == TANK_RESETTING) {
		/*if (_resetElapsed >= kBlinkIntervalMillis) {
			_resetElapsed = 0;
			_blinks++;
			_visible = !_visible;
			if (_blinks > kBlinkMax ) {
				changeState(TANK_ACTIVE);
			}
		}*/

		changeState(TANK_ACTIVE);
	} else if (aState == TANK_ACTIVE) {
		if (_invulnerable) {
			if (_resetElapsed >= kBlinkIntervalMillis) {
				_resetElapsed = 0;
				_blinks++;
				_visible = !_visible;
				if (_blinks > kBlinkMax ) {
					_invulnerable = false;
				}
			}
		}

		Direction dir = _joystick->getDirection();
		JoystickThreshold threshold = _joystick->getThreshold();

		if (dir) {
			uint16_t movementDelay = kIntervalPlayerDelayMillis - (threshold * kIntervalPlayerSpeedMillis);
			if (_joystick->clickDown()) {
				movementDelay = 0;
			}
			updateMovement(dir, movementDelay);
		}
		updateBullets();
		if (_joystick->clickUp()) {
			fireBullet();
		}
	} else if (aState == TANK_HIT) {
		if (_hardwareManager->ledSet()->allAnimsDone()) {
			if (_health == 0) {
				changeState(TANK_DESTROYED);
			} else {
				_invulnerable = true;
				changeState(TANK_ACTIVE);
			}
		}
	} else if (aState == TANK_DESTROYED) {
		if (_hardwareManager->ledSet()->allAnimsDone()) {
			if (_lives == 0) {
				changeState(TANK_GAME_OVER);
				_gameManager->getOtherTank(_tankNumber)->changeState(TANK_GAME_OVER);
				_gameManager->gameOver();
			} else {
				changeState(TANK_RESETTING);
				_gameManager->getOtherTank(_tankNumber)->changeState(TANK_RESETTING);
			}
		}
	} else if (aState == TANK_GAME_OVER) {

	}
}

void Tank::endState(TankState aState) {
	LOG("Init State: %s", stateString());

	if (aState == TANK_INIT) {

	} else if (aState == TANK_RESETTING) {
		_blinks = 0;
		_visible = true;
	} else if (aState == TANK_ACTIVE) {
		_direction = DIR_NONE;
		_movementFreq = 0;
	} else if (aState == TANK_HIT) {

	} else if (aState == TANK_DESTROYED) {

	} else if (aState == TANK_GAME_OVER) {

	}
}

bool Tank::checkCollisionAtIndex(uint16_t aIndex, TileType &aCollision) {
	bool collided = false;
	if (aIndex == _index) {
		collided = true;
		if (_tankNumber == TANK_ONE) {
			aCollision = TILE_TANK_ONE;
		} else if (_tankNumber == TANK_TWO) {
			aCollision = TILE_TANK_TWO;
		}
	}
	if (kTurretCollision) {
		if (aIndex == _turretIndex) {
			collided = true;
			if (_tankNumber == TANK_ONE) {
				aCollision = TILE_TURRET_ONE;
			} else if (_tankNumber == TANK_TWO) {
				aCollision = TILE_TURRET_TWO;
			}
		}
	}
	if (checkBulletCollision(aIndex)) {
		collided = true;
		aCollision = TILE_BULLET;
	}

	return collided;
}

bool Tank::checkBulletCollision(uint16_t aIndex) {
	for (uint8_t i = 0; i < kMaxBulletsLive; ++i) {
		if (_bullets[i].getState() == BULLET_ACTIVE) {
			if (aIndex == _bullets[i].getIndex()) {
				_bullets[i].changeState(BULLET_HIT);
				return true;
			}
		}
	}
	return false;
}

void Tank::updateMovement(Direction aDirection, uint16_t aMovementFreq) {
	if (_moveElapsed >= aMovementFreq) {
		_moveElapsed = 0;

		if ((_direction == DIR_NONE) && (aDirection != DIR_NONE)) {
			_moveElapsed = _movementFreq;
		}
		Direction dir = aDirection;
		_lastIndex = _index;

		TileType tile = TILE_BACKGROUND;

		// Get new position index while checking for screen boundaries and walls.
		uint16_t newIndex = _gameManager->getLevel()->getNewPosition(_index, dir, tile);

		bool moved = false;

		if ((tile == TILE_BOUNDARY) || (tile == TILE_WALL)) {
			// Barrier, don't do anything.
		} else if (_gameManager->getOtherTank(_tankNumber)->checkCollisionAtIndex(newIndex, tile)) {
			if ((tile == TILE_TANK_ONE) || (tile == TILE_TANK_TWO)) {
				// Don't do anything.
			} else if ((tile == TILE_TURRET_ONE) || (tile == TILE_TURRET_TWO)) {
				moved = true;
			} else if (tile == TILE_BULLET) {
				moved = true;
				if (!_invulnerable) {
					changeState(TANK_HIT);
				}
			}
		} else if (checkBulletCollision(_index)) {
			if (kFriendlyFire) {

				moved = true;
				if (!_invulnerable) {
					changeState(TANK_HIT);
				}
			} else {
				// Barrier, don't do anything.
			}
		}

		if (moved) {
			_index = newIndex;
		}
	}
}

uint16_t Tank::findTurretIndex() {
	TileType collisionTile = TILE_BOUNDARY;
	//uint16_t index = _level->getNewPosition(_index, _direction, collisionTile);
	uint16_t index = _index;
	Direction tryDir = _direction;
	bool searching = true;
	while (searching) {
		index = _gameManager->getLevel()->getNewPosition(_index, tryDir, collisionTile);
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

	/*if ((_turretOverlap != TILE_TURRET_ONE) && (_turretOverlap != TILE_TURRET_TWO) &&
	    (_turretOverlap != TILE_TANK_ONE) && (_turretOverlap != TILE_TANK_TWO)) {
		_lastTurretOverlap = _turretOverlap;
	}
	_turretOverlap = _gameManager->getLevel()->getTileAtIndex(index);*/
	return index;
}

void Tank::updateBullets() {
	for (uint8_t i = 0; i < kMaxBulletsLive; ++i) {
		_bullets[i].updateState();
		/*if (_bullets[i].collided) {
			DEBUG("Bullet hit tank!");
			_bullets[i].collided = false;
			_tankState = TANK_DESTROYED;
			_health--;
		}
		if (_bullets[i].endOfLife) {
			_gameManager->getLevel()->setTileAtIndex(_bullets[i].getOverlapTile(), _bullets[i].getIndex());
			_bullets[i].endOfLife = false;
			_bullets[i].active = false;
		}*/
	}
}

bool Tank::fireBullet() {
	DEBUG("try to fire: %d", _bulletCount);
	if (getBulletCount() < kMaxBulletsLive) {
		DEBUG("can fire");
		uint8_t i;
		for (i = 0; i < kMaxBulletsLive; ++i) {
			if (_bullets[i].getState() == BULLET_AVAILABLE) {
				DEBUG("Fire!");
				_bullets[i].reset(_turretIndex, _direction);
				_bullets[i].changeState(BULLET_ACTIVE);
				break;
			}
		}
		return true;
	}
	return false;
}

}