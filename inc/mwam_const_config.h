#ifndef MWAM_CONST_CONFIG_H
#define MWAM_CONST_CONFIG_H

namespace mwam
{

	// Max bogey LED index.
	const uint16_t kBogeyIndexMax = 8;
	// Min bogey LED index.
	const uint16_t kBogeyIndexMin = 0;

	// Total count of LEDs.
	const uint16_t kLedCount = 11;
	// LED pin.
	const uint8_t kLedPin = A7;
	// LED type.
	const uint8_t kLedType = 0x02; // WS2812B
	// LED default brightness.
	const uint8_t kLedBrightness = 200;

	// Rotary Digital Pins
	const uint8_t kRotaryDialingPin = D5;
	const uint8_t kRotaryPulsingPin = D6;

	// Button Pins
	const uint8_t kButtonPin = D4;
	const uint8_t kButtonLedPin = D3;

}

#endif