#ifndef MWAM_TANK_H
#define MWAM_TANK_H

#include "mwam_constants.h"
#include "mwam_bullet.h"
#include "mwam_joystick.h"
#include "ElapsedTime.h"

namespace mwam
{

class Level;

class Tank
{
	public:
		/* Public Methods */
		Tank();
		void initialize(Level* aLevel, uint8_t aStartIndex, uint32_t aMovementFreq);
		void reset();
		void updateState(Direction aDirection, uint16_t aDelayReduction);
		bool fireBullet();

		uint16_t getIndex();
		void setIndex(uint16_t aIndex);
		uint16_t getLastIndex();
		uint8_t getBulletCount();
		Bullet* getBulletAtIndex(uint8_t aIndex);

		/* Public Variables */
		bool active;

	private:
		/* Private Methods */
		void updateBullets();

		/* Private Variables */
		ElapsedMillis _timeElapsed;
		Level* _level;
		Direction _direction;
		uint32_t _movementFreq;

		uint16_t _startIndex;
		uint16_t _index;
		uint16_t _lastIndex;
		uint8_t _health;
		uint8_t _bulletCount;
		Bullet _bullets[kMaxBulletsLive];

};

}

#endif