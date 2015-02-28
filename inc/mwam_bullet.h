#ifndef MWAM_BULLET_H
#define MWAM_BULLET_H

#include "mwam_constants.h"
#include "mwam_joystick.h"
#include "ElapsedTime.h"

namespace mwam
{

class Bullet
{
	public:
		/* Public Methods */
		Bullet();
		void initialize(uint16_t aIndex, Direction aDir, uint32_t aMovementDelay);
		void updateState();

		uint16_t getIndex();
		void setIndex(uint16_t aIndex);
		uint16_t getLastIndex();

		/* Public Variables */
		bool active;

	private:
		/* Private Methods */

		/* Private Variables */
		ElapsedMillis _timeElapsed;
		uint32_t _movementDelay;
		uint16_t _index;
		uint16_t _lastIndex;
		Direction _direction;

};

}

#endif