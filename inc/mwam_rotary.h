#ifndef MWAM_ROTARY_H
#define MWAM_ROTARY_H

#include "mwam_constants.h"
#include "ElapsedTime.h"

namespace mwam
{

class Rotary
{
	public:
		/* Public Methods */
		Rotary();
		void initialize(uint8_t aPinInUse, uint8_t aPinPulses, uint32_t aUpdateFreq);
		void updateState();

		void resetRotary();
		// Check for a new dial event.
		bool newDialEvent();
		// Whether or not a dial is in progress.
		bool dialing();
		// The number of pulses for a dial in progress.
		uint8_t pulseCount();
		// The last number dialed. Clears event flag.
		int8_t lastDialed();

		/* Public Variables */
		bool active;

	private:
		/* Private Methods */
		void updateRotary();

		/* Private Variables */
		// Time elapsed.
		ElapsedMillis _timeElapsed;
		// Update frequency.
		uint32_t _updateFreq;
		// Pin to read if rotary is in use (dialing).
		uint8_t _pinDialing;
		// Pin to read if rotary is pulsing (passing a number).
		uint8_t _pinPulsing;
		// A rotary dial has completed since last examination.
		bool _dialEvent;
		// Is rotary being dialed.
		bool _dialing;
		// The pulse count for this dial.
		uint8_t _pulseCount;
		// Last number dialed.
		int8_t _lastDialed;
		// Last pin dialing read.
		uint8_t _lastNotDialingVal;
		// Last pin pulsing read.
		uint8_t _lastPulsingVal;

};

}

#endif