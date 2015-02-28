#ifndef MWAM_JOYSTICK_H
#define MWAM_JOYSTICK_H

#include "mwam_constants.h"
#include "ElapsedTime.h"

namespace mwam
{

// Joystick Directions
typedef enum Direction_t {
	DIR_UP_LEFT,
	DIR_LEFT,
	DIR_DOWN_LEFT,
	DIR_UP,
	DIR_UP_RIGHT,
	DIR_RIGHT,
	DIR_DOWN_RIGHT,
	DIR_DOWN
} Direction;

// Joystick Thresholds
typedef enum JoystickThreshold_t {
	JOY_IDLE = 0,
	JOY_ONE,
	JOY_TWO,
	JOY_THREE
} JoystickThreshold;

class Joystick
{
	public:
		/* Public Methods */
		Joystick();
		void initialize(uint8_t aPinX, uint8_t aPinY, uint32_t aUpdateFreq);
		void reset();
		void updateState();
		Direction getDirection();
		JoystickThreshold getThreshold();
		bool clickDown();
		bool clickUp();

		/* Public Variables */
		bool active;

	private:
		/* Private Methods */
		void updateJoystick();
		void updateDirections(int16_t aXValue, int16_t aYValue);

		/* Private Variables */
		ElapsedMillis _timeElapsed;
		uint32_t _updateFreq;
		uint8_t _pinX;
		uint8_t _pinY;
		int16_t _lastX;
		int16_t _lastY;
		bool _clickDown;
		bool _clickUp;
		Direction _direction;
		JoystickThreshold _threshold;

};

}

#endif