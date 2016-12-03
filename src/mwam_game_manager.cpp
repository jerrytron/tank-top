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
	_level->initialize(THEME_ONE, DIM_THREE_QUARTER);
	_textRenderer = new TextRenderer();
	_textRenderer->initialize(_level, false);
}

void GameManager::reset() {
	_tankOneReady = false;
	_tankTwoReady = false;

	_hardwareManager->resetHardware();
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
	_textRenderer->newMessage(kTankTop, kTankTopLen, 5, 0, 100, -10);
	_tileIndex = 1;
	_timeElapsed = 0;
	_waitingElapsed = 0;
}

void GameManager::updateWaiting() {
	if (_waitingElapsed >= 5) {
		_waitingElapsed = 0;
		_textRenderer->renderText((TileType)_tileIndex);

		if (_timeElapsed >= 1000) {
			_timeElapsed = 0;
			_tileIndex += 1;
			if (_tileIndex > 6) {
				_tileIndex = 1;
			}
		}
	}
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
#ifdef CYLINDRUS
	_level->drawLine(DIR_RIGHT, TILE_WALL, 582, 15);
	_level->drawLine(DIR_RIGHT, TILE_WALL, 24, 15);
	_level->drawLine(DIR_DOWN, TILE_WALL, 582, 19);
	_level->drawLine(DIR_DOWN, TILE_WALL, 596, 19);

	_level->drawLine(DIR_RIGHT, TILE_WALL, 566, 16);
	_level->drawLine(DIR_RIGHT, TILE_WALL, 8, 16);
	_level->drawLine(DIR_DOWN, TILE_WALL, 566, 19);
	_level->drawLine(DIR_DOWN, TILE_WALL, 581, 19);

	_level->setTileAtIndex(TILE_TANK_ONE, kPlayerOneStartIndex);
	_level->setTileAtIndex(TILE_TURRET_ONE, kPlayerOneStartIndex + kLedLeft);
	_level->setTileAtIndex(TILE_BULLET, kPlayerOneStartIndex + (kLedLeft * 3));

	_level->setTileAtIndex(TILE_TANK_TWO, kPlayerTwoStartIndex);
	_level->setTileAtIndex(TILE_TURRET_TWO, kPlayerTwoStartIndex + kLedLeft);
	_level->setTileAtIndex(TILE_BULLET, kPlayerTwoStartIndex + (kLedLeft * 3));
#else
	_level->drawLine(DIR_RIGHT, TILE_WALL, 218, 11);
	_level->drawLine(DIR_RIGHT, TILE_WALL, 13, 11);
	_level->drawLine(DIR_DOWN, TILE_WALL, 218, 7);
	_level->drawLine(DIR_DOWN, TILE_WALL, 228, 7);

	_level->drawLine(DIR_RIGHT, TILE_WALL, 229, 10);
	_level->drawLine(DIR_RIGHT, TILE_WALL, 24, 10);
	_level->drawLine(DIR_DOWN, TILE_WALL, 229, 7);
	_level->drawLine(DIR_DOWN, TILE_WALL, 238, 7);

	_level->setTileAtIndex(TILE_TANK_ONE, kPlayerOneStartIndex);
	_level->setTileAtIndex(TILE_TURRET_ONE, kPlayerOneStartIndex + kLedDiagDownLeft);
	_level->setTileAtIndex(TILE_BULLET, kPlayerOneStartIndex + (kLedDiagDownLeft * 3));

	_level->setTileAtIndex(TILE_TANK_TWO, kPlayerTwoStartIndex);
	_level->setTileAtIndex(TILE_TURRET_TWO, kPlayerTwoStartIndex + kLedDiagDownLeft);
	_level->setTileAtIndex(TILE_BULLET, kPlayerTwoStartIndex + (kLedDiagDownLeft * 3));
#endif
	_selectElapsed = 0;
	_tankOneReady = false;
	_tankTwoReady = false;
}

void GameManager::updateSelect() {
	if (_hardwareManager->button()->wasClicked()) {
		DEBUG("Change theme!");
		_level->nextTheme();
	}

	if (_selectElapsed >= 10) {
		_selectElapsed = 0;
		if (!_tankOneReady && _hardwareManager->joystickOne()->clickUp()) {
			_tankOneReady = true;
			_timeElapsed = 0;
#ifdef CYLINDRUS
			_level->drawSquare(false, TILE_TANK_ONE, 552, 14, 17);
			_level->drawSquare(false, TILE_BACKGROUND, 369, 8, 5);

			_level->drawLine(DIR_RIGHT, TILE_WALL, 582, 15);
			_level->drawLine(DIR_RIGHT, TILE_WALL, 24, 15);
			_level->drawLine(DIR_DOWN, TILE_WALL, 582, 19);
			_level->drawLine(DIR_DOWN, TILE_WALL, 596, 19);

			_level->setTileAtIndex(TILE_TANK_ONE, kPlayerOneStartIndex);
			_level->setTileAtIndex(TILE_TURRET_ONE, kPlayerOneStartIndex + kLedLeft);
			_level->setTileAtIndex(TILE_BULLET, kPlayerOneStartIndex + (kLedLeft * 3));
#else
			_level->drawSquare(false, TILE_TANK_ONE, 198, 10, 5);
			_level->drawSquare(false, TILE_TANK_ONE, 178, 9, 4);
			_level->drawSquare(false, TILE_BACKGROUND, 158, 8, 3);
			_level->drawSquare(false, TILE_BACKGROUND, 138, 7, 2);

			_level->drawLine(DIR_RIGHT, TILE_WALL, 218, 11);
			_level->drawLine(DIR_RIGHT, TILE_WALL, 13, 11);
			_level->drawLine(DIR_DOWN, TILE_WALL, 218, 7);
			_level->drawLine(DIR_DOWN, TILE_WALL, 228, 7);

			_level->setTileAtIndex(TILE_TANK_ONE, kPlayerOneStartIndex);
			_level->setTileAtIndex(TILE_TURRET_ONE, kPlayerOneStartIndex + kLedDiagDownLeft);
			_level->setTileAtIndex(TILE_BULLET, kPlayerOneStartIndex + (kLedDiagDownLeft * 3));
#endif
		}
		if (!_tankTwoReady && _hardwareManager->joystickTwo()->clickUp()) {
			_tankTwoReady = true;
			_timeElapsed = 0;
#ifdef CYLINDRUS
			_level->drawSquare(false, TILE_TANK_TWO, 536, 14, 17);
			_level->drawSquare(false, TILE_BACKGROUND, 354, 8, 5);

			_level->drawLine(DIR_RIGHT, TILE_WALL, 566, 16);
			_level->drawLine(DIR_RIGHT, TILE_WALL, 8, 16);
			_level->drawLine(DIR_DOWN, TILE_WALL, 566, 19);
			_level->drawLine(DIR_DOWN, TILE_WALL, 581, 19);

			_level->setTileAtIndex(TILE_TANK_TWO, kPlayerTwoStartIndex);
			_level->setTileAtIndex(TILE_TURRET_TWO, kPlayerTwoStartIndex + kLedLeft);
			_level->setTileAtIndex(TILE_BULLET, kPlayerTwoStartIndex + (kLedLeft * 3));
#else
			_level->drawSquare(false, TILE_TANK_TWO, 209, 9, 5);
			_level->drawSquare(false, TILE_TANK_TWO, 189, 8, 4);
			_level->drawSquare(false, TILE_BACKGROUND, 169, 7, 3);
			_level->drawSquare(false, TILE_BACKGROUND, 149, 6, 2);

			_level->drawLine(DIR_RIGHT, TILE_WALL, 229, 10);
			_level->drawLine(DIR_RIGHT, TILE_WALL, 24, 10);
			_level->drawLine(DIR_DOWN, TILE_WALL, 229, 7);
			_level->drawLine(DIR_DOWN, TILE_WALL, 238, 7);

			_level->setTileAtIndex(TILE_TANK_TWO, kPlayerTwoStartIndex);
			_level->setTileAtIndex(TILE_TURRET_TWO, kPlayerTwoStartIndex + kLedDiagDownLeft);
			_level->setTileAtIndex(TILE_BULLET, kPlayerTwoStartIndex + (kLedDiagDownLeft * 3));
#endif
		}
		if (_tankOneReady && _tankTwoReady) {
			if (_timeElapsed >= 1000) {
				_stateController->changeState(STATE_PLAY);
			}
		}
	}
}

void GameManager::endSelect() {
	_level->clearLevel();
}

void GameManager::initPlay() {
	_tankOne = new Tank();
	_tankOne->initialize(TANK_ONE, kPlayerOneStartIndex);
	_tankTwo = new Tank();
	_tankTwo->initialize(TANK_TWO, kPlayerTwoStartIndex);
}

void GameManager::updatePlay() {
	if (_hardwareManager->button()->wasClicked()) {
		DEBUG("Change dim level!");
		_level->nextDimLevel();
	}

	_level->clearLevel();

	drawLevel(); // Place walls / obstacles.

	_tankOne->updateState();
	_tankTwo->updateState();

	drawObjects();
}

void GameManager::endPlay() {
	_level->clearLevel();
	_hardwareManager->ledSet()->setAllOff();
	_hardwareManager->ledSet()->updateLeds(_level);
}

void GameManager::initGameOver() {
	uint8_t oneLives = _tankOne->getLives();
	uint8_t twoLives = _tankTwo->getLives();

	//DEBUG("P1 Lives: %d, P2 Lives: %d", oneLives, twoLives);
	if (!oneLives && !twoLives) { // A Draw
		_textRenderer->newMessage(kDrawGame, kDrawGameLen, 5, 1, 61);
		_textTile = TILE_BULLET;
	} else if (!oneLives) { // Player One Loses
		_textRenderer->newMessage(kPlayerTwoWins, kPlayerTwoWinsLen, 5, 1, 61);
		_textTile = TILE_TANK_TWO;
	} else if (!twoLives) { // Player Two Loses
		_textRenderer->newMessage(kPlayerOneWins, kPlayerOneWinsLen, 5, 1, 61);
		_textTile = TILE_TANK_ONE;
	}

	_waitingElapsed = 0;
}

void GameManager::updateGameOver() {
	if (_textRenderer->isAnimating()) {
		if (_waitingElapsed >= 10) {
			_waitingElapsed = 0;
			_textRenderer->renderText(_textTile);
		}
	} else {
		_stateController->changeState(STATE_INIT);
	}
}

void GameManager::endGameOver() {
	_hardwareManager->joystickOne()->reset();
	_hardwareManager->joystickTwo()->reset();
}


void GameManager::drawLevel() {
#ifdef CYLINDRUS
	_level->drawLine(DIR_RIGHT, TILE_WALL, 569, 31);
	_level->drawLine(DIR_RIGHT, TILE_WALL, 0, 31);
	_level->drawLine(DIR_DOWN, TILE_WALL, 568, 5);
	_level->drawLine(DIR_DOWN, TILE_WALL, 409, 5);
	_level->drawLine(DIR_DOWN, TILE_WALL, 229, 4);
#else
	_level->drawLine(DIR_DOWN, TILE_WALL, 178, 2);
	_level->drawLine(DIR_DOWN, TILE_WALL, 123, 2);

	_level->drawLine(DIR_DOWN, TILE_WALL, 190, 2);
	_level->drawLine(DIR_DOWN, TILE_WALL, 135, 2);
#endif
}


void GameManager::drawObjects() {
	// Order of drawing is important!
	if (_tankOne->isVisible()) {
		_level->setTileAtIndex(TILE_TURRET_ONE, _tankOne->getTurretIndex());
	}

	if (_tankTwo->isVisible()) {
		_level->setTileAtIndex(TILE_TURRET_TWO, _tankTwo->getTurretIndex());
	}

	if (_tankOne->isVisible()) {
		_level->setTileAtIndex(TILE_TANK_ONE, _tankOne->getIndex());
	}

	if (_tankTwo->isVisible()) {
		_level->setTileAtIndex(TILE_TANK_TWO, _tankTwo->getIndex());
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