#ifndef MWAM_TANK_H
#define MWAM_TANK_H

#include "mwam_constants.h"

namespace mwam
{

class Tank
{
	public:
		/* Public Methods */
		Tank();
		void initialize(uint32_t aUpdateFreq);
		void updateState();

		/* Public Variables */
		bool active;

	private:
		/* Private Methods */

		/* Private Variables */
		ElapsedMillis _timeElapsed;
		uint32_t _updateFreq;


};

}

#endif