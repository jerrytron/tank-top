#ifndef MWAM_CONST_VALUES_H
#define MWAM_CONST_VALUES_H

namespace mwam
{

typedef struct Color_t {
	uint8_t red;
	uint8_t green;
	uint8_t blue;

	// Default Constructor
	Color_t(uint8_t aRed=0, uint8_t aGreen=0, uint8_t aBlue=0)
		: red(aRed), green(aGreen), blue(aBlue) {}

	// Assignment Operator
	Color_t& operator=(const Color_t& aColor) {
		red = aColor.red;
		green = aColor.green;
		blue = aColor.blue;
		return *this;
	}

	// Add Operation
	Color_t operator+(const Color_t& aColor) const {
		return Color_t(aColor.red + red, aColor.green + green, aColor.blue + blue);
	}

	// Subtract Operation
	Color_t operator-(const Color_t& aColor) const {
		return Color_t(aColor.red - red, aColor.green - green, aColor.blue - blue);
	}

	// Equality Operation
	bool operator==(const Color_t& aColor) const {
		return (aColor.red == red && aColor.green == green && aColor.blue == blue);
	}
} Color;

// Colors
const Color kColorGreen = Color(0, 255, 0);
const Color kColorYellow = Color(255, 255, 0);
const Color kColorOrange = Color(255, 140, 0);
const Color kColorRed = Color(255, 0, 0);
const Color kColorBlack = Color(0, 0, 0);
const Color kColorWhite = Color(255, 255, 255);
const Color kColorBlue = Color(0, 0, 255);
const Color kColorPurple = Color(255, 0, 255);

// Serial Communication Commands
const uint8_t kSerialCmdMode = 'c';
const uint8_t kSerialCmdListeningMode = 'w';
const uint8_t kSerialCmdClearWiFi = 'x';
const uint8_t kSerialCmdRebootUnit = 'r';
const uint8_t kSerialCmdDfuMode = 'd';
const uint8_t kSerialCmdGetVersion = 0x03;

}

#endif