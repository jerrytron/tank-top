#ifndef MWAM_CONST_CONFIG_H
#define MWAM_CONST_CONFIG_H

namespace mwam
{
#define CYLINDRUS

	// Total count of LEDs.
#ifdef CYLINDRUS
const uint16_t kLedCount = 600; // Cylindrus
#else
const uint16_t kLedCount = 240; // Original
#endif

	// LEDs per ring in the spiral.
#ifdef CYLINDRUS
	const uint8_t kLedsPerRing = 31;
#else
	const uint8_t kLedsPerRing = 20;
#endif
	// LED indexes to add/subtract for diagnal positions.
#ifdef CYLINDRUS
	/*const int16_t kLedDiagUpLeft = 31;
	const int16_t kLedDiagUp = 32;
	const int16_t kLedDiagUpRight = 33;
	const int16_t kLedDiagRight = 1;
	const int16_t kLedDiagDownRight = -31;
	const int16_t kLedDiagDown = -32;
	const int16_t kLedDiagDownLeft = -33;
	const int16_t kLedDiagLeft = -1;*/

	const int16_t kLedDiagUpLeft = 31;
	const int16_t kLedDiagUpRight = 1;
	const int16_t kLedDiagDownLeft = -1;
	const int16_t kLedDiagDownRight = -31;
#else
	const int16_t kLedDiagUpLeft = 20;
	const int16_t kLedDiagUpRight = 21;
	const int16_t kLedDiagDownLeft = -21;
	const int16_t kLedDiagDownRight = -20;
#endif

#ifdef CYLINDRUS
	// 20 between top and bottom rows 'connecting'
	/*const uint16_t kLedVertRollover = 12;
	const uint16_t kLedLowThresh = 569;
	const uint16_t kLedHighThresh = 588;
	const uint16_t kLedRollover = 12;*/

	const uint16_t kLedVertRollover = 12;
	const uint16_t kLedDiagRightLowThresh = 569;
	const uint16_t kLedDiagRightHighThresh = 588;
	const uint16_t kLedDiagRightRollover = 12;
	const uint16_t kLedDiagLeftLowThresh = 569;
	const uint16_t kLedDiagLeftHighThresh = 588;
	const uint16_t kLedDiagLeftRollover = 12;

	const uint16_t kTextLedIndexMax = 588;
#else
	const uint8_t kLedVertRollover = 34;
	const uint8_t kLedDiagRightLowThresh = 219;
	const uint8_t kLedDiagRightHighThresh = 226;
	const uint8_t kLedDiagRightRollover = 15;
	const uint8_t kLedDiagLeftLowThresh = 220;
	const uint8_t kLedDiagLeftHighThresh = 225;
	const uint8_t kLedDiagLeftRollover = 13;

	const uint8_t kTextLedIndexMax = 225;
#endif
	const bool kMirrorText = false;

	// LED pin.
	const uint8_t kLedPin = A4;
	// LED type.
	const uint8_t kLedType = 0x02; // WS2812B
	// LED default brightness.
	const uint8_t kLedBrightness = 200;

	// Base delay for player movement.
	const uint16_t kIntervalPlayerDelayMillis = 325;
	// Amount to reduce player speed delay for increased speed levels.
	const uint16_t kIntervalPlayerSpeedMillis = 125;

#ifdef CYLINDRUS
	// Player One Start Index
	const uint16_t kPlayerOneStartIndex = 312; // Ten rows up from index 0
	// Player One Start Index
	const uint16_t kPlayerTwoStartIndex = 297;
#else
	// Player One Start Index
	const uint8_t kPlayerOneStartIndex = 141;
	// Player One Start Index
	const uint8_t kPlayerTwoStartIndex = 152;
#endif
	// Max Bullets Live
	const uint8_t kMaxBulletsLive = 3;
	// Max Bullet Bounces
	const uint8_t kMaxBulletBounces = 3;
	// Bullet speed, delay before movement.
	const uint8_t kBulletMovementDelay = 100;
	// Health Total Per Player
	const uint8_t kHealthTotal = 3;
	// Lives Total Per Player
	const uint8_t kLivesTotal = 1;
	// Do turrets count as obstacles.
	const bool kTurretCollision = true;
	// Can you get hurt by your own bullets?
	const bool kFriendlyFire = true;
	// Number of blinks until effect is over.
	const uint16_t kBlinkMax = 15;
	// Blinking interval.
	const uint16_t kBlinkIntervalMillis = 150;

	// Piezo Pins
	const uint8_t kPiezoOnePin = A5;
	const uint8_t kPiezoTwoPin = A6;

	// Button Pins
	const uint8_t kButtonPin = D2;

#ifdef CYLINDRUS
	const uint8_t kJoystickOneBtnPin = D3;
	const uint8_t kJoystickTwoBtnPin = D4;
#else
	const uint8_t kJoystickOneBtnPin = D0;
	const uint8_t kJoystickTwoBtnPin = D1;
#endif

	// Joystick Pins
	const uint8_t kJoystickOneXPin = A1;
	const uint8_t kJoystickOneYPin = A0;
	const uint8_t kJoystickTwoXPin = A3;
	const uint8_t kJoystickTwoYPin = A2;

	const uint16_t kJoystickOffset = 2045;
	const uint16_t kJoystickThreshOne = 40;
	const uint16_t kJoystickThreshTwo = 1600;
	const uint16_t kJoystickThreshThree = 2000;

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