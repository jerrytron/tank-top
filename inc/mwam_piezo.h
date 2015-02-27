#ifndef MWAM_PIEZO_H
#define MWAM_PIEZO_H

#include "mwam_constants.h"

namespace mwam
{

class Piezo
{
	public:
		/* Public Methods */
		Piezo();
		void initialize();

		/* Public Variables */
		void updateState();

		/* Public Variables */
		bool active;

	private:
		/* Private Methods */

		/* Private Variables */

};

}

#endif