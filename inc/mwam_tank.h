#ifndef MWAM_TANK_H
#define MWAM_TANK_H

#include "mwam_constants.h"
#include "mwam_bullet.h"
#include "ElapsedTime.h"

namespace mwam
{

class Tank
{
	public:
		/* Public Methods */
		Tank();
		void initialize(uint32_t aUpdateFreq, uint8_t aStartIndex);
		void reset();
		void updateState();

		/* Public Variables */
		bool active;

	private:
		/* Private Methods */
		uint16_t _startIndex;
		uint16_t _index;
		uint8_t _health;
		uint8_t _bulletCount;
		Bullet _bullets[kMaxBulletsLive];

		/* Private Variables */
		ElapsedMillis _timeElapsed;
		uint32_t _updateFreq;


};

}

#endif