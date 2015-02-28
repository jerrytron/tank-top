#ifndef MWAM_JOYSTICK_H
#define MWAM_JOYSTICK_H

#include "mwam_constants.h"
#include "ElapsedTime.h"

namespace mwam
{

// Joystick Directions
typedef enum Direction_t {
	DIR_NONE = 0,
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
	JOY_ONE = 0,
	JOY_TWO,
	JOY_THREE,
	JOY_IDLE
} JoystickThreshold;

// Joystick Direction Sets
typedef enum DirectionSet_t {
	DIR_SET_EIGHT = 0,
	DIR_SET_FOUR,
	DIR_SET_FOUR_DIAG
} DirectionSet;

class Joystick
{
	public:
		/* Public Methods */
		Joystick();
		void initialize(uint8_t aPinX, uint8_t aPinY, DirectionSet aDirSet, uint32_t aUpdateFreq = 0);
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
		DirectionSet _dirSet;
		Direction _direction;
		JoystickThreshold _threshold;

};

}

#endif