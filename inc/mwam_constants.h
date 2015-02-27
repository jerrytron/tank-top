
#ifndef MWAM_CONSTANTS_H
#define MWAM_CONSTANTS_H

#include "debug.h"
#include "application.h"
#include "mwam_const_config.h"
#include "mwam_const_values.h"
#include "mwam_const_structs.h"
#include "mwam_utilities.h"

namespace mwam
{

#define IsBitSet(val, bit) ((val) & (1 << (bit)) ? true : false)

}

#endif