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
	_hardwareManager->resetHardware();
	generateWalls();
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

void GameManager::updatePlay() {
	if (_hardwareManager->button()->wasClicked()) {
		DEBUG("Change theme!");
		_level->nextTheme();
	}
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
		_hardwareManager->ledSet()->animateLed(_tankOne->getIndex() - kLedDiagDownLeft, a, false);
		_hardwareManager->ledSet()->animateLed(_tankOne->getIndex() - kLedDiagDownRight, a, false);
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

void GameManager::updateAnimations() {
	_hardwareManager->ledSet()->setColor(random(0, 240), Color(random(0, 127), random(0, 127), random(0, 127)));
}

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