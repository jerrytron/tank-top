#include "mwam_level.h"
#include "mwam_tank.h"

namespace mwam
{

/*struct ThemeSet_t {
	Theme themeIndex;
} Themes[] = {
	{ THEME_DEFAULT, kColorGreen, kColorRed, kColorBlue, kColorRed, kColorYellow, kColorBlack, kColorOrange }
};*/

// Themes: Background, P1 Color, P1 Turret, P2 Color, P2 Turret, Bullets, Walls
const Color themeDefault[] = { kColorBlack, kColorGreen, kColorRed, kColorBlue, kColorRed, kColorYellow, kColorOrange };

const Color themeBright[] = { kColorWhite, kColorBlue, kColorYellow, kColorRed, kColorYellow, kColorOrange, kColorBlack };


/* Public Methods */

Level::Level() {
}

void Level::initialize(Theme aTheme, uint32_t aUpdateFreq) {
	_updateFreq = aUpdateFreq;
	_timeElapsed = 0;
	setTheme(aTheme);
	memset(_levelTiles, 0, kLedCount);
}

void Level::updateState() {
	if (this->active && (_timeElapsed >= _updateFreq)) {
		_timeElapsed = 0;
	}
}

void Level::setTankAtIndex(Tank* aTank) {
	if (aTank->getTankNumber() == TANK_ONE) {
		setTileAtIndex(TILE_BACKGROUND, aTank->getLastIndex());
		setTileAtIndex(aTank->getLastTurretOverlapTile(), aTank->getLastTurretIndex());
		DEBUG("Cleanup indexes: %d & %d", aTank->getLastIndex(), aTank->getLastTurretIndex());
		setTileAtIndex(TILE_TANK_ONE, aTank->getIndex());
		setTileAtIndex(TILE_TURRET_ONE, aTank->getTurretIndex());
		DEBUG("New indexes: %d & %d", aTank->getIndex(), aTank->getTurretIndex());
	} else if (aTank->getTankNumber() == TANK_TWO) {
		setTileAtIndex(TILE_BACKGROUND, aTank->getLastIndex());
		setTileAtIndex(aTank->getLastTurretOverlapTile(), aTank->getLastTurretIndex());
		setTileAtIndex(TILE_TANK_TWO, aTank->getIndex());
		setTileAtIndex(TILE_TURRET_TWO, aTank->getTurretIndex());
	}
}

void Level::setTileAtIndex(TileType aTile, uint16_t aIndex) {
	_levelTiles[aIndex] = aTile;
}

TileType Level::getTileAtIndex(uint16_t aIndex) {
	return _levelTiles[aIndex];
}

void Level::nextTheme() {
	if (_currentTheme == THEME_BRIGHT) {
		_currentTheme = THEME_DEFAULT;
	} else {
		_currentTheme = (Theme)(_currentTheme + 1);
	}
}
void Level::setTheme(Theme aTheme) {
	_currentTheme = aTheme;
	if (_currentTheme == THEME_DEFAULT) {
		_activeTheme = themeDefault;
	} else if (_currentTheme == THEME_BRIGHT) {
		_activeTheme = themeBright;
	}
}

uint16_t Level::getNewPosition(uint16_t aIndex, Direction aDir, TileType &aCollision) {
	uint16_t newIndex = aIndex;
	if (aDir == DIR_UP_LEFT) {
		newIndex += kLedDiagUpLeft;
		if (newIndex >= kLedCount) {
			aCollision = TILE_BOUNDARY;
			DEBUG("Upper left boundry.");
			return aIndex;
		}
	} else if (aDir == DIR_UP_RIGHT) {
		newIndex += kLedDiagUpRight;
		if (newIndex >= kLedCount) {
			aCollision = TILE_BOUNDARY;
			DEBUG("Upper Right boundry.");
			return aIndex;
		}
	} else if (aDir == DIR_DOWN_LEFT) {
		if (newIndex < kLedDiagDownLeft) {
			aCollision = TILE_BOUNDARY;
			DEBUG("Lower left boundry.");
			return aIndex;
		}
		newIndex -= kLedDiagDownLeft;
	} else if (aDir == DIR_DOWN_RIGHT) {
		if (newIndex < kLedDiagDownRight) {
			aCollision = TILE_BOUNDARY;
			DEBUG("LowerRight boundry.");
			return aIndex;
		}
		newIndex -= kLedDiagDownRight;
	}
	aCollision = checkForCollision(newIndex);
	if ((aCollision == TILE_WALL) || (aCollision == TILE_TANK_ONE) || (aCollision == TILE_TANK_TWO)) {
		return aIndex;
	}

	//_levelTiles[newIndex] = _levelTiles[aIndex];
	//_levelTiles[aIndex] = TILE_BACKGROUND;
	return newIndex;
}

uint32_t Level::getColorAtIndex(uint16_t aIndex) {
	Color color = _activeTheme[getTileAtIndex(aIndex)];
	return ((uint32_t)color.red << 16) | ((uint32_t)color.green <<  8) | color.blue;
}

/* Private Methods */

TileType Level::checkForCollision(uint16_t aIndex) {
	return _levelTiles[aIndex];
}

}