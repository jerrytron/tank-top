#ifndef MWAM_GAME_MANAGER_H
#define MWAM_GAME_MANAGER_H

#include "mwam_constants.h"
#include "mwam_level.h"
#include "mwam_tank.h"
#include "ElapsedTime.h"

namespace mwam
{

class StateController;
class DataManager;
class HardwareManager;
class TextRenderer;

class GameManager
{
	public:
		/* Public Methods */
		GameManager();
		void initialize(StateController *aStateController);
		void reset();

		void generateWalls();
		void updateIntro();
		void updateTutorial();

		void playLedTest();
		void playFireStormAnim();

		void initWaiting();
		void updateWaiting();
		void endWaiting();

		void initSelect();
		void updateSelect();
		void endSelect();

		void initPlay();
		void updatePlay();
		void endPlay();

		void initGameOver();
		void updateGameOver();
		void endGameOver();

		void drawLevel();
		void drawObjects();

		void gameOver();

		Level* getLevel();
		Tank* getTankOne();
		Tank* getTankTwo();
		Tank* getOtherTank(TankNumber aTankNum);

		/* Animation Accessors */

		/* Public Variables */

	private:
		/* Private Methods */

		/* Private Variables */
		StateController* _stateController;
		DataManager* _dataManager;
		HardwareManager* _hardwareManager;
		TextRenderer* _textRenderer;

		ElapsedMillis _waitingElapsed;
		ElapsedMillis _selectElapsed;
		ElapsedMillis _timeElapsed;
		uint8_t _tileIndex;

		Level* _level;
		Tank* _tankOne;
		bool _tankOneReady;
		Tank* _tankTwo;
		bool _tankTwoReady;

		TileType _textTile;

		bool _playerOneActive;
		bool _playerTwoActive;

		//uint8_t _fireStormAnimStep;

		//Animation _testAnim[5];
		//Animation _fireStormAnimSeries[5];
		/*Animation _playerHitAnimSeries[kPlayHitAnimLength];
		Animation _disruptorAnimSeries[kDisruptorAnimLength];
		Animation _destroyAnimSeries[kDestroyAnimLength];
		Animation _bombAnimSeries[kBombAnimLength];*/

};

}

#endif