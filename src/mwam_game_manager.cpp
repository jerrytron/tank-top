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
	_tankOne->initialize(TANK_ONE, _level, kPlayerOneStartIndex, kIntervalPlayerDelayMillis);
	_tankTwo = new Tank();
	_tankTwo->initialize(TANK_TWO, _level, kPlayerTwoStartIndex, kIntervalPlayerDelayMillis);

	_hardwareManager->ledSet()->setFastUpdates(_tankOne, _tankTwo);
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

	Direction dir = _hardwareManager->joystickOne()->getDirection();
	JoystickThreshold threshold = _hardwareManager->joystickOne()->getThreshold();
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
	/*Animation b = Animation();
	b.startColor = Color(0, 0, 0);
	b.endColor = Color(0, 0, 127);
	b.repeats = 0;
	b.yoyo = false;
	b.tweenTime = 1000;
	b.ease = EASE_QUAD_IN_OUT;

	Animation c = Animation();
	c.startColor = Color(0, 0, 0);
	c.endColor = Color(0, 0, 127);
	c.repeats = 0;
	c.yoyo = false;
	c.tweenTime = 1000;
	c.ease = EASE_QUAD_IN_OUT;*/
}

/*void GameManager::playFireStormAnim() {
	Animation bomb1 = Animation();
	bomb1.endColor = Color(200, 0, 0); // Red
	bomb1.tweenTime = 500;
	bomb1.ease = EASE_LINEAR;

	Animation bomb2 = Animation();
	bomb2.startColor = Color(200, 0, 0);
	bomb2.endColor = Color(200, 200, 0); // Yellow
	bomb2.tweenTime = 250;
	bomb2.ease = EASE_QUAD_IN_OUT;
	bomb2.yoyo = true;
	bomb2.repeats = 2;

	Animation bomb3 = Animation();
	bomb3.startColor = Color(200, 200, 0);
	bomb3.endColor = Color(255, 102, 0); // Orange
	bomb3.tweenTime = 250;
	bomb3.ease = EASE_QUAD_IN_OUT;
	bomb3.yoyo = true;
	bomb3.repeats = 2;

	Animation bomb4 = Animation();
	bomb4.startColor = Color(255, 102, 0);
	bomb4.endColor = Color(255, 255, 255); // White
	bomb4.tweenTime = 500;
	bomb4.ease = EASE_QUAD_IN_OUT;
	bomb4.repeats = 0;

	Animation bomb5 = Animation();
	bomb5.startColor = Color(255, 255, 255);
	bomb5.endColor = Color(0, 0, 0);
	bomb5.tweenTime = 700;
	bomb5.ease = EASE_LINEAR;

	//Animation bombAnimSeries[kBombAnimLength] = { bomb1, bomb2, bomb3, bomb4, bomb5 };

	_fireStormAnimSeries[0] = bomb1;
	_fireStormAnimSeries[1] = bomb2;
	_fireStormAnimSeries[2] = bomb3;
	_fireStormAnimSeries[3] = bomb4;
	_fireStormAnimSeries[4] = bomb5;

	for (int16_t i = 0; i < _hardwareManager->ledSet()->ledCount(); ++i) {
		_fireStormAnimSeries[0].delayTime = random(0, 500);
		_fireStormAnimSeries[1].repeats = random(2, 6);
		_fireStormAnimSeries[2].repeats = random(2, 6);
		_hardwareManager->ledSet()->animateSeries(i, _fireStormAnimSeries, 5, 0);
	}
}*/

void GameManager::updateAnimations() {
	if (1) {
		/*delay(10);
		for (int16_t i = 0; i < _hardwareManager->ledSet()->ledCount(); ++i) {
			_hardwareManager->ledSet()->setColor(i, Color(random(0, 256), random(0, 256), random(0, 256)));
		}*/
		_hardwareManager->ledSet()->setColor(random(0, 240), Color(random(0, 127), random(0, 127), random(0, 127)));
		//updateFireStormAnim();
	}
}

/*void GameManager::updateFireStormAnim() {
	if (_fireStormAnimStep == 0) {
		if (_hardwareManager->ledSet()->allLedsIdle()) {
			Animation a = Animation();
			a.endColor = Color(200, 0, 0); // Red
			a.tweenTime = 500;
			a.ease = EASE_LINEAR;
			_hardwareManager->ledSet()->animateAll(a, true, 0, false);
			_fireStormAnimStep++;
		}
	} else if (_fireStormAnimStep == 1) {
		if (_hardwareManager->ledSet()->allLedsIdle()) {
			Animation a = Animation();
			a.startColor = Color(200, 0, 0);
			a.endColor = Color(200, 200, 0); // Yellow
			a.tweenTime = 250;
			a.ease = EASE_QUAD_IN_OUT;
			a.yoyo = true;
			a.repeats = 2;
			_hardwareManager->ledSet()->animateAll(a, true, 0, false);
			_fireStormAnimStep++;
		}
	} else if (_fireStormAnimStep == 2) {
		if (_hardwareManager->ledSet()->allLedsIdle()) {
			Animation a = Animation();
			a.startColor = Color(200, 200, 0);
			a.endColor = Color(255, 102, 0); // Orange
			a.tweenTime = 250;
			a.ease = EASE_QUAD_IN_OUT;
			a.yoyo = true;
			a.repeats = 2;
			_hardwareManager->ledSet()->animateAll(a, true, 0, false);
			_fireStormAnimStep++;
		}
	} else if (_fireStormAnimStep == 3) {
		if (_hardwareManager->ledSet()->allLedsIdle()) {
			Animation a = Animation();
			a.startColor = Color(255, 102, 0);
			a.endColor = Color(255, 255, 255); // White
			a.tweenTime = 500;
			a.ease = EASE_QUAD_IN_OUT;
			_hardwareManager->ledSet()->animateAll(a, true, 0, false);
			_fireStormAnimStep++;
		}
	} else if (_fireStormAnimStep == 4) {
		if (_hardwareManager->ledSet()->allLedsIdle()) {
			Animation a = Animation();
			a.startColor = Color(255, 255, 255);
			a.endColor = Color(0, 0, 0);
			a.tweenTime = 700;
			a.ease = EASE_LINEAR;
			_hardwareManager->ledSet()->animateAll(a, true, 0, false);
			_fireStormAnimStep++;
		}
	}
}*/

Level* GameManager::getLevel() {
	return _level;
}

/*void GameManager::playMissAnim(uint16_t aLedIndex) {
	Animation a = Animation();
	a.endColor = Color(0, 0, 127);
	a.repeats = 1;
	a.yoyo = true;
	a.tweenTime = kFireMissAnimMillis;
	a.ease = EASE_QUAD_IN_OUT;
	_hardwareManager->ledSet()->animateLed(aLedIndex, a, false);
}

void GameManager::playPlayerHitAnim() {
	Animation playerHit1 = Animation();
	playerHit1.endColor = Color(127, 0, 0);
	playerHit1.tweenTime = 300;
	playerHit1.ease = EASE_QUAD_OUT;

	Animation playerHit2 = Animation();
	playerHit2.startColor = Color(127, 0, 0);
	playerHit2.tweenTime = 300;
	playerHit2.ease = EASE_QUAD_IN;

	//Animation playerHitAnimSeries[kPlayHitAnimLength] = { playerHit1, playerHit2 };

	_playerHitAnimSeries[0] = playerHit1;
	_playerHitAnimSeries[1] = playerHit2;

	for (int16_t i = 0; i < _hardwareManager->ledSet()->ledCount(); ++i) {
		_hardwareManager->ledSet()->animateSeries(i, _playerHitAnimSeries, kPlayHitAnimLength, 0, true);
	}
}

void GameManager::playPlayerDeathAnim() {
	_hardwareManager->ledSet()->animateAll(sweepRedAnim(), false, kRadarAnimStaggerMillis);
}

void GameManager::playRadarDisruptor() {
	Animation disruptor1 = Animation();
	disruptor1.endColor = Color(0, 100, 0);
	disruptor1.tweenTime = 100;

	Animation disruptor2 = Animation();
	disruptor2.startColor = Color(0, 100, 0);
	disruptor2.tweenTime = 200;

	Animation disruptor3 = Animation();
	disruptor3.startColor = Color(0, 100, 0);
	disruptor3.tweenTime = 100;

	//Animation disruptorAnimSeries[kDisruptorAnimLength] = { disruptor1, disruptor2, disruptor3 };

	_disruptorAnimSeries[0] = disruptor1;
	_disruptorAnimSeries[1] = disruptor2;
	_disruptorAnimSeries[2] = disruptor3;

	for (int16_t i = 0; i < _hardwareManager->ledSet()->ledCount(); ++i) {
		_disruptorAnimSeries[0].delayTime = random(0, 250);
		_disruptorAnimSeries[1].ease = (EaseType)random(0, 2);
		_disruptorAnimSeries[1].tweenTime = random(kDisruptorMinAnimMillis, (kDisruptorMaxAnimMillis - _disruptorAnimSeries[0].delayTime));
		_disruptorAnimSeries[1].repeats = random(1, 4);
		_disruptorAnimSeries[2].ease = (EaseType)random(0, 2);
		_disruptorAnimSeries[2].tweenTime = kDisruptorMinAnimMillis;
		_hardwareManager->ledSet()->animateSeries(i, _disruptorAnimSeries, kDisruptorAnimLength, 0, true);
	}
}*/

/* Animation Accessors */


/* Private Methods */

}