#ifndef MWAM_TANK_H
#define MWAM_TANK_H

#include "mwam_constants.h"
#include "mwam_bullet.h"
#include "mwam_joystick.h"
#include "mwam_level.h"
#include "mwam_led_set.h"
#include "ElapsedTime.h"

namespace mwam
{

class GameManager;
class HardwareManager;
class Joystick;

typedef enum TankNumber_t {
	TANK_ONE = 1,
	TANK_TWO
} TankNumber;

typedef enum TankState_t {
	TANK_INIT = 0,
	TANK_RESETTING,
	TANK_ACTIVE,
	TANK_HIT,
	TANK_DESTROYED,
	TANK_GAME_OVER
} TankState;

class Tank
{
	public:
		/* Public Methods */
		Tank();
		void initialize(TankNumber aTankNum, uint8_t aStartIndex);
		const char* stateString();
		void reset();

		TankState getState();
		void changeState(TankState aState);
		void updateState();

		bool checkCollisionAtIndex(TileType aSource, uint16_t aIndex, TileType &aTile, bool aInvulnerable = false);
		bool checkBulletCollision(TileType aSource, uint16_t aIndex, bool aInvulnerable = false);

		TankNumber getTankNumber();
		uint16_t getIndex();
		void setIndex(uint16_t aIndex);
		uint8_t getLives();
		uint8_t getBulletCount();
		Bullet* getBulletAtIndex(uint8_t aIndex);
		uint16_t getTurretIndex();
		uint16_t getLastTurretIndex();
		bool isVisible();
		bool isInvulnerable();

		/* Public Variables */
		bool active;

	private:
		/* Private Methods */
		void initState(TankState aState);
		void loopState(TankState aState);
		void endState(TankState aState);
		void updateMovement(Direction aDirection, uint16_t aMovementFreq);
		uint16_t findTurretIndex();
		void updateBullets();
		bool fireBullet();

		/* Private Variables */
		GameManager* _gameManager;
		HardwareManager* _hardwareManager;

		Joystick* _joystick;

		ElapsedMillis _moveElapsed;
		ElapsedMillis _resetElapsed;

		// For resets, how many Tank Blinks have occurred.
		uint8_t _blinks;
		bool _visible;
		bool _invulnerable;

		Direction _direction;
		uint16_t _movementFreq;
		TankState _state;
		TankNumber _tankNumber;
		TileType _tankTile;
		TileType _turretTile;
		uint16_t _startIndex;
		uint16_t _index;
		uint16_t _turretIndex;
		uint16_t _lastTurretIndex;
		uint8_t _health;
		uint8_t _lives;
		uint8_t _bulletCount;
		Bullet _bullets[kMaxBulletsLive];

};

}

#endif