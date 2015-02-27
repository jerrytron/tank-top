#include "mwam_manager.h"
#include "mwam_state_controller.h"

namespace mwam
{

/* Public Methods */

bool Manager::initialize(StateController* aStateController) {
	bool result = dataManager->initialize(aStateController);
	hardwareManager->initialize(aStateController);
	gameManager->initialize(aStateController);
	return result;
}


/* Private Methods */
Manager::Manager() {
	dataManager = new DataManager();
	gameManager = new GameManager();
	hardwareManager = new HardwareManager();
}

}