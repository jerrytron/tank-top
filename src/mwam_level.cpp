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
const Color themeDefault[] = { kColorBlack, kColorGreen, kColorRed, kColorBlue, kColorRed, kColorYellow, kColorPurple };

const Color themeBright[] = { kColorWhite, kColorBlue, kColorYellow, kColorRed, kColorYellow, kColorOrange, kColorBlack };


/* Public Methods */

Level::Level() {
}

void Level::initialize(Theme aTheme, uint32_t aUpdateFreq) {
	_updateFreq = aUpdateFreq;
	_timeElapsed = 0;
	setTheme(aTheme);
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

void Level::setTankAtIndex(Tank* aTank) {
	if (aTank->getTankNumber() == TANK_ONE) {
		//setTileAtIndex(aTank->getLastOverlapTile(), aTank->getLastIndex());
		setTileAtIndex(TILE_BACKGROUND, aTank->getLastIndex());
		setTileAtIndex(aTank->getLastTurretOverlapTile(), aTank->getLastTurretIndex());
		//DEBUG("Cleanup indexes: %d & %d", aTank->getLastIndex(), aTank->getLastTurretIndex());
		setTileAtIndex(TILE_TANK_ONE, aTank->getIndex());
		setTileAtIndex(TILE_TURRET_ONE, aTank->getTurretIndex());
		//DEBUG("New indexes: %d & %d", aTank->getIndex(), aTank->getTurretIndex());
	} else if (aTank->getTankNumber() == TANK_TWO) {
		//setTileAtIndex(aTank->getLastOverlapTile(), aTank->getLastIndex());
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
			index += (kLedDiagDownLeft + kLedDiagDownRight);
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

/*void Level::drawText(TileType aTile, uint16_t aIndex, char* aBuffer, uint16_t aLength) {
	if (aIndex > kTextLedIndexMax) {
		aIndex = kTextLedIndexMax;
	}

	DEBUG("Msg: %s", aBuffer);
	// initiate display of new text
	int8_t textPixelOffset = 0;//-kLedsPerRing;
	//uint8_t textCycleCount = 0;
	//uint8_t repeatCount = 0;

	// fade between rows
	//byte maxBright = kTextIntensity - repeatCount * kFadePerRepeat;
	//byte thisBright, nextBright;
	//crossFade(255 * textCycleCount / kCyclesPerPx, maxBright, thisBright, nextBright);
	// generate vertical rows
	uint8_t activeCols = kLedsPerRing - 2;
	// calculate text length in pixels
	uint16_t totalTextPixels = 0;

	for (int i = 0; i < aLength; i++) {
		// sum up width of individual chars
		totalTextPixels += kFontGlyphs[glyphIndexForChar(aBuffer[i])].width + kGlyphSpacing;
	}
	DEBUG("TTP: %d", totalTextPixels);

	for (int x = 0; x < kLedsPerRing; x++) {
		uint8_t column = 0;
		// determine font column
		if (x < activeCols) {
			DEBUG("Active");
			int colPixelOffset = textPixelOffset + x;
			DEBUG("Col Off: %d", colPixelOffset);
			if (colPixelOffset >= 0) {
				// visible column
				// - calculate character index
				int charIndex = 0;
				int glyphOffset = colPixelOffset;
				const glyph_t *glyphP = NULL;
				while (charIndex < aLength) {
					glyphP = &kFontGlyphs[glyphIndexForChar(aBuffer[charIndex])];
					int cw = glyphP->width + kGlyphSpacing;
					if (glyphOffset < cw) {
						break; // found char
					}
					glyphOffset -= cw;
					charIndex++;
				}
				// now we have
				// - glyphP = the glyph,
				// - glyphOffset=column offset within that glyph (but might address a spacing column not stored in font table)
				if (charIndex < aLength) {
					// is a column of a visible char
					if (glyphOffset < glyphP->width) {
						// fetch glyph column
						column = glyphP->cols[glyphOffset];
					}
				}
			}
		}
		// now render columns
		for (int glyphRow = 0; glyphRow < kRowsPerGlyph; glyphRow++) {
			int i;
			//int leftstep;
			if (kMirrorText) {
				i = (glyphRow + 1) * kLedsPerRing - 1 - x; // LED index, x-direction mirrored
				leftstep = 1;
			} else {
			i = glyphRow * kLedsPerRing + x; // LED index
			//leftstep = -1;
			//}
			if (glyphRow < kRowsPerGlyph) {
				if (column & (0x40 >> glyphRow)) {
					//textLayer[i] = thisBright; DELETE
					DEBUG("I: %d", i);
					setTileAtIndex(aTile, i);
					// also adjust pixel left to this one
					if (x > 0) {
						increase(textLayer[i + leftstep], nextBright, maxBright);
					}
					continue;
				}
			}
			setTileAtIndex(TILE_BACKGROUND, i); // no text
		}
	}
}*/

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
			index += (kLedDiagUpLeft + kLedDiagUpRight);
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
			index += (kLedDiagDownLeft + kLedDiagDownRight);
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
	if (_currentTheme == THEME_BRIGHT) {
		_currentTheme = THEME_DEFAULT;
	} else {
		_currentTheme = (Theme)(_currentTheme + 1);
	}
}
void Level::setTheme(Theme aTheme) {
	_currentTheme = aTheme;
	if (_currentTheme == THEME_DEFAULT) {
		//memcpy(_activeTheme, themeDefault, sizeof(themeDefault));
		_activeTheme = themeDefault;
	} else if (_currentTheme == THEME_BRIGHT) {
		//memcpy(_activeTheme, themeBright, sizeof(themeDefault));
		_activeTheme = themeBright;
	}
	Manager::getInstance().hardwareManager->ledSet()->updateLeds(this);
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
		newIndex += kLedDiagDownLeft;
	} else if (aDir == DIR_DOWN_RIGHT) {
		if (newIndex < kLedDiagDownRight) {
			aCollision = TILE_BOUNDARY;
			DEBUG("LowerRight boundry.");
			return aIndex;
		}
		newIndex += kLedDiagDownRight;
	}
	aCollision = checkForCollision(newIndex);
	if ((aCollision == TILE_WALL) || (aCollision == TILE_TANK_ONE) || (aCollision == TILE_TANK_TWO)) {
		//return aIndex;
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