#include "mwam_joystick.h"

namespace mwam
{

/* Public Methods */

Joystick::Joystick() {
}

void Joystick::initialize(uint8_t aPinX, uint8_t aPinY, DirectionSet aDirSet, uint32_t aUpdateFreq) {
	_pinX = aPinX;
	_pinY = aPinY;
	pinMode(_pinX, INPUT);
	pinMode(_pinY, INPUT);
	_dirSet = aDirSet;
	_updateFreq = aUpdateFreq;
	this->active = true;
	reset();
}

void Joystick::reset() {
	_timeElapsed = 0;
	_lastX = 0;
	_lastY = 0;
	_clickDown = false;
	_clickUp = false;
	_direction = DIR_NONE;
	_threshold = JOY_IDLE;
}

void Joystick::updateState() {
	if (this->active && (_timeElapsed >= _updateFreq)) {
		updateJoystick();
		_timeElapsed = 0;
	}
}

Direction Joystick::getDirection() {
	if (_dirSet == DIR_SET_FOUR) {
		if (_direction % 2 != 0) {
			return DIR_NONE;
		}
	} else if (_dirSet == DIR_SET_FOUR_DIAG) {
		if (_direction % 2 == 0) {
			return DIR_NONE;
		}
	}
	Direction returnDir = _direction;
	_direction = DIR_NONE;
	return returnDir;
}

JoystickThreshold Joystick::getThreshold() {
	JoystickThreshold jt = _threshold;
	_threshold = JOY_IDLE;
	return jt;
}

bool Joystick::clickDown() {
	return _clickDown;
}

bool Joystick::clickUp() {
	bool clickUp = _clickUp;
	_clickUp = false;
	return clickUp;
}

/* Private Methods */

void Joystick::updateJoystick() {
	_lastX = analogRead(_pinX) - kJoystickOffset;
	_lastY = analogRead(_pinY) - kJoystickOffset;
	if (_lastY > kJoystickOffset) {
		_clickDown = true;
		_lastY -= kJoystickOffset;
	} else {
		if (_clickDown) {
			_clickUp = true;
		} else {
			_clickUp = false;
		}
		_clickDown = false;
	}
	//DEBUG("X: %d, Y: %d, Clicked: %s", _lastX, _lastY, (_clicked ? "yes" : "no"));
	updateDirections(_lastX, _lastY);
}

void Joystick::updateDirections(int16_t aXValue, int16_t aYValue) {
	uint8_t dir = 0;

	uint16_t value = max(abs(aXValue), abs(aYValue));
	JoystickThreshold threshold = JOY_IDLE;
	if (value >= kJoystickThreshThree) {
		threshold = JOY_THREE;
	} else if (value >= kJoystickThreshTwo) {
		threshold = JOY_TWO;
	} else if (value >= kJoystickThreshOne) {
		threshold = JOY_ONE;
	} else {
		return;
	}

	if (aXValue <= -kJoystickThreshOne) {
		dir = DIR_LEFT;
		if (aYValue <= -kJoystickThreshOne) {
			dir--;
		} else if (aYValue >= kJoystickThreshOne) {
			dir++;
		}
	} else if (aXValue >= kJoystickThreshOne) {
		dir = DIR_RIGHT;
		if (aYValue <= -kJoystickThreshOne) {
			dir--;
		} else if (aYValue >= kJoystickThreshOne) {
			dir++;
		}
	} else {
		if (aYValue <= -kJoystickThreshOne) {
			dir = DIR_UP;
		} else if (aYValue >= kJoystickThreshOne) {
			dir = DIR_DOWN;
		}
	}

	_direction = (Direction)dir;
	_threshold = (JoystickThreshold)threshold;
	//DEBUG("Dir: %d, thresh: %d", dir, threshold);
}

}