#ifndef MWAM_DATA_MANAGER_H
#define MWAM_DATA_MANAGER_H

#include "mwam_constants.h"
#include "mwam_state_controller.h"
#include "flashee-eeprom.h"
#include "ElapsedTime.h"

namespace mwam
{

class StateController;

class DataManager
{
	public:
		/* Public Methods */
		DataManager();

		bool initialize(StateController *aStateController);
		void handleSerialData();

		/* API Calls */
		// Set and write a flag.
		bool setFlag(uint8_t aFlagIndex, uint8_t aBitIndex, bool aValue);
		bool setValue(uint8_t aIndex, uint16_t aValue);
		// Resets metadata to default values.
		//bool resetMetadata();
		// Erase entire flash memory, includes metadata.
		//bool eraseFlash();

		// Accessor Functions
		StateController *stateController();
		Metadata* metadata();

		/* Public Variables */
		// Whether or not the state machine should execute it's update loop.
		bool runState;
		bool hasCredentials;

	private:
		/* Private Methods */
		/*bool uploadScore(ScoreData *aScoreData);
		bool tweetHighScore(ScoreData *aScoreData, uint8_t aStanding);
		uint16_t calculateScore(ScoreData *aScoreData);
		bool insertHighScore(Metadata *aMetadata, ScoreData *aScoreData, uint8_t aTableIndex);
		bool loadMetadata();
		bool initializeMetadata(Metadata *aMetadata);
		bool initializeHighScores(Metadata *aMetadata);
		bool readMetadata(Metadata *aMetadata);
		bool writeMetadata(Metadata *aMetadata);
		bool readScoreData(ScoreData *aScoreData, uint8_t aIndex);
		bool writeScoreData(ScoreData *aScoreData, uint8_t aIndex);*/

		/* Private Variables */
		StateController* _stateController;
		//Flashee::FlashDevice* _flashData;
		Metadata _metadata;

		ElapsedMillis _serialElapsed;
		ElapsedMillis _serialTimeout;
};

}

#endif