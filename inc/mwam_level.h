#ifndef MWAM_LEVEL_H
#define MWAM_LEVEL_H

#include "mwam_constants.h"
#include "mwam_joystick.h"
#include "ElapsedTime.h"

namespace mwam
{

class Tank;

typedef enum Theme_t {
	THEME_DEFAULT = 0
} Theme;

typedef enum ThemeElement_t {
	BACKGROUND = 0,
	P1_BODY = 0,
	P1_TURRET,
	P2_BODY,
	P2_TURRET,
	BULLETS,
	WALLS
} ThemeElement;

typedef enum TileType_t {
	TILE_BACKGROUND = 0,
	TILE_TANK_ONE,
	TILE_TURRET_ONE,
	TILE_TANK_TWO,
	TILE_TURRET_TWO,
	TILE_BULLET,
	TILE_WALL,
	TILE_BOUNDARY
} TileType;

/*typedef enum Collision_t {
	COL_NONE = 0,
	COL_BOUNDS,
	COL_WALL,
	COL_BULLET,
} Collision;*/

class Level
{
	public:
		/* Public Methods */
		Level();
		void initialize(Theme aTheme, uint32_t aUpdateFreq = 0);
		void updateState();
		void setTankAtIndex(Tank* aTank);
		void setTileAtIndex(TileType aTile, uint16_t aIndex);

		void setTheme(Theme aTheme);
		uint16_t updatePosition(uint16_t aIndex, Direction aDir, TileType &aCollision);

		TileType getTileAtIndex(uint16_t aIndex);
		uint32_t getColorAtIndex(uint16_t aIndex);

		/* Public Variables */
		bool active;

	private:
		/* Private Methods */
		TileType checkForCollision(uint16_t aIndex);

		/* Private Variables */
		ElapsedMillis _timeElapsed;
		uint32_t _updateFreq;
		const Color* _activeTheme;
		TileType _levelTiles[kLedCount];

};

}

#endif