#include "mwam_Nunchuk.h"

namespace mwam
{

/* Public Methods */

Nunchuk::Nunchuk() {
}

void Nunchuk::initialize(uint8_t aPinX, uint8_t aPinY, uint8_t aPinBtn, DirectionSet aDirSet, uint32_t aUpdateFreq) {
	_pinX = aPinX;
	_pinY = aPinY;
	_pinBtn = aPinBtn;
	pinMode(_pinX, INPUT);
	pinMode(_pinY, INPUT);
	pinMode(_pinBtn, INPUT);
	_dirSet = aDirSet;
	_updateFreq = aUpdateFreq;
	this->active = true;
	reset();
}

void Nunchuk::reset() {
	_timeElapsed = 0;
	_lastX = 0;
	_lastY = 0;
	_clickDown = false;
	_clickUp = false;
	_direction = DIR_NONE;
	_threshold = JOY_IDLE;
}

void Nunchuk::updateState() {
	if (this->active && (_timeElapsed >= _updateFreq)) {
		updateJoystick();
		_timeElapsed = 0;
	}
}

Direction Nunchuk::getDirection() {
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

JoystickThreshold Nunchuk::getThreshold() {
	JoystickThreshold jt = _threshold;
	_threshold = JOY_IDLE;
	return jt;
}

bool Nunchuk::clickDown() {
	return _clickDown;
}

bool Nunchuk::clickUp() {
	bool clickUp = _clickUp;
	_clickUp = false;
	return clickUp;
}

/* Private Methods */

void Nunchuk::updateJoystick() {
	_lastX = analogRead(_pinX) - kJoystickOffset;
	_lastY = analogRead(_pinY) - kJoystickOffset;
	_lastBtn = digitalRead(_pinBtn);
	//DEBUG("X: %d, Y: %d, Btn: %d", _lastX, _lastY, _lastBtn);
	if (_lastBtn) {
		_clickDown = true;
	} else {
		if (_clickDown) {
			_clickUp = true;
		} else {
			_clickUp = false;
		}
		_clickDown = false;
	}
	//DEBUG("X: %d, Y: %d, Clicked: %s", _lastX, _lastY, (_clickDown ? "yes" : "no"));
	updateDirections(_lastX, _lastY);
}

void Nunchuk::updateDirections(int16_t aXValue, int16_t aYValue) {
	uint8_t dir = 0;

	uint16_t value = max(abs(aXValue), abs(aYValue));
	//DEBUG("JVal: %d", value);
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