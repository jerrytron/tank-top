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

typedef enum Theme_t {
	DEFAULT = 0
} Theme;

class GameManager
{
	public:
		/* Public Methods */
		GameManager();
		void initialize(StateController *aStateController);
		void reset();

		void setupLevel(Theme aTheme);

		void updateIntro();
		void updateTutorial();
		void updatePlay();
		void updateGameOver(bool aWonGame);

		void playLedTest();
		void playFireStormAnim();

		void updateAnimations();
		void updateFireStormAnim();

		/* Animation Accessors */

		/* Public Variables */

	private:
		/* Private Methods */

		/* Private Variables */
		StateController* _stateController;
		DataManager* _dataManager;
		HardwareManager* _hardwareManager;

		const Color* _activeTheme;

		uint8_t _fireStormAnimStep;

		Animation _testAnim[5];
		Animation _fireStormAnimSeries[5];
		/*Animation _playerHitAnimSeries[kPlayHitAnimLength];
		Animation _disruptorAnimSeries[kDisruptorAnimLength];
		Animation _destroyAnimSeries[kDestroyAnimLength];
		Animation _bombAnimSeries[kBombAnimLength];*/

};

}

#endif