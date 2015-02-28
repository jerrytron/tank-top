#ifndef MWAM_BULLET_H
#define MWAM_BULLET_H

#include "mwam_constants.h"
#include "ElapsedTime.h"

namespace mwam
{

class Bullet
{
	public:
		/* Public Methods */
		Bullet();
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