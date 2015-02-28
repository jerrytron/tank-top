#include "mwam_level.h"

namespace mwam
{

/*struct ThemeSet_t {
	Theme themeIndex;
} Themes[] = {
	{ THEME_DEFAULT, kColorGreen, kColorRed, kColorBlue, kColorRed, kColorYellow, kColorBlack, kColorOrange }
};*/

// Themes: P1 Color, P1 Turret, P2 Color, P2 Turret, Bullets, Background, Walls
const Color themeDefault[] = { kColorGreen, kColorRed, kColorBlue, kColorRed, kColorYellow, kColorBlack, kColorOrange };


/* Public Methods */

Level::Level() {
}

void Level::initialize(Theme aTheme, uint32_t aUpdateFreq) {
	_updateFreq = aUpdateFreq;
	setTheme(aTheme);
	_timeElapsed = 0;
}

void Level::updateState() {
	if (this->active && (_timeElapsed >= _updateFreq)) {
		_timeElapsed = 0;
	}
}

void Level::setTheme(Theme aTheme) {
	if (aTheme == THEME_DEFAULT) {
		_activeTheme = themeDefault;
	}
}

uint16_t Level::updatePosition(uint16_t aIndex, Direction aDir, TileType &aCollision) {
	uint16_t newIndex = aIndex;
	if (aDir == DIR_UP_LEFT) {
		newIndex += kLedDiagUpLeft;
		if (newIndex >= kLedCount) {
			aCollision = TILE_WALL;
			return aIndex;
		}
	} else if (aDir == DIR_UP_RIGHT) {
		newIndex += kLedDiagUpRight;
		if (newIndex >= kLedCount) {
			aCollision = TILE_WALL;
			return aIndex;
		}
	} else if (aDir == DIR_DOWN_LEFT) {
		if (newIndex < kLedDiagDownLeft) {
			aCollision = TILE_WALL;
			return aIndex;
		}
		newIndex -= kLedDiagDownLeft;
	} else if (aDir == DIR_DOWN_RIGHT) {
		if (newIndex < kLedDiagDownRight) {
			aCollision = TILE_WALL;
			return aIndex;
		}
		newIndex -= kLedDiagDownRight;
	}
	aCollision = checkForCollision(newIndex);
	if ((aCollision == TILE_WALL) && (aCollision == TILE_TANK)) {
		return aIndex;
	}

	return newIndex;
}

TileType Level::getTileAtIndex(uint16_t aIndex) {
	return _levelTiles[aIndex];
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