#ifndef MWAM_PIEZO_H
#define MWAM_PIEZO_H

#include "mwam_constants.h"
#include "ElapsedTime.h"

namespace mwam
{

class Piezo
{
	public:
		/* Public Methods */
		Piezo();
		void initialize(uint8_t aPin, uint32_t aUpdateFreq);
		void updateState();

		/* Public Variables */
		bool active;

	private:
		/* Private Methods */

		/* Private Variables */
		ElapsedMillis _timeElapsed;
		uint32_t _updateFreq;
		uint8_t _pin;

};

}

#endif