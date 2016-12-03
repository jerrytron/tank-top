#include "mwam_level.h"
#include "mwam_tank.h"
#include "mwam_manager.h"

namespace mwam
{

/*struct ThemeSet_t {
	Theme themeIndex;
} Themes[] = {
	{ THEME_DEFAULT, kColorGreen, kColorRed, kColorBlue, kColorRed, kColorYellow, kColorBlack, kColorOrange }
};*/

// Themes: Background, P1 Color, P1 Turret, P2 Color, P2 Turret, Bullets, Walls
const Color themes[][7] = {
	{ kColorBlack, kColorGreen, kColorRed, kColorBlue, kColorRed, kColorYellow, kColorPurple },
	{ kColorBlack, kColorBlue, kColorYellow, kColorRed, kColorYellow, kColorOrange, kColorWhite },
	{ kColorBlack, kColorPurple, kColorWhite, kColorOrange, kColorWhite, kColorGreen, kColorBlue }
};

/* Public Methods */

Level::Level() {
}

void Level::initialize(Theme aTheme, DimLevel aDimLevel, uint32_t aUpdateFreq) {
	_updateFreq = aUpdateFreq;
	_timeElapsed = 0;
	setTheme(aTheme);
	setDimLevel(aDimLevel);
	clearLevel();
}

void Level::updateState() {
	if (this->active && (_timeElapsed >= _updateFreq)) {
		_timeElapsed = 0;
	}
}

void Level::clearLevel() {
	memset(_levelTiles, 0, kLedCount);
}

void Level::setTileAtIndex(TileType aTile, uint16_t aIndex) {
	_levelTiles[aIndex] = aTile;
}

TileType Level::getTileAtIndex(uint16_t aIndex) {
	return _levelTiles[aIndex];
}

void Level::drawSquare(bool aDiagonal, TileType aTile, uint16_t aIndex, uint8_t aWidth, uint8_t aHeight) {
	int16_t index = aIndex;
	if (aDiagonal) {
		for (uint8_t i = 0; i < aWidth; i++) {
			drawLine(DIR_DOWN_RIGHT, aTile, aIndex, aHeight);
			index += kLedDiagUpRight;
			if (index >= kLedCount) {
				if (aIndex < kLedDiagRightHighThresh) {
					index = kLedDiagRightRollover - (kLedDiagRightLowThresh - aIndex);
				} else {
					index = kLedDiagRightRollover - ((kLedCount - 1) - aIndex) - 1;
				}
			}
			aIndex = index;
		}
	} else {
		for (uint8_t i = 0; i < aHeight; i++) {
			drawLine(DIR_RIGHT, aTile, aIndex, aWidth);
#ifdef CYLINDRUS
			index += kLedDiagDownRight;
#else
			index += (kLedDiagDownLeft + kLedDiagDownRight);
#endif
			if (index < 0) {
				index = aIndex + ((kLedCount - 1) - kLedVertRollover);
			}
			aIndex = index;
		}
	}
}

int16_t Level::glyphIndexForChar(const char aChar) {
	int i = aChar - 0x20;
	if (i < 0 || i >= kNumGlyphs) {
		i = 95; // ASCII 0x7F-0x20
	}
	return i;
}

void Level::drawLine(Direction aDir, TileType aTile, uint16_t aIndex, uint8_t aLength) {
	int16_t index = aIndex;
	uint16_t newIndex = aIndex;
	for (uint8_t i = 0; i < aLength; i++) {
		setTileAtIndex(aTile, newIndex);
		if (aDir == DIR_UP_RIGHT) {
			index += kLedDiagUpRight;
			if (index >= kLedCount) {
				if (newIndex < kLedDiagRightHighThresh) {
					// Just comment out and ignore drawing to the LEDs between end of LED strip and beginning?
					index = kLedDiagRightRollover - (kLedDiagRightLowThresh - newIndex) - 1;
				} else {
					index = kLedDiagRightRollover - ((kLedCount - 1) - newIndex) - 2;
				}
			}
		} else if (aDir == DIR_UP) {
#ifdef CYLINDRUS
			index += kLedDiagUpLeft;
#else
			index += (kLedDiagUpLeft + kLedDiagUpRight);
#endif
			if (index >= kLedCount) {
				index = kLedVertRollover - ((kLedCount - 1) - newIndex);
			}
		} else if (aDir == DIR_UP_LEFT) {
			index += kLedDiagUpLeft;
			if (index >= kLedCount) {
				if (newIndex < kLedDiagLeftHighThresh) {
					index = kLedDiagLeftRollover - (kLedDiagLeftLowThresh - newIndex) + 2;
				} else {
					index = kLedDiagLeftRollover - ((kLedCount - 1) - newIndex) + 1;
				}
			}
		} else if (aDir == DIR_LEFT) {
			index -= 1;
			if (index < 0) {
				index = kLedCount - 1;
			}
		} else if (aDir == DIR_DOWN_LEFT) {
			index += kLedDiagDownLeft;
			if (index < 0) {
				if (newIndex <= kLedDiagLeftRollover) {
					index = newIndex + ((kLedCount - 1) - kLedDiagRightRollover) + 2;
				} else {
					index = newIndex + (kLedDiagRightLowThresh - kLedDiagRightRollover) + 1;
				}
			}
		} else if (aDir == DIR_DOWN) {
#ifdef CYLINDRUS
			index += kLedDiagDownRight;
#else
			index += (kLedDiagDownLeft + kLedDiagDownRight);
#endif
			if (index < 0) {
				index = newIndex + ((kLedCount - 1) - kLedVertRollover);
			}
		} else if (aDir == DIR_DOWN_RIGHT) {
			index += kLedDiagDownRight;
			if (index < 0) {
				if (newIndex < kLedDiagRightRollover) {
					index = newIndex + ((kLedCount - 1) - kLedDiagLeftRollover); // - 1 ?
				} else {
					index = newIndex + (kLedDiagLeftLowThresh - kLedDiagLeftRollover) - 1; // - 2 ?
				}
			}
		} else if (aDir == DIR_RIGHT) {
			index += 1;
			if (index >= kLedCount) {
				index = 0;
			}
		}
		//DEBUG("Index: %d", index);
		newIndex = index;
	}
}

void Level::nextTheme() {
	if (_currentTheme == THEME_THREE) {
		_currentTheme = THEME_ONE;
	} else {
		_currentTheme = (Theme)(_currentTheme + 1);
	}
	setTheme(_currentTheme);
}

void Level::setTheme(Theme aTheme) {
	_currentTheme = aTheme;
	_activeTheme = themes[_currentTheme];
	Manager::getInstance().hardwareManager->ledSet()->updateLeds(this);
}

void Level::nextDimLevel() {
	if (_dimLevel == DIM_NONE) {
		_dimLevel = DIM_THREE_QUARTER;
	} else {
		_dimLevel = (DimLevel)(_dimLevel + 1);
	}
	setDimLevel(_dimLevel);
}

void Level::setDimLevel(DimLevel aDimLevel) {
	_dimLevel = aDimLevel;
}

uint16_t Level::getNewPosition(uint16_t aIndex, Direction aDir, TileType &aCollision) {
	int16_t newIndex = aIndex;

	if (aDir == DIR_UP_LEFT) {
		newIndex += kLedDiagUpLeft;
		if (newIndex >= kLedCount) {
			aCollision = TILE_BOUNDARY;
			//DEBUG("Upper left boundry.");
			return aIndex;
		}
	} else if (aDir == DIR_UP_RIGHT) {
		newIndex += kLedDiagUpRight;
		if (newIndex >= kLedCount) {
			aCollision = TILE_BOUNDARY;
			//DEBUG("Upper Right boundry.");
			return aIndex;
		}
	} else if (aDir == DIR_DOWN_LEFT) {
		if (newIndex < abs(kLedDiagDownLeft)) {
			aCollision = TILE_BOUNDARY;
			//DEBUG("Lower left boundry.");
			return aIndex;
		}
		newIndex += kLedDiagDownLeft;
	} else if (aDir == DIR_DOWN_RIGHT) {
		if (newIndex < abs(kLedDiagDownRight)) {
			aCollision = TILE_BOUNDARY;
			//DEBUG("LowerRight boundry.");
			return aIndex;
		}
		newIndex += kLedDiagDownRight;
	}

	aCollision = getTileAtIndex(newIndex);
	if (aCollision == TILE_WALL) {
		return aIndex;
	}

	return newIndex;
}

uint32_t Level::getColorAtIndex(uint16_t aIndex) {
	Color color = _activeTheme[getTileAtIndex(aIndex)];
	if (_dimLevel == DIM_QUARTER) {
		color.red *= 0.75;
		color.blue *= 0.75;
		color.green *= 0.75;
	} else if (_dimLevel == DIM_HALF) {
		color.red *= 0.5;
		color.blue *= 0.5;
		color.green *= 0.5;
	} else if (_dimLevel == DIM_THREE_QUARTER) {
		color.red *= 0.25;
		color.blue *= 0.25;
		color.green *= 0.25;
	}
	return ((uint32_t)color.red << 16) | ((uint32_t)color.green <<  8) | color.blue;
}

/* Private Methods */


}