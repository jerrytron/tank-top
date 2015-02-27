#ifndef MWAM_GAME_MANAGER_H
#define MWAM_GAME_MANAGER_H

#include "mwam_constants.h"
#include "mwam_led_set.h"
#include "ElapsedTime.h"

namespace mwam
{

class StateController;
class DataManager;
class HardwareManager;

typedef enum BogeyType_t {
	B_NONE = 0,
	B_BURNING,
	B_LIGHT,
	B_MEDIUM,
	B_HEAVY,
	B_BOSS,
	B_DISRUPTOR,
	B_MISSILE
} BogeyType;

static struct BogeySpecs_t {
	BogeyType type;
	uint16_t maxHealth; // How many hits to destroy.
	uint16_t timeToArrive; // Milliseconds until arrival.
	uint16_t weaponRefresh; // Milliseconds until enemy weapon can refire.
} BogeySpecs[] = {
	{ B_NONE, 0, 0, 0 },
	{ B_BURNING, 0, 0, 0 },
	{ B_LIGHT, 1, 12000, 0 },
	{ B_MEDIUM, 1, 14000, 5000 },
	{ B_HEAVY, 2, 18000, 5000 },
	{ B_BOSS, 3, 0, 5000 },
	{ B_DISRUPTOR, 1, 6000, 0},
	{ B_MISSILE, 1, 6000, 0 }
};

typedef struct Bogey_t {
	BogeyType type;
	//uint16_t ledIndex = 0; // 0 = Place bogey at any available & valid position. 1-9 = Position at led index of value.
	uint16_t health;
	uint16_t deployDelay; // 0 = Immediately, milliseconds from beginning of set of bogeys in a wave.
	uint8_t movement; // How likely bogey is to move at interval. 0 = will not, 10 = will for sure.
	uint16_t movementInterval; // How many milliseconds until a bogey decides if it should shift position.
	ElapsedMillis delayElapsed = 0;
	ElapsedMillis travelElapsed = 0;
	ElapsedMillis weaponElapsed = 0;
	ElapsedMillis movementElapsed = 0;

	// Default Constructor
	Bogey_t()
		: type(B_NONE), deployDelay(0), movement(0), movementInterval(0) {health = 0;}
	Bogey_t(BogeyType aType)
		: type(aType), deployDelay(0), movement(0), movementInterval(0) {health = BogeySpecs[aType].maxHealth;}
	Bogey_t(BogeyType aType, uint16_t aDeployDelay)
		: type(aType), deployDelay(aDeployDelay), movement(0), movementInterval(0) {health = BogeySpecs[aType].maxHealth;}
	Bogey_t(BogeyType aType, uint16_t aDeployDelay, uint8_t aMovement, uint16_t aMovementInterval)
		: type(aType), deployDelay(aDeployDelay), movement(aMovement), movementInterval(aMovementInterval) {health = BogeySpecs[aType].maxHealth;}
} Bogey;


	// Default Constructor
	/*Bogey_t() {}
	Bogey_t(BogeyType aType, uint16_t aHealth)
		: type(aType), health(aHealth) {}
	Bogey_t(BogeyType aType, uint16_t aHealth, uint16_t aDeployDelay)
		: type(aType), health(aHealth), deployDelay(aDeployDelay) {}
	Bogey_t(BogeyType aType, uint16_t aHealth, uint16_t aDeployDelay, uint8_t aMovement, uint16_t aMovementInterval)
		: type(aType), health(aHealth), deployDelay(aDeployDelay), movement(aMovement), movementInterval(aMovementInterval) {}
maxHealth(B_LIGHT)*/
	//uint16_t timeToArrive = 0;
	//uint8_t health = 1;
	//uint8_t width = 1;
/*struct T {
   static const unsigned int array[10];
};

const unsigned int T::array[10] = {0,1,2,3,4,5,6,7,8,9};*/


/*static Bogey wave[10][3][] = {
	{ // Wave 1
		{ // Set 1
			Bogey(BOGEY_LIGHT, EXPLICIT, 6, 0)
		},
		{
			Bogey(BOGEY_LIGHT, EXPLICIT, 6, 0), Bogey(BOGEY_LIGHT, EXPLICIT, 6, 0) },
	},
	{ // Wave 1

	}
};*/

class GameManager
{
	public:
		/* Public Methods */
		GameManager();
		void initialize(StateController *aStateController);
		void reset();
		void scoreDataStart();
		void scoreDataEnd(bool aGameWon);

		//void updateState();
		void updateIntro();
		void updateTutorial();
		void updatePlay();
		void updateGameOver(bool aWonGame);
		void updateHighScore();
		//void updateGameWon();

		//void nextIntroStep();
		uint8_t dialToIndex(uint8_t aDialed);
		uint8_t indexToDial(uint8_t aLedIndex);
		uint16_t currentWave();
		uint8_t currentSet();

		void refreshRadar();
		bool radarOff();
		void displayScore();
		uint8_t bogeyCount();
		void resetBogeys();
		bool fireWeapon(int8_t aDialValue);
		void detonateBomb();
		void setupNextSet();
		void endWave();

		void playRadarAnim();
		void playMissAnim(uint16_t aLedIndex);
		void playHitAnim(uint16_t aLedIndex);
		void playDestroyAnim(uint16_t aLedIndex);
		void playBombAnim();
		void playPlayerHitAnim();
		void playPlayerDeathAnim();
		void playRadarDisruptor();

		/* Animation Accessors */
		Animation sweepGrnAnim();
		Animation sweepRedAnim();
		//Animation* bombAnimSeries();

		/* Public Variables */

	private:
		/* Private Methods */
		bool setupWaveOne(uint8_t aSet, uint8_t aIteration);
		bool setupWaveTwo(uint8_t aSet, uint8_t aIteration);
		bool setupWaveThree(uint8_t aSet, uint8_t aIteration);
		bool setupWaveFour(uint8_t aSet, uint8_t aIteration);
		bool setupWaveFive(uint8_t aSet, uint8_t aIteration);
		bool setupWaveSix(uint8_t aSet, uint8_t aIteration);
		bool setupWaveSeven(uint8_t aSet, uint8_t aIteration);
		bool setupWaveEight(uint8_t aSet, uint8_t aIteration);
		bool setupWaveNine(uint8_t aSet, uint8_t aIteration);
		bool setupWaveTen(uint8_t aSet, uint8_t aIteration);
		void incrementKills(BogeyType aBogeyType);
		uint16_t maxHealth(BogeyType aType);
		uint16_t timeToArrive(BogeyType aType);
		uint16_t weaponRefresh(BogeyType aType);
		uint16_t randomPosition();
		uint16_t randomPosition(uint16_t aLower, uint16_t aHigher);
		//void initAnimations();

		/* Private Variables */
		StateController* _stateController;
		DataManager* _dataManager;
		HardwareManager* _hardwareManager;
		ScoreData _playerScore;
		char* _initialsBuffer;
		uint8_t _initialDialed;
		uint8_t _initialIndex;
		char _initialChar;
		uint8_t _charOffset;
		//uint8_t _playerStanding;
		uint8_t _introStep;
		uint8_t _tutorialStep;
		uint8_t _gameOverStep;
		uint8_t _highScoreStep;
		//uint8_t _gameWonStep;

		uint8_t _playerHealth;
		uint16_t _currentWave;
		uint8_t _currentSet;
		uint8_t _bogeysLeft;
		uint8_t _bombs;
		bool _bombing;
		bool _setOver;
		bool _takingHit;
		bool _disruptorHit;
		ElapsedMillis _disruptorElapsed;
		Bogey _bogeys[9];

		//uint8_t _radarBrightness;
		//ElapsedMillis _radarElapsed;

		// Animation Presets
		//Animation _sweepGrnAnim;
		//Animation _sweepRedAnim;
		//Animation _bossShipAnim;
		//Animation _missAnim;
		//Animation _hitAnim;
		Animation _playerHitAnimSeries[kPlayHitAnimLength];
		Animation _disruptorAnimSeries[kDisruptorAnimLength];
		Animation _destroyAnimSeries[kDestroyAnimLength];
		Animation _bombAnimSeries[kBombAnimLength];

};

}

#endif