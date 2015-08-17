#ifndef MWAM_LED_SET_H
#define MWAM_LED_SET_H

#include "mwam_constants.h"
#include "ElapsedTime.h"
#include "Adafruit_NeoPixel.h"

namespace mwam
{

class Level;
class Tank;

// Led States
typedef enum LedState_t {
	LED_IDLE = 0,
	LED_DELAY,
	LED_REPEAT_DELAY,
	LED_ANIMATING
} LedState;

typedef enum EaseType_t {
	EASE_LINEAR = 0,
	EASE_BLINK,
	EASE_QUAD_IN,
	EASE_CUBIC_IN,
	EASE_QUARTIC_IN,
	EASE_QUINTIC_IN,
	EASE_SIN_IN,
	EASE_EXP_IN,
	EASE_CIRCULAR_IN,
	EASE_QUAD_OUT,
	EASE_CUBIC_OUT,
	EASE_QUARTIC_OUT,
	EASE_QUINTIC_OUT,
	EASE_SIN_OUT,
	EASE_EXP_OUT,
	EASE_CIRCULAR_OUT,
	EASE_QUAD_IN_OUT,
	EASE_CUBIC_IN_OUT,
	EASE_QUARTIC_IN_OUT,
	EASE_QUINTIC_IN_OUT,
	EASE_SIN_IN_OUT,
	EASE_EXP_IN_OUT,
	EASE_CIRCULAR_IN_OUT
} EaseType;

// Can switch between an ease in and out by adding / subtracting diff.
const uint8_t kEaseInOutDiff = EASE_QUAD_OUT - EASE_QUAD_IN;

// Anim Events
typedef enum AnimEvent_t {
	EVENT_NONE = 0,
	EVENT_INVALID,
	EVENT_STARTED,
	EVENT_REPEAT_START,
	EVENT_PAUSED,
	EVENT_KILLED,
	EVENT_DONE,
	EVENT_ALL_DONE,
	EVENT_SERIES_DONE
} AnimEvent;

typedef struct Animation_t {
	uint16_t tweenTime = 0; // In millis.
	uint16_t delayTime = 0; // In millis.
	//uint16_t repeatDelay = 0; // In millis;
	Color startColor = Color(0, 0, 0);
	Color currentColor = Color(0, 0, 0);
	Color endColor = Color(0, 0, 0);
	EaseType ease = EASE_LINEAR;
	uint8_t repeats = 0;
	bool repeatForever = false;
	bool yoyo = false;
} Animation;

typedef struct Led2_t {
	Animation anim;
	//Animation* series;
	//uint8_t seriesLength = 0;
	//uint8_t seriesIndex = 0;
	//uint16_t seriesRepeats = 0;
	//bool seriesFromCurrentColor = false;
	ElapsedMillis elapsedTime = 0;
	LedState state = LED_IDLE;
	AnimEvent doneEvent = EVENT_NONE;
	//bool paused = false;
	//unsigned long pauseTime = 0;
} Led;

/*typedef struct Animation_t {
	uint16_t tweenTime = 0; // In millis.
	uint32_t delayTime = 0; // In millis.
	uint16_t repeatDelay = 0; // In millis;
	Color startColor = Color(0, 0, 0);
	Color currentColor = Color(0, 0, 0);
	Color endColor = Color(0, 0, 0);
	EaseType ease = EASE_LINEAR;
	uint16_t repeats = 0;
	bool repeatForever = false;
	bool yoyo = false;
} Animation;

typedef struct Led_t {
	Animation anim;
	Animation* series;
	uint8_t seriesLength = 0;
	uint8_t seriesIndex = 0;
	uint16_t seriesRepeats = 0;
	bool seriesFromCurrentColor = false;
	ElapsedMillis elapsedTime = 0;
	LedState state = LED_IDLE;
	AnimEvent doneEvent = EVENT_NONE;
	bool paused = false;
	unsigned long pauseTime = 0;
} Led;*/

/*typedef struct LedCallbacks_t {
	void (*animStart)(uint8_t aLedIndex);
	void (*animRepeatStart)(uint8_t aLedIndex, uint16_t aRemaining);
	void (*animDone)(uint8_t aLedIndex, uint16_t aRemaining);
	void (*animAllDone)(uint8_t aLedIndex);
	void (*animSeriesDone)(uint8_t aLedIndex);
	void (*animSetDone)(uint8_t aLedIndexes[]);
} LedCallbacks;*/

class LedSet
{
	public:
		/* Public Methods */
		LedSet();
		void initialize(uint16_t aLedCount, uint8_t aLedPin, uint8_t aLedType, uint32_t aUpdateFreq);
		void updateState(Level* aLevel);
		void updateLeds(Level* aLevel);
		void setFastUpdates(Tank* aTankOne, Tank* aTankTwo);

		uint16_t ledCount();
		void setBrightness(uint8_t aBrightness);
		Color getColor(uint16_t aLedIndex);
		void setColor(uint16_t aLedIndex, Color aColor);
		void setAllColors(Color aColor);
		void setOff(uint16_t aLedIndex);
		void setAllOff();
		void resetLeds();
		Animation getAnimation(uint16_t aLedIndex);
		//AnimEvent checkStartEvent(uint16_t aLedIndex);
		AnimEvent checkDoneEvent(uint16_t aLedIndex);
		LedState getLedState(uint16_t aLedIndex);
		bool allLedsIdle();
		bool allAnimsDone();
		//void pauseAnim(uint16_t aLedIndex);
		//void pauseAllAnims();
		//void resumeAnim(uint16_t aLedIndex);
		//void resumeAllAnims();
		void restartAnim(uint16_t aLedIndex, bool aWithDelay = false);
		void killAnim(uint16_t aLedIndex);
		void killAllAnims();
		void animateLed(uint16_t aLedIndex, Animation aAnimation, bool aFromCurrentColor = false);
		//void animateSeries(uint16_t aLedIndex, Animation aAnims[], uint8_t aLength, uint16_t aRepeat, bool aFromCurrentColor = false);
		void animateSet(uint16_t aLedIndexes[], Animation aAnims[], uint8_t aLength, uint32_t aDelayBetween, bool aFromCurrentColor = false);
		void animateSet(uint16_t aLedIndexes[], Animation aAnim, uint8_t aLength, uint32_t aDelayBetween, bool aFromCurrentColor = false);
		void animateRange(uint16_t aLowIndex, uint16_t aHighIndex, Animation aAnim, bool aCountUp, uint32_t aDelayBetween, bool aFromCurrentColor = false);
		void animateAll(Animation aAnim, bool aCountUp, uint32_t aDelayBetween, bool aFromCurrentColor = false);

		float easeLinear(float aCurrentFrame, float aEndFrame, float aStartVal, float aChangeVal);
		float easeQuadraticIn(float aCurrentFrame, float aEndFrame, float aStartVal, float aChangeVal);
		float easeQuadraticOut(float aCurrentFrame, float aEndFrame, float aStartVal, float aChangeVal);
		float easeQuadraticInOut(float aCurrentFrame, float aEndFrame, float aStartVal, float aChangeVal);
		/*float easeCubicIn(float aCurrentFrame, float aEndFrame, float aStartVal, float aChangeVal);
		float easeCubicOut(float aCurrentFrame, float aEndFrame, float aStartVal, float aChangeVal);
		float easeCubicInOut(float aCurrentFrame, float aEndFrame, float aStartVal, float aChangeVal);
		float easeQuarticIn(float aCurrentFrame, float aEndFrame, float aStartVal, float aChangeVal);
		float easeQuarticOut(float aCurrentFrame, float aEndFrame, float aStartVal, float aChangeVal);
		float easeQuarticInOut(float aCurrentFrame, float aEndFrame, float aStartVal, float aChangeVal);
		float easeQuinticIn(float aCurrentFrame, float aEndFrame, float aStartVal, float aChangeVal);
		float easeQuinticOut(float aCurrentFrame, float aEndFrame, float aStartVal, float aChangeVal);
		float easeQuinticInOut(float aCurrentFrame, float aEndFrame, float aStartVal, float aChangeVal);
		float easeSinIn(float aCurrentFrame, float aEndFrame, float aStartVal, float aChangeVal);
		float easeSinOut(float aCurrentFrame, float aEndFrame, float aStartVal, float aChangeVal);
		float easeSinInOut(float aCurrentFrame, float aEndFrame, float aStartVal, float aChangeVal);
		float easeExponentialIn(float aCurrentFrame, float aEndFrame, float aStartVal, float aChangeVal);
		float easeExponentialOut(float aCurrentFrame, float aEndFrame, float aStartVal, float aChangeVal);
		float easeExponentialInOut(float aCurrentFrame, float aEndFrame, float aStartVal, float aChangeVal);
		float easeCircularIn(float aCurrentFrame, float aEndFrame, float aStartVal, float aChangeVal);
		float easeCircularOut(float aCurrentFrame, float aEndFrame, float aStartVal, float aChangeVal);
		float easeCircularInOut(float aCurrentFrame, float aEndFrame, float aStartVal, float aChangeVal);*/

		/* Public Variables */
		bool active;

	private:
		/* Private Methods */
		//void updateLedsFast(Level* aLevel);
		void checkIfAllAnimsDone();

		Color calculateEase(EaseType aEase, float aCurrentFrame, float aEndFrame, Color aStart, Color aEnd, bool &aError);

		/* Private Variables */
		ElapsedMillis _timeElapsed;
		uint32_t _updateFreq;
		AnimEvent _doneEvent;
		Adafruit_NeoPixel* _ledSet;
		//Led* _leds;
		Led _leds[kLedCount];
		uint16_t _ledCount;
		bool _fastUpdates;
		Level* _level;
		Tank* _tankOne;
		Tank* _tankTwo;

};

}

#endif