#ifndef MWAM_CONST_VALUES_H
#define MWAM_CONST_VALUES_H

namespace mwam
{

// Serial Communication Commands

const uint8_t kSerialCmdMode = 'c';
const uint8_t kSerialCmdListeningMode = 'w';
const uint8_t kSerialCmdClearWiFi = 'x';
const uint8_t kSerialCmdSetFlag = 0x01;
const uint8_t kSerialCmdSetValue = 0x02;
const uint8_t kSerialCmdResetMetadata = 'm';
const uint8_t kSerialCmdEraseFlash = 'e';
const uint8_t kSerialCmdRebootUnit = 'r';
const uint8_t kSerialCmdDfuMode = 'd';

const uint8_t kSerialCmdGetVersion = 0x03;
const uint8_t kSerialCmdGetFlag = 0x04;
const uint8_t kSerialCmdGetValue = 0x05;

}

#endif