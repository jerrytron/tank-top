#ifndef MWAM_CONST_CONFIG_H
#define MWAM_CONST_CONFIG_H

namespace mwam
{

	// Total count of LEDs.
	const uint8_t kLedCount = 240;
	// LEDs per ring in the spiral.
	const uint8_t kLedsPerRing = 21;
	// LED pin.
	const uint8_t kLedPin = A7;
	// LED type.
	const uint8_t kLedType = 0x02; // WS2812B
	// LED default brightness.
	const uint8_t kLedBrightness = 200;

	// Piezo Pins
	const uint8_t kPiezoOnePin = A0;
	const uint8_t kPiezoTwoPin = A1;

	// Button Pins
	const uint8_t kButtonPin = D4;
	const uint8_t kButtonLedPin = D3;

	// Timeout to input a serial command.
	const uint16_t kTimeoutSerialMillis = 8000;
	// How often to check for serial communication.
	const uint16_t kIntervalSerialMillis = 500;
	// How often to update the Led set state.
	const uint16_t kIntervalLedSetMillis = 10;
	// How often to update the button state.
	const uint16_t kIntervalButtonMillis = 10;
	// How often to update the piezos.
	const uint16_t kIntervalPiezosMillis = 10;
	// How often to update the tanks.
	const uint16_t kIntervalTankMillis = 10;
	// How often to update the bullets.
	const uint16_t kIntervalBulletMillis = 10;
	// How often to update the Level.
	const uint16_t kIntervalLevelMillis = 10;

}

#endif