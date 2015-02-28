#ifndef MWAM_LEVEL_H
#define MWAM_LEVEL_H

#include "mwam_constants.h"
#include "ElapsedTime.h"

namespace mwam
{

typedef enum Theme_t {
	THEME_DEFAULT = 0
} Theme;

typedef enum ThemeElement_t {
	P1_BODY = 0,
	P1_TURRET,
	P2_BODY,
	P2_TURRET,
	BULLETS,
	BACKGROUND,
	WALLS
} ThemeElement;

class Level
{
	public:
		/* Public Methods */
		Level();
		void initialize(uint32_t aUpdateFreq, Theme aTheme);
		void updateState();

		void setTheme(Theme aTheme);

		/* Public Variables */
		bool active;

	private:
		/* Private Methods */
		const Color* _activeTheme;

		/* Private Variables */
		ElapsedMillis _timeElapsed;
		uint32_t _updateFreq;

};

}

#endif