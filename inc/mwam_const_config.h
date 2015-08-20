#ifndef MWAM_CONST_CONFIG_H
#define MWAM_CONST_CONFIG_H

namespace mwam
{

	// Total count of LEDs.
	const uint16_t kLedCount = 240;
	// LEDs per ring in the spiral.
	const uint8_t kLedsPerRing = 20;
	// LED indexes to add/subtract for diagnal positions.
	const int8_t kLedDiagUpLeft = 20;
	const int8_t kLedDiagUpRight = 21;
	const int8_t kLedDiagDownLeft = -21;
	const int8_t kLedDiagDownRight = -20;

	const uint8_t kLedVertRollover = 34;
	const uint8_t kLedDiagRightLowThresh = 219;
	const uint8_t kLedDiagRightHighThresh = 226;
	const uint8_t kLedDiagRightRollover = 15;
	const uint8_t kLedDiagLeftLowThresh = 220;
	const uint8_t kLedDiagLeftHighThresh = 225;
	const uint8_t kLedDiagLeftRollover = 13;

	const uint8_t kTextLedIndexMax = 225;
	const bool kMirrorText = false;

	// LED pin.
	const uint8_t kLedPin = A4;
	// LED type.
	const uint8_t kLedType = 0x02; // WS2812B
	// LED default brightness.
	const uint8_t kLedBrightness = 200;

	// Base delay for player movement.
	const uint16_t kIntervalPlayerDelayMillis = 400;
	// Amount to reduce player speed delay for increased speed levels.
	const uint16_t kIntervalPlayerSpeedMillis = 125;

	// Player One Start Index
	const uint8_t kPlayerOneStartIndex = 141;
	// Player One Start Index
	const uint8_t kPlayerTwoStartIndex = 152;
	// Max Bullets Live
	const uint8_t kMaxBulletsLive = 3;
	// Max Bullet Bounces
	const uint8_t kMaxBulletBounces = 5;
	// Bullet speed, delay before movement.
	const uint8_t kBulletMovementDelay = 100;
	// Health Total Per Player
	const uint8_t kHealthTotal = 2;
	// Lives Total Per Player
	const uint8_t kLivesTotal = 3;
	// Do turrets count as obstacles.
	const bool kTurretCollision = true;
	// Can you get hurt by your own bullets?
	const bool kFriendlyFire = true;
	// Number of blinks until effect is over.
	const uint16_t kBlinkMax = 15;
	// Blinking interval.
	const uint16_t kBlinkIntervalMillis = 200;

	// Piezo Pins
	const uint8_t kPiezoOnePin = A5;
	const uint8_t kPiezoTwoPin = A6;

	// Button Pins
	const uint8_t kButtonPin = D2;

	const uint8_t kJoystickOneBtnPin = D0;
	const uint8_t kJoystickTwoBtnPin = D1;

	// Joystick Pins
	const uint8_t kJoystickOneXPin = A1;
	const uint8_t kJoystickOneYPin = A0;
	const uint8_t kJoystickTwoXPin = A3;
	const uint8_t kJoystickTwoYPin = A2;

	const uint16_t kJoystickOffset = 2045;
	const uint16_t kJoystickThreshOne = 200;
	const uint16_t kJoystickThreshTwo = 800;
	const uint16_t kJoystickThreshThree = 1600;

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