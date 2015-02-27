#ifndef MWAM_UTILITIES_H
#define MWAM_UTILITIES_H

#include "mwam_constants.h"

namespace mwam
{

class Utils {

public:
	//static void GetBase36(uint8_t aLength, char* aBuffer);
	static void GetBase62(uint8_t aLength, char* aBuffer);
};

}

#endif