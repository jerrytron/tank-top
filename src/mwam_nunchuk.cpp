#include "mwam_nunchuk.h"
#include "WiiNunchuk.h"

namespace mwam
{

/* Public Methods */

Nunchuk::Nunchuk() {
}

void Nunchuk::initialize(DirectionSet aDirSet, uint32_t aUpdateFreq) {
	Serial.println("HE");
	_dirSet = aDirSet;
	_updateFreq = aUpdateFreq;
	this->active = true;
	reset();

	Wire.begin();
}

bool Nunchuk::connect() {
	return nunchuk_init();
}

void Nunchuk::reset() {
	_timeElapsed = 0;
	_lastX = 0;
	_lastY = 0;
	_clickDown = false;
	_clickUp = false;
	_direction = DIR_NONE;
	_threshold = JOY_IDLE;
	_connected = false;
}

void Nunchuk::updateState() {
	if (this->active && (_timeElapsed >= _updateFreq)) {
		if (!_connected) {
			_connected = connect();
			if (_connected) {
				delay(100);
				nunchuk_get_data();
				//nunchuk_calibrate_joy();
				Log.info("CONNECTED");
			} else {
				Log.info("failed");
			}
		} else {
			updateJoystick();
		}
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
	nunchuk_get_data();
	_lastX = nunchuk_joy_x() - kNunchukOffsetX;
	_lastY = nunchuk_joy_y() - kNunchukOffsetY;
	_lastBtn = nunchuk_zbutton() || nunchuk_cbutton();
	//Log.info("X: %d, Y: %d, Btn: %d", _lastX, _lastY, _lastBtn);
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

	//uint16_t value = max(abs(aXValue), abs(aYValue));
	uint16_t value = abs(aXValue) + abs(aYValue);
	//DEBUG("JVal: %d", value);
	JoystickThreshold threshold = JOY_IDLE;
	if (value >= kNunchukThreshThree) {
		threshold = JOY_THREE;
	} else if (value >= kNunchukThreshTwo) {
		threshold = JOY_TWO;
	} else if (value >= kNunchukThreshOne) {
		threshold = JOY_ONE;
	} else {
		return;
	}

	if (aXValue <= -kNunchukThreshOne) {
		dir = DIR_LEFT;
		if (aYValue <= -kNunchukThreshOne) {
			//dir++;
			dir = DIR_DOWN_LEFT;
		} else if (aYValue >= kNunchukThreshOne) {
			//dir--;
			dir = DIR_UP_LEFT;
		}
	} else if (aXValue >= kNunchukThreshOne) {
		dir = DIR_RIGHT;
		if (aYValue <= -kNunchukThreshOne) {
			//dir++;
			dir = DIR_DOWN_RIGHT;
		} else if (aYValue >= kNunchukThreshOne) {
			//dir--;
			dir = DIR_UP_RIGHT;
		}
	} else {
		if (aYValue <= -kNunchukThreshOne) {
			dir = DIR_DOWN;
		} else if (aYValue >= kNunchukThreshOne) {
			dir = DIR_UP;
		}
	}

	_direction = (Direction)dir;
	_threshold = (JoystickThreshold)threshold;
	//Log.info("Dir: %d, thresh: %d", dir, threshold);
}

}