#include "mwam_game_manager.h"
#include "mwam_manager.h"

extern char* itoa(int a, char* buffer, unsigned char radix);

namespace mwam
{

/* Public Methods */

GameManager::GameManager() {
}

void GameManager::initialize(StateController *aStateController) {
	_stateController = aStateController;
	_dataManager = Manager::getInstance().dataManager;
	_hardwareManager = Manager::getInstance().hardwareManager;

	_level = new Level();
	_level->initialize(THEME_DEFAULT);
	_tankOne = new Tank();
	_tankOne->initialize(TANK_ONE, kPlayerOneStartIndex, kIntervalPlayerDelayMillis);
	_tankTwo = new Tank();
	_tankTwo->initialize(TANK_TWO, kPlayerTwoStartIndex, kIntervalPlayerDelayMillis);

	//_hardwareManager->ledSet()->setFastUpdates(_tankOne, _tankTwo);
}

void GameManager::reset() {
	_playerOneActive = false;
	_playerTwoActive = false;

	_hardwareManager->resetHardware();
	//generateWalls();
	_hardwareManager->ledSet()->updateLeds(_level);
}

void GameManager::updateIntro() {
}

void GameManager::updateTutorial() {
}

void GameManager::generateWalls() {
	for (uint16_t i = 100; i > 21; i -= 21) {
		_level->setTileAtIndex(TILE_WALL, i);
	}
}


void GameManager::updateWaiting() {
	//_hardwareManager->ledSet()->setColor(random(0, 240), Color(random(0, 127), random(0, 127), random(0, 127)));
}

void GameManager::initSelect() {
	/*bool writeTile = true;
	bool longRow = true;
	for (uint8_t i = 0; i < kLedCount; i++) {
	}
	for (uint8_t i = 0; i < kLedsPerRing; i++) {
		if (i < (kLedsPerRing / 2)) {
			_level->setTileAtIndex(TILE_TANK_ONE, i);
		} else {
			_level->setTileAtIndex(TILE_TANK_TWO, i);
		}
	}*/

	//_level->drawLine(DIR_UP, TILE_BULLET, 219, 5);

	/*_level->drawLine(DIR_UP_LEFT, TILE_BULLET, 198, 5);
	_level->drawLine(DIR_UP_LEFT, TILE_TANK_ONE, 199, 5);
	_level->drawLine(DIR_UP_LEFT, TILE_TANK_TWO, 200, 5);
	_level->drawLine(DIR_UP_LEFT, TILE_WALL, 201, 5);
	_level->drawLine(DIR_UP_LEFT, TILE_BULLET, 202, 5);
	_level->drawLine(DIR_UP_LEFT, TILE_TANK_ONE, 203, 5);
	_level->drawLine(DIR_UP_LEFT, TILE_TANK_TWO, 204, 5);
	_level->drawLine(DIR_UP_LEFT, TILE_WALL, 205, 5);
	_level->drawLine(DIR_UP_LEFT, TILE_BULLET, 206, 5);
	_level->drawLine(DIR_UP_LEFT, TILE_TANK_ONE, 207, 5);
	_level->drawLine(DIR_UP_LEFT, TILE_TANK_TWO, 208, 5);
	_level->drawLine(DIR_UP_LEFT, TILE_WALL, 209, 5);*/

	/*_level->drawLine(DIR_UP_RIGHT, TILE_TANK_TWO, 196, 5);
	_level->drawLine(DIR_UP_RIGHT, TILE_WALL, 197, 5);
	_level->drawLine(DIR_UP_RIGHT, TILE_BULLET, 198, 5);
	_level->drawLine(DIR_UP_RIGHT, TILE_TANK_ONE, 199, 5);
	_level->drawLine(DIR_UP_RIGHT, TILE_TANK_TWO, 200, 5);
	_level->drawLine(DIR_UP_RIGHT, TILE_WALL, 201, 5);
	_level->drawLine(DIR_UP_RIGHT, TILE_BULLET, 202, 5);
	_level->drawLine(DIR_UP_RIGHT, TILE_TANK_ONE, 203, 5);
	_level->drawLine(DIR_UP_RIGHT, TILE_TANK_TWO, 204, 5);
	_level->drawLine(DIR_UP_RIGHT, TILE_WALL, 205, 5);
	_level->drawLine(DIR_UP_RIGHT, TILE_BULLET, 206, 5);
	_level->drawLine(DIR_UP_RIGHT, TILE_TANK_ONE, 207, 5);
	_level->drawLine(DIR_UP_RIGHT, TILE_TANK_TWO, 208, 5);
	_level->drawLine(DIR_UP_RIGHT, TILE_WALL, 209, 5);*/

	//_level->setTileAtIndex(TILE_TANK_TWO, 180);

	/*_level->drawLine(DIR_DOWN_RIGHT, TILE_TANK_TWO, 33, 5);
	_level->drawLine(DIR_DOWN_RIGHT, TILE_WALL, 34, 5);
	_level->drawLine(DIR_DOWN_RIGHT, TILE_BULLET, 35, 5);
	_level->drawLine(DIR_DOWN_RIGHT, TILE_TANK_ONE, 36, 5);
	_level->drawLine(DIR_DOWN_RIGHT, TILE_TANK_TWO, 37, 5);
	_level->drawLine(DIR_DOWN_RIGHT, TILE_WALL, 38, 5);
	_level->drawLine(DIR_DOWN_RIGHT, TILE_BULLET, 39, 5);
	_level->drawLine(DIR_DOWN_RIGHT, TILE_TANK_ONE, 40, 5);
	_level->drawLine(DIR_DOWN_RIGHT, TILE_TANK_TWO, 21, 5);
	_level->drawLine(DIR_DOWN_RIGHT, TILE_WALL, 22, 5);
	_level->drawLine(DIR_DOWN_RIGHT, TILE_BULLET, 23, 5);
	_level->drawLine(DIR_DOWN_RIGHT, TILE_TANK_ONE, 24, 5);
	_level->drawLine(DIR_DOWN_RIGHT, TILE_TANK_TWO, 25, 5);
	_level->drawLine(DIR_DOWN_RIGHT, TILE_WALL, 26, 5);*/

	/*_level->drawLine(DIR_DOWN_LEFT, TILE_TANK_TWO, 33, 5);
	_level->drawLine(DIR_DOWN_LEFT, TILE_WALL, 34, 5);
	_level->drawLine(DIR_DOWN_LEFT, TILE_BULLET, 35, 5);
	_level->drawLine(DIR_DOWN_LEFT, TILE_TANK_ONE, 36, 5);
	_level->drawLine(DIR_DOWN_LEFT, TILE_TANK_TWO, 37, 5);
	_level->drawLine(DIR_DOWN_LEFT, TILE_WALL, 38, 5);
	_level->drawLine(DIR_DOWN_LEFT, TILE_BULLET, 39, 5);
	_level->drawLine(DIR_DOWN_LEFT, TILE_TANK_ONE, 40, 5);
	_level->drawLine(DIR_DOWN_LEFT, TILE_BULLET, 41, 5);
	_level->drawLine(DIR_DOWN_LEFT, TILE_TANK_TWO, 21, 5);
	_level->drawLine(DIR_DOWN_LEFT, TILE_WALL, 22, 5);
	_level->drawLine(DIR_DOWN_LEFT, TILE_BULLET, 23, 5);
	_level->drawLine(DIR_DOWN_LEFT, TILE_TANK_ONE, 24, 5);
	_level->drawLine(DIR_DOWN_LEFT, TILE_TANK_TWO, 25, 5);
	_level->drawLine(DIR_DOWN_LEFT, TILE_WALL, 26, 5);*/

	//_level->drawSquare(false, TILE_WALL, 238, 3, 5);
	//_level->drawSquare(true, TILE_BULLET, 206, 5, 5);
	char hi[] = "Tank";

	_level->drawText(TILE_WALL, 220, hi, 4);

	//_level->drawSquare(true, TILE_WALL, 19, 5, 4);

	//_level->drawLine(DIR_DOWN_LEFT, TILE_BULLET, 40, 5);

	//_level->drawLine(DIR_DOWN_RIGHT, TILE_WALL, 70, 5);

	//_level->drawLine(DIR_LEFT, TILE_BULLET, 20, 22);

	//_level->drawLine(DIR_RIGHT, TILE_WALL, 235, 6);
}

void GameManager::updateSelect() {
	//_hardwareManager->ledSet()->setColor(random(0, 240), Color(random(0, 127), random(0, 127), random(0, 127)));

}

void GameManager::updatePlay() {
	/*if (_hardwareManager->button()->wasClicked()) {
		DEBUG("Change theme!");
		_level->nextTheme();
	}*/
	Direction dir;
	JoystickThreshold threshold;
	TankState state = _tankOne->getState();
	if (state == TANK_DESTROYED) {
		DEBUG("DESTROYED!");
		Animation a;
		a.endColor = kColorRed;
		a.yoyo = true;
		a.repeats = 1;
		a.ease = EASE_QUAD_IN_OUT;
		a.tweenTime = 1000;
		_hardwareManager->ledSet()->animateLed(_tankOne->getIndex(), a, false);
		a.delayTime = 250;
		_hardwareManager->ledSet()->animateLed(_tankOne->getIndex() + kLedDiagUpLeft, a, false);
		_hardwareManager->ledSet()->animateLed(_tankOne->getIndex() + kLedDiagUpRight, a, false);
		_hardwareManager->ledSet()->animateLed(_tankOne->getIndex() + kLedDiagDownLeft, a, false);
		_hardwareManager->ledSet()->animateLed(_tankOne->getIndex() + kLedDiagDownRight, a, false);
		_tankOne->setState(TANK_WAITING);
	} else if (state == TANK_ACTIVE) {
		dir = _hardwareManager->joystickOne()->getDirection();
		threshold = _hardwareManager->joystickOne()->getThreshold();
		if (dir) {
			//DEBUG("Tank: %d", _tankOne->getIndex());
			uint16_t movementDelay = kIntervalPlayerDelayMillis - (threshold * kIntervalPlayerSpeedMillis);
			if (_hardwareManager->joystickOne()->clickDown()) {
				movementDelay = 0;
			}
			_tankOne->updateState(dir, movementDelay);
		}
		if (_hardwareManager->joystickOne()->clickUp()) {
			_tankOne->fireBullet();
		}
	}
	_tankOne->updateBullets();

	dir = _hardwareManager->joystickTwo()->getDirection();
	threshold = _hardwareManager->joystickTwo()->getThreshold();
	if (dir) {
		uint16_t movementDelay = kIntervalPlayerDelayMillis - (threshold * kIntervalPlayerSpeedMillis);
		if (_hardwareManager->joystickTwo()->clickDown()) {
			movementDelay = 0;
		}
		_tankTwo->updateState(dir, movementDelay);
	}
	if (_hardwareManager->joystickTwo()->clickUp()) {
		_tankTwo->fireBullet();
	}
	_tankTwo->updateBullets();
}


void GameManager::updateGameOver(bool aWonGame) {
}


void GameManager::playLedTest() {
	/*Animation a = Animation();
	a.startColor = Color(0, 0, 0);
	a.endColor = Color(0, 0, 127);
	a.repeats = 5;
	a.yoyo = true;
	a.tweenTime = 1000;
	a.ease = EASE_QUAD_IN_OUT;

	_hardwareManager->ledSet()->animateAll(a, true, 250, false);*/

	for (int16_t i = 0; i < _hardwareManager->ledSet()->ledCount(); ++i) {
		_hardwareManager->ledSet()->setColor(i, Color(0, 0, 127));
	}
	delay(4000);
	/*for (int16_t i = 0; i < _hardwareManager->ledSet()->ledCount(); i+=2) {
		_hardwareManager->ledSet()->setColor(i, Color(127, 0, 0));
	}*/
	for (int16_t i = 0; i < 21; ++i) {
		for (int16_t j = 0; j < 6; ++j) {
			_hardwareManager->ledSet()->setColor(i + (41 * j), Color(127, 0, 0));
		}
	}
}

/*void GameManager::updateTitleAnimation() {

}

void GameManager::drawPlayerScreen() {

}*/



/* Accessors */

Level* GameManager::getLevel() {
	return _level;
}

Tank* GameManager::getTankOne() {
	return _tankOne;
}

Tank* GameManager::getTankTwo() {
	return _tankTwo;
}

/* Private Methods */

}