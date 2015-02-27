#ifndef MWAM_CONST_STRUCTS_H
#define MWAM_CONST_STRUCTS_H

namespace mwam
{

// Start of Header byte, ASCII SOH
const uint8_t kAsciiHeaderByte = 0x01;

const uint8_t kScoreHashIdLength = 5;
const uint8_t kScoreInitialsLength = 3;
const uint8_t kHighScoreCount = 10;
const uint16_t kMaxStoredScores = 2000;

const uint8_t kScoreHashIdOffset = 0;
const uint8_t kScoreHashIdSize = kScoreHashIdLength + 1;
const uint8_t kScoreTweetedOffset = (kScoreHashIdOffset + kScoreHashIdSize);
const uint8_t kScoreTweetedSize = 1;
const uint8_t kScoreUploadedOffset = (kScoreTweetedOffset + kScoreTweetedSize);
const uint8_t kScoreUploadedSize = 1;
const uint8_t kScoreWonGameOffset = (kScoreUploadedOffset + kScoreUploadedSize);
const uint8_t kScoreWonGameSize = 1;
const uint8_t kScoreHasRealTimeOffset = (kScoreWonGameOffset + kScoreWonGameSize);
const uint8_t kScoreHasRealTimeSize = 1; // bytes
const uint8_t kScoreInitialsOffset = (kScoreHasRealTimeOffset + kScoreHasRealTimeSize);
const uint8_t kScoreInitialsSize = kScoreInitialsLength + 1;
const uint8_t kScoreInitialStandingOffset = (kScoreInitialsOffset + kScoreInitialsSize);
const uint8_t kScoreInitialStandingSize = 1; // bytes
const uint8_t kScoreLifeRemainingOffset = (kScoreInitialStandingOffset + kScoreInitialStandingSize);
const uint8_t kScoreLifeRemainingSize = 1; // bytes
const uint8_t kScoreFinalScoreOffset = (kScoreLifeRemainingOffset + kScoreLifeRemainingSize);
const uint8_t kScoreFinalScoreSize = 2; // bytes
const uint8_t kScoreScansOffset = (kScoreFinalScoreOffset + kScoreFinalScoreSize);
const uint8_t kScoreScansSize = 2; // bytes
const uint8_t kScoreDisruptedOffset = (kScoreScansOffset + kScoreScansSize);
const uint8_t kScoreDisruptedSize = 2; // bytes
const uint8_t kScoreKillsOffset = (kScoreDisruptedOffset + kScoreDisruptedSize);
const uint8_t kScoreKillsSize = 2; // bytes
const uint8_t kScoreMissesOffset = (kScoreKillsOffset + kScoreKillsSize);
const uint8_t kScoreMissesSize = 2; // bytes
const uint8_t kScoreShotsFiredOffset = (kScoreMissesOffset + kScoreMissesSize);
const uint8_t kScoreShotsFiredSize = 2; // bytes
const uint8_t kScoreBombsUsedOffset = (kScoreShotsFiredOffset + kScoreShotsFiredSize);
const uint8_t kScoreBombsUsedSize = 2; // bytes
const uint8_t kScoreBLightKillsOffset = (kScoreBombsUsedOffset + kScoreBombsUsedSize);
const uint8_t kScoreBLightKillsSize = 2; // bytes
const uint8_t kScoreBMediumKillsOffset = (kScoreBLightKillsOffset + kScoreBLightKillsSize);
const uint8_t kScoreBMediumKillsSize = 2; // bytes
const uint8_t kScoreBHeavyKillsOffset = (kScoreBMediumKillsOffset + kScoreBMediumKillsSize);
const uint8_t kScoreBHeavyKillsSize = 2; // bytes
const uint8_t kScoreBDisruptorKillsOffset = (kScoreBHeavyKillsOffset + kScoreBHeavyKillsSize);
const uint8_t kScoreBDisruptorKillsSize = 2; // bytes
const uint8_t kScoreBMissileKillsOffset = (kScoreBDisruptorKillsOffset + kScoreBDisruptorKillsSize);
const uint8_t kScoreBMissileKillsSize = 2; // bytes
const uint8_t kScoreBBossKillsOffset = (kScoreBMissileKillsOffset + kScoreBMissileKillsSize);
const uint8_t kScoreBBossKillsSize = 2; // bytes
const uint8_t kScoreFirstHitByOffset = (kScoreBBossKillsOffset + kScoreBBossKillsSize);
const uint8_t kScoreFirstHitBySize = 1; // bytes
const uint8_t kScoreFirstHitWaveOffset = (kScoreFirstHitByOffset + kScoreFirstHitBySize);
const uint8_t kScoreFirstHitWaveSize = 1; // bytes
const uint8_t kScoreSecondHitByOffset = (kScoreFirstHitWaveOffset + kScoreFirstHitWaveSize);
const uint8_t kScoreSecondHitBySize = 1; // bytes
const uint8_t kScoreSecondHitWaveOffset = (kScoreSecondHitByOffset + kScoreSecondHitBySize);
const uint8_t kScoreSecondHitWaveSize = 1; // bytes
const uint8_t kScoreKilledByOffset = (kScoreSecondHitWaveOffset + kScoreSecondHitWaveSize);
const uint8_t kScoreKilledBySize = 1; // bytes
const uint8_t kScoreWaveReachedOffset = (kScoreKilledByOffset + kScoreKilledBySize);
const uint8_t kScoreWaveReachedSize = 1; // bytes
const uint8_t kScoreStartTimeOffset = (kScoreWaveReachedOffset + kScoreWaveReachedSize);
const uint8_t kScoreStartTimeSize = 4; // bytes
const uint8_t kScoreEndTimeOffset = (kScoreStartTimeOffset + kScoreStartTimeSize);
const uint8_t kScoreEndTimeSize = 4; // bytes

// Should be 56 bytes
const uint8_t kScoreDataSize = (kScoreEndTimeOffset + kScoreEndTimeSize);

typedef struct ScoreData_t {
	char hashId[kScoreHashIdLength + 1]; // Add 1 for null termination
	bool tweeted; // Was the score tweeted.
	bool uploaded; // Was the score uploaded to the server.
	bool wonGame; // Did the player win the game.
	bool hasRealTime; // True if actual datatime acquired;
	char initials[kScoreInitialsLength + 1]; // Player initials + 1 null term
	uint8_t standing; // The high score standing at time of play.
	uint8_t lifeRemaining; // Hits remaining, 0 unless player won.
	uint16_t finalScore; // Final calculated score.
	uint16_t scans; // Total number of radar scans.
	uint16_t disrupted; // Total hits with disruptors.
	uint16_t kills; // Total number of kills.
	uint16_t misses; // Total number of missed shots.
	uint16_t shotsFired; // Number of total shots fired.
	uint16_t bombsUsed; // Number of bombs used.
	uint16_t bLightKills;
	uint16_t bMediumKills;
	uint16_t bHeavyKills;
	uint16_t bDisruptorKills;
	uint16_t bMissileKills;
	uint16_t bBossKills;
	uint8_t firstHitBy;
	uint8_t firstHitWave;
	uint8_t secondHitBy;
	uint8_t secondHitWave;
	uint8_t killedBy; // Enemy type that killed the player.
	uint8_t waveReached; // The wave the player died on.
	int32_t startTime; // Time game started.
	int32_t endTime; // Time game ended. (can get duration even if realtime == false)
} ScoreData; // 56 bytes length

/* Structure definitions for Metadata */

const uint8_t kMetadataSohOffset = 0;
const uint8_t kMetadataSohSize = 1; // bytes
const uint8_t kMetadataRsvdOffset = (kMetadataSohOffset + kMetadataSohSize);
const uint8_t kMetadataRsvdSize = 1; // bytes
const uint8_t kMetadataScoreIndexOffset = (kMetadataRsvdOffset + kMetadataRsvdSize);
const uint8_t kMetadataScoreIndexSize = 2; // bytes
const uint8_t kMetadataTotalScoresOffset = (kMetadataScoreIndexOffset + kMetadataScoreIndexSize);
const uint8_t kMetadataTotalScoresSize = 2; // bytes
const uint8_t kMetadataFlagsOffset = (kMetadataTotalScoresOffset + kMetadataTotalScoresSize);
const uint8_t kMetadataFlagsSize = 8; // bytes
const uint8_t kMetadataValuesOffset = (kMetadataFlagsOffset + kMetadataFlagsSize);
const uint8_t kMetadataValuesSize = 16; // bytes
const uint8_t kMetadataHighScoresOffset = (kMetadataValuesOffset + kMetadataValuesSize);
const uint8_t kMetadataHighScoresSize = kHighScoreCount;

// Should be 40 bytes
const uint8_t kMetadataSize = (kMetadataHighScoresOffset + kMetadataHighScoresSize);

typedef struct MetaFlags_t {
	union {
		uint8_t flag1;
		struct {
			uint8_t cheatsOn        :1;
			uint8_t saveScores      :1;
			uint8_t saveHighScores  :1; // Depends on saveScores
			uint8_t uploadScores    :1; // Depends on saveScores
			uint8_t tweetScores     :1; // Depends on saveScores
			uint8_t sdCard			:1;
			uint8_t demo			:1;
			uint8_t offline			:1; // Bit 7
		};
	};
	union {
		uint8_t flag2;
		struct {
			uint8_t rsvd2 			:8;
		};
	};
	union {
		uint8_t flag3;
		struct {
			uint8_t rsvd3 			:8;
		};
	};
	union {
		uint8_t flag4;
		struct {
			uint8_t rsvd4			:8;
		};
	};
	union {
		uint8_t flag5;
		struct {
			uint8_t rsvd5 			:8;
		};
	};
	union {
		uint8_t flag6;
		struct {
			uint8_t rsvd6			:8;
		};
	};
	union {
		uint8_t flag7;
		struct {
			uint8_t rsvd7 			:8;
		};
	};
	union {
		uint8_t flag8;
		struct {
			uint8_t rsvd8			:8;
		};
	};
} MetaFlags;

typedef struct MetaValues_t {
	uint16_t value1;
	uint16_t value2;
	uint16_t value3;
	uint16_t value4;
	uint16_t value5;
	uint16_t value6;
	uint16_t value7;
	uint16_t value8;
} MetaValues;

typedef struct Metadata_t {
	uint8_t soh;
	uint8_t rsvd;
	uint16_t scoreIndex;
	uint16_t totalScores;
	MetaFlags flags;
	MetaValues values;
	// The indexes of the 10 highest scores.
	uint8_t highScores[kHighScoreCount];
} Metadata; // 40 bytes length

}

#endif