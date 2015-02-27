#include "mwam_utilities.h"

namespace mwam
{

const char _base62chars[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

/*void Utils::GetBase36(uint8_t aLength, char* aBuffer) {
	for (int i = 0; i < aLength; i++) {
		//sb.Append(_base62chars[_random.Next(36)]);
	}

	return;
}*/

void Utils::GetBase62(uint8_t aLength, char* aBuffer) {
	for (int i = 0; i < aLength; i++) {
		aBuffer[i] = _base62chars[random(0, 62)];
	}

	return;
}

}