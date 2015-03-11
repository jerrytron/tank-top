#ifndef MWAM_CONST_CONFIG_H
#define MWAM_CONST_CONFIG_H

namespace mwam
{

	// Total count of LEDs.
	const uint16_t kLedCount = 240;
	// LEDs per ring in the spiral.
	const uint8_t kLedsPerRing = 21;
	// LED indexes to add/subtract for diagnal positions.
	const int8_t kLedDiagUpLeft = 20;
	const int8_t kLedDiagUpRight = 21;
	const int8_t kLedDiagDownLeft = 21;
	const int8_t kLedDiagDownRight = 20;
	// LED pin.
	const uint8_t kLedPin = A7;
	// LED type.
	const uint8_t kLedType = 0x02; // WS2812B
	// LED default brightness.
	const uint8_t kLedBrightness = 200;

	// Base delay for player movement.
	const uint16_t kIntervalPlayerDelayMillis = 400;
	// Amount to reduce player speed delay for increased speed levels.
	const uint16_t kIntervalPlayerSpeedMillis = 125;

	// Player One Start Index
	const uint8_t kPlayerOneStartIndex = 123;
	// Player One Start Index
	const uint8_t kPlayerTwoStartIndex = 134;
	// Max Bullets Live
	const uint8_t kMaxBulletsLive = 3;
	// Max Bullet Bounces
	const uint8_t kMaxBulletBounces = 5;
	// Bullet speed, delay before movement.
	const uint8_t kBulletMovementDelay = 100;
	// Health Total Per Player
	const uint8_t kHealthTotal = 3;

	// Piezo Pins
	const uint8_t kPiezoOnePin = A0;
	const uint8_t kPiezoTwoPin = A1;

	// Button Pins
	const uint8_t kButtonPin = D3;
	const uint8_t kButtonLedPin = D2;

	// Joystick Pins
	const uint8_t kJoystickOneX = A3;
	const uint8_t kJoystickOneY = A2;
	const uint8_t kJoystickTwoX = A5;
	const uint8_t kJoystickTwoY = A4;

	const uint16_t kJoystickOffset = 2045;
	const uint16_t kJoystickThreshOne = 100;
	const uint16_t kJoystickThreshTwo = 450;
	const uint16_t kJoystickThreshThree = 850;

	// Timeout to input a serial command.
	const uint16_t kTimeoutSerialMillis = 8000;
	// How often to check for serial communication.
	const uint16_t kIntervalSerialMillis = 500;
	// How often to update the Led set state.
	const uint16_t kIntervalLedSetMillis = 10;
	// How often to update the joystick set state.
	const uint16_t kIntervalJoystickMillis = 10;
	// How often to update the button state.
	const uint16_t kIntervalButtonMillis = 10;
	// How often to update the piezos.
	const uint16_t kIntervalPiezoMillis = 10;
	// How often to update the game play loop.
	//const uint16_t kIntervalGamePlayMillis = 10;

}

#endif