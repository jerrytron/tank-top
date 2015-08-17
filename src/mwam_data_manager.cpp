#include "mwam_data_manager.h"
#include "mwam_constants.h"
#include "mwam_state_controller.h"

extern char* itoa(int a, char* buffer, unsigned char radix);

namespace mwam
{

/* Public Methods */

DataManager::DataManager() {}

bool DataManager::initialize(StateController *aStateController) {
	_stateController = aStateController;
	this->runState = true;
	this->hasCredentials = false;

	return true;
}

void DataManager::handleSerialData() {
	if (!SPARK_FLASH_UPDATE && (_serialElapsed >= kIntervalSerialMillis)) {
		if (Serial.available()) {
			uint8_t cmd = Serial.read();

			if (cmd == kSerialCmdMode) {
				Serial.println("Command Mode");
				_serialTimeout = 0;
				while (!Serial.available()) {
					if (_serialTimeout >= kTimeoutSerialMillis) {
						//Serial.println("Timeout");
						return;
					}
				}
				cmd = Serial.read();

				switch (cmd)
				{
					case kSerialCmdListeningMode: {
						WiFi.listen();
						break;
					}
					case kSerialCmdClearWiFi: {
						//Serial.println("Clearing WiFi creds.");
						WiFi.disconnect();
						WiFi.clearCredentials();
						WiFi.off();
						break;
					}
					case kSerialCmdRebootUnit: {
						stateController()->changeState(STATE_INIT);
						break;
					}
					case kSerialCmdDfuMode: {
						FLASH_OTA_Update_SysFlag = 0x0000;
						Save_SystemFlags();
						BKP_WriteBackupRegister(BKP_DR10, 0x0000);
						USB_Cable_Config(DISABLE);
						NVIC_SystemReset();
						break;
					}
				}
			}
		}
	}
}

/* API Calls */


/* Accessors */

StateController* DataManager::stateController() {
	return _stateController;
}

/* Private Methods */

}