#ifndef MWAM_BULLET_H
#define MWAM_BULLET_H

#include "mwam_constants.h"
#include "mwam_joystick.h"
#include "mwam_level.h"
#include "ElapsedTime.h"

namespace mwam
{

class GameManager;

typedef enum BulletState_t {
	BULLET_INIT = 0,
	BULLET_AVAILABLE,
	BULLET_ACTIVE,
	BULLET_BOUNCE,
	BULLET_HIT,
	BULLET_EXPLODE,
	BULLET_EXPIRE
} BulletState;

class Bullet
{
	public:
		/* Public Methods */
		Bullet();
		void initialize(uint32_t aMovementDelay);
		void reset(uint16_t aIndex, Direction aDir);

		BulletState getState();
		void changeState(BulletState aState);
		void updateState();

		uint16_t getIndex();
		void setIndex(uint16_t aIndex);
		uint16_t getLastIndex();
		//TileType getOverlapTile();
		//TileType getLastOverlapTyle();

		/* Public Variables */

	private:
		/* Private Methods */
		void initState(BulletState aState);
		void loopState(BulletState aState);
		void endState(BulletState aState);

		void updateMovement();
		bool bounceBullet();

		/* Private Variables */
		ElapsedMillis _timeElapsed;
		GameManager* _gameManager;
		BulletState _state;
		uint32_t _movementDelay;
		uint16_t _index;
		uint16_t _lastIndex;
		Direction _direction;
		//TileType _overlapTile;
		//TileType _lastOverlapTile;
		uint16_t _bouncesLeft;

};

}

#endif