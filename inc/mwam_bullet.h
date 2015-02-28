#ifndef MWAM_BULLET_H
#define MWAM_BULLET_H

#include "mwam_constants.h"
#include "mwam_joystick.h"
#include "mwam_level.h"
#include "ElapsedTime.h"

namespace mwam
{

class Bullet
{
	public:
		/* Public Methods */
		Bullet();
		void initialize(Level* aLevel, uint32_t aMovementDelay);
		void reset(uint16_t aIndex, Direction aDir);
		void updateState();

		uint16_t getIndex();
		void setIndex(uint16_t aIndex);
		uint16_t getLastIndex();
		TileType getOverlapTile();
		TileType getLastOverlapTyle();

		/* Public Variables */
		bool collided;
		bool endOfLife;

	private:
		/* Private Methods */
		bool bounceBullet();

		/* Private Variables */
		ElapsedMillis _timeElapsed;
		uint32_t _movementDelay;
		Level* _level;
		uint16_t _index;
		uint16_t _lastIndex;
		Direction _direction;
		TileType _overlapTile;
		TileType _lastOverlapTile;
		uint16_t _bouncesLeft;

};

}

#endif