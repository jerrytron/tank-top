#include "mwam_game_manager.h"
#include "mwam_manager.h"
#include "mwam_text_renderer.h"

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
	_textRenderer = new TextRenderer();
	_textRenderer->initialize(_level, false);
	_tankOneReady = false;
	_tankTwoReady = false;
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

void GameManager::initWaiting() {
	//_level->setTheme(THEME_SPOOKY);
	_textRenderer->newMessage(kTankTop, kTankTopLen, 5, 0, 100, -10);
	_tileIndex = 1;
	_timeElapsed = 0;
}

void GameManager::updateWaiting() {
	if (_waitingElapsed >= 10) {
		_waitingElapsed = 0;
		_textRenderer->renderText((TileType)_tileIndex);

		if (_timeElapsed >= 1000) {
			_timeElapsed = 0;
			_tileIndex += 1;
			if (_tileIndex > 6) {
				_tileIndex = 1;
			}
			DEBUG("Tile Index: %d", _tileIndex);
		}
	}
	//_hardwareManager->ledSet()->setColor(random(0, 240), Color(random(0, 127), random(0, 127), random(0, 127)));
}

void GameManager::endWaiting() {
	_textRenderer->stopMessage();
}

void GameManager::initSelect() {
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

	_level->drawLine(DIR_RIGHT, TILE_TANK_ONE, 218, 11);
	_level->drawLine(DIR_RIGHT, TILE_TANK_ONE, 13, 11);
	_level->drawLine(DIR_DOWN, TILE_TANK_ONE, 218, 7);
	_level->drawLine(DIR_DOWN, TILE_TANK_ONE, 228, 7);

	_level->drawLine(DIR_RIGHT, TILE_TANK_TWO, 229, 10);
	_level->drawLine(DIR_RIGHT, TILE_TANK_TWO, 24, 10);
	_level->drawLine(DIR_DOWN, TILE_TANK_TWO, 229, 7);
	_level->drawLine(DIR_DOWN, TILE_TANK_TWO, 238, 7);
}

void GameManager::updateSelect() {
	//_hardwareManager->ledSet()->setColor(random(0, 240), Color(random(0, 127), random(0, 127), random(0, 127)));
	if (_selectElapsed >= 10) {
		_selectElapsed = 0;
		if (!_tankOneReady && _hardwareManager->joystickOne()->clickUp()) {
			_tankOneReady = true;
			_timeElapsed = 0;
			_level->drawSquare(false, TILE_TANK_ONE, 218, 11, 7);
		}
		if (!_tankTwoReady && _hardwareManager->joystickTwo()->clickUp()) {
			_tankTwoReady = true;
			_timeElapsed = 0;
			_level->drawSquare(false, TILE_TANK_TWO, 229, 10, 7);
		}
		if (_tankOneReady && _tankTwoReady) {
			if (_timeElapsed >= 2000) {
				_stateController->changeState(STATE_PLAY);
			}
		}
	}
}

void GameManager::endSelect() {
	DEBUG("endSelect");
	_level->clearLevel();
}

void GameManager::initPlay() {
	_tankOne = new Tank();
	_tankOne->initialize(TANK_ONE, kPlayerOneStartIndex);
	_tankTwo = new Tank();
	_tankTwo->initialize(TANK_TWO, kPlayerTwoStartIndex);
}

void GameManager::updatePlay() {
	/*if (_hardwareManager->button()->wasClicked()) {
		DEBUG("Change theme!");
		_level->nextTheme();
	}*/

	_level->clearLevel();
	drawLevel(); // Place walls / obstacles.

	_tankOne->updateState();
	_tankTwo->updateState();

	/*if (state == TANK_DESTROYED) {
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
	_tankOne->updateBullets();*/

	/*dir = _hardwareManager->joystickTwo()->getDirection();
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
	_tankTwo->updateBullets();*/


	drawObjects();
}

void GameManager::endPlay() {

}

void GameManager::drawLevel() {
	_level->drawLine(DIR_DOWN, TILE_WALL, 178, 4);
	_level->drawLine(DIR_DOWN, TILE_WALL, 184, 4);

	_level->drawLine(DIR_DOWN, TILE_WALL, 190, 4);
	_level->drawLine(DIR_DOWN, TILE_WALL, 196, 4);
}


void GameManager::drawObjects() {
	// Order of drawing is important!
	if (_tankOne->isVisible()) {
		_level->setTileAtIndex(TILE_TANK_ONE, _tankOne->getIndex());
		_level->setTileAtIndex(TILE_TURRET_ONE, _tankOne->getTurretIndex());
	}

	if (_tankTwo->isVisible()) {
		_level->setTileAtIndex(TILE_TANK_TWO, _tankTwo->getIndex());
		_level->setTileAtIndex(TILE_TURRET_TWO, _tankTwo->getTurretIndex());
	}

	for (int i = 0; i < kMaxBulletsLive; i++) {
		if (_tankOne->getBulletAtIndex(i)->getState() == BULLET_ACTIVE) {
			_level->setTileAtIndex(TILE_BULLET, _tankOne->getBulletAtIndex(i)->getIndex());
		}
		if (_tankTwo->getBulletAtIndex(i)->getState() == BULLET_ACTIVE) {
			_level->setTileAtIndex(TILE_BULLET, _tankTwo->getBulletAtIndex(i)->getIndex());
		}
	}
}

void GameManager::gameOver() {
	_stateController->changeState(STATE_GAME_OVER);
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

Tank* GameManager::getOtherTank(TankNumber aTankNum) {
	if (aTankNum == TANK_ONE) {
		return _tankTwo;
	} else if (aTankNum == TANK_TWO) {
		return _tankOne;
	}
	return NULL;
}

/* Private Methods */

}