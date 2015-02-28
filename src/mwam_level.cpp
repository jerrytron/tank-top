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

void Level::initialize(uint32_t aUpdateFreq, Theme aTheme) {
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

/* Private Methods */

}