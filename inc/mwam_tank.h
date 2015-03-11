#ifndef MWAM_TANK_H
#define MWAM_TANK_H

#include "mwam_constants.h"
#include "mwam_bullet.h"
#include "mwam_joystick.h"
#include "mwam_level.h"
#include "ElapsedTime.h"

namespace mwam
{

class Level;

typedef enum TankNumber_t {
	TANK_ONE = 1,
	TANK_TWO
} TankNumber;

typedef enum TankState_t {
	TANK_WAITING = 0,
	TANK_READY,
	TANK_ACTIVE,
	TANK_INVULNERABLE,
	TANK_HIT,
	TANK_DESTROYED,
	TANK_BURNING
} TankState;

class Tank
{
	public:
		/* Public Methods */
		Tank();
		void initialize(TankNumber aTankNum, Level* aLevel, uint8_t aStartIndex, uint32_t aMovementFreq);
		void reset();
		void updateState(Direction aDirection, uint16_t aDelayReduction);
		void updateBullets();
		bool fireBullet();

		TankState getState();
		void setState(TankState aState);
		TankNumber getTankNumber();
		uint16_t getIndex();
		void setIndex(uint16_t aIndex);
		uint16_t getLastIndex();
		TileType getLastOverlapTile();
		uint8_t getBulletCount();
		Bullet* getBulletAtIndex(uint8_t aIndex);
		uint16_t getTurretIndex();
		uint16_t getLastTurretIndex();
		uint16_t findTurretIndex();
		TileType getTurretOverlapTile();
		TileType getLastTurretOverlapTile();

		/* Public Variables */
		bool active;

	private:
		/* Private Methods */

		/* Private Variables */
		ElapsedMillis _timeElapsed;
		Level* _level;
		Direction _direction;
		uint32_t _movementFreq;
		TankState _tankState;
		TankNumber _tankNumber;
		uint16_t _startIndex;
		uint16_t _index;
		uint16_t _lastIndex;
		TileType _overlap;
		TileType _lastOverlap;
		uint16_t _turretIndex;
		uint16_t _lastTurretIndex;
		TileType _turretOverlap;
		TileType _lastTurretOverlap;
		uint8_t _health;
		uint8_t _bulletCount;
		Bullet _bullets[kMaxBulletsLive];

};

}

#endif