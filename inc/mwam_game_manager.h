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

class GameManager
{
	public:
		/* Public Methods */
		GameManager();
		void initialize(StateController *aStateController);
		void reset();

		//void updateState();
		void updateIntro();
		void updateTutorial();
		void updatePlay();
		void updateGameOver(bool aWonGame);

		/* Animation Accessors */

		/* Public Variables */

	private:
		/* Private Methods */

		/* Private Variables */
		StateController* _stateController;
		DataManager* _dataManager;
		HardwareManager* _hardwareManager;

		Animation _testAnim[5];
		/*Animation _playerHitAnimSeries[kPlayHitAnimLength];
		Animation _disruptorAnimSeries[kDisruptorAnimLength];
		Animation _destroyAnimSeries[kDestroyAnimLength];
		Animation _bombAnimSeries[kBombAnimLength];*/

};

}

#endif