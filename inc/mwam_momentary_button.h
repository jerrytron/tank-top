#ifndef MWAM_MOMENTARY_BUTTON_H
#define MWAM_MOMENTARY_BUTTON_H

#include "mwam_constants.h"
#include "ElapsedTime.h"

namespace mwam
{

class MomentaryButton
{
	public:
		/* Public Methods */
		MomentaryButton();
		void initialize(uint8_t aBtnPin, uint32_t aUpdateFreq);
		void reset();
		void updateState();
		bool pressed();
		bool wasClicked();
		bool wasHeld();

		/* Public Variables */
		bool active;

	private:
		/* Private Methods */
		void updateButton();

		/* Private Variables */
		uint8_t _btnPin;
		uint32_t _updateFreq;
		ElapsedMillis _timeElapsed;
		uint32_t _closeTimeMillis;
		uint8_t _wasOpen;
		bool _clicked;
		bool _clickReported;
		bool _held;
		bool _holdReported;
};

}

#endif