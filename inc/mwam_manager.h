#ifndef MWAM_MANAGER_H
#define MWAM_MANAGER_H

#include "mwam_constants.h"
#include "mwam_data_manager.h"
#include "mwam_game_manager.h"
#include "mwam_hardware_manager.h"

namespace mwam
{

class Manager
{
    public:
        /* Public Methods */
        static Manager& getInstance()
        {
            // The only instance
            // Guaranteed to be lazy initialized
            // Guaranteed that it will be destroyed correctly
            static Manager instance;
            return instance;
        }

        bool initialize(StateController* aStatecontroller);

        /* Public Variables */
        DataManager *dataManager;
        GameManager *gameManager;
        HardwareManager *hardwareManager;

    private:
        /* Private Methods */
        Manager();
        // Stop the compiler generating methods of copy the object
        Manager(Manager const& copy);            // Not Implemented
        Manager& operator=(Manager const& copy); // Not Implemented

};

}

#endif