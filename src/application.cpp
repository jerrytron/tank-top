/* Includes ------------------------------------------------------------------*/
#include "application.h"
#include "mwam_constants.h"
#include "mwam_manager.h"
#include "mwam_state_controller.h"

SYSTEM_MODE(MANUAL);

/* Function prototypes -------------------------------------------------------*/

/* Variables -----------------------------------------------------------------*/
mwam::StateController *_stateController;

/* This function is called once at start up ----------------------------------*/
void setup()
{
#ifndef DEBUG_BUILD
	Serial.begin(9600);
#endif
	while(!Serial.available()) SPARK_WLAN_Loop();
	_stateController = new mwam::StateController();
	_stateController->initialize();
}

/* This function loops forever --------------------------------------------*/
void loop()
{
	//mwam::Manager::getInstance().dataManager->handleSerialData();
	mwam::Manager::getInstance().hardwareManager->updateIntervalTimers();
	if (mwam::Manager::getInstance().dataManager->runState) {
		_stateController->updateState();
	}
}

#ifdef DEBUG_BUILD
void debug_output_(const char *p)
{
    static boolean once = false;
    if (!once) {
        once = true;
        Serial.begin(9600);
    }
    Serial.print(p);
}
#endif