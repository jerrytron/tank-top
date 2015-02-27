#ifndef MWAM_LEVEL_H
#define MWAM_LEVEL_H

#include "mwam_constants.h"

namespace mwam
{

class Level
{
	public:
		/* Public Methods */
		Level();
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