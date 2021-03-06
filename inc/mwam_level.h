#ifndef MWAM_LEVEL_H
#define MWAM_LEVEL_H

#include "mwam_constants.h"
#include "mwam_joystick.h"
#include "ElapsedTime.h"

namespace mwam
{

class Tank;

typedef enum Theme_t {
	THEME_ONE = 0,
	THEME_TWO,
	THEME_THREE
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
	TILE_BOUNDARY,
	TILE_KEEP_COLOR
} TileType;

typedef enum DimLevel_t {
	DIM_THREE_QUARTER = 0,
	DIM_HALF,
	DIM_QUARTER,
	DIM_NONE
} DimLevel;

class Level
{
	public:
		/* Public Methods */
		Level();
		void initialize(Theme aTheme, DimLevel aDimLevel, uint32_t aUpdateFreq = 0);
		void updateState();
		void clearLevel();
		void setTileAtIndex(TileType aTile, uint16_t aIndex);
		TileType getTileAtIndex(uint16_t aIndex);

		int16_t glyphIndexForChar(const char aChar);
		void drawSquare(bool aDiagonal, TileType aTile, uint16_t aIndex, uint8_t aWidth, uint8_t aHeight);
		void drawLine(Direction aDir, TileType aTile, uint16_t aIndex, uint8_t aLength);

		void nextTheme();
		void setTheme(Theme aTheme);
		void nextDimLevel();
		void setDimLevel(DimLevel aDimLevel);
		uint16_t getNewPosition(uint16_t aIndex, Direction aDir, TileType &aCollision);

		uint32_t getColorAtIndex(uint16_t aIndex);

		/* Public Variables */
		bool active;

	private:
		/* Private Methods */

		/* Private Variables */
		ElapsedMillis _timeElapsed;
		uint32_t _updateFreq;
		const Color* _activeTheme;
		//Color _activeTheme[7];
		Theme _currentTheme;
		DimLevel _dimLevel;
		TileType _levelTiles[kLedCount];

};

}

#endif