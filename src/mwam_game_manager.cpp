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
}

void GameManager::updateIntro() {
}

void GameManager::updateTutorial() {
}

void GameManager::updatePlay() {
}

void GameManager::updateGameOver(bool aWonGame) {
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