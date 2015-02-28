#include "mwam_game_manager.h"
#include "mwam_manager.h"

extern char* itoa(int a, char* buffer, unsigned char radix);

namespace mwam
{

const Color kColorGreen = Color(0, 255, 0);
const Color kColorYellow = Color(255, 255, 0);
const Color kColorOrange = Color(255, 140, 0);
const Color kColorRed = Color(255, 0, 0);
const Color kColorBlack = Color(0, 0, 0);
const Color kColorWhite = Color(255, 255, 255);
const Color kColorBlue = Color(0, 0, 255);
const Color kColorPurple = Color(255, 0, 255);
// Themes: P1 Color, P1 Turret, P2 Color, P2 Turret, Bullets, Background, Walls
const Color themeDefault[] = { kColorGreen, kColorRed, kColorBlue, kColorRed, kColorYellow, kColorBlack, kColorOrange };


/* Public Methods */

GameManager::GameManager() {
}

void GameManager::initialize(StateController *aStateController) {
	_stateController = aStateController;
	_dataManager = Manager::getInstance().dataManager;
	_hardwareManager = Manager::getInstance().hardwareManager;
}

void GameManager::reset() {
	_hardwareManager->button()->resetButton();
	_hardwareManager->ledSet()->resetLeds();

	_fireStormAnimStep = 0;
}

void GameManager::setupLevel(Theme aTheme) {
	if (aTheme == DEFAULT) {
		_activeTheme = themeDefault;
	}
}

void GameManager::updateIntro() {
}

void GameManager::updateTutorial() {
}

void GameManager::updatePlay() {
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

void GameManager::playFireStormAnim() {
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
}

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

void GameManager::updateFireStormAnim() {
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