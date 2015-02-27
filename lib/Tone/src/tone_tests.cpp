#include "tone_tests.h"
#include "tone_mario.h"
#include "mwam_constants.h"

namespace mwam
{

#define PIEZO A0

ToneTests::ToneTests() {
	isTestRunning = false;
}

void ToneTests::init() {
	/*pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_BLUE, HIGH);*/
}

void ToneTests::testComplete() {
	this->isTestRunning = false;
}

void ToneTests::initTestSound() {
  pinMode(PIEZO, OUTPUT);
}

void ToneTests::startTestSound() {
	this->isTestRunning = true;

}

void ToneTests::initTestMario() {
  Serial.println("initTestMario");
  pinMode(PIEZO, OUTPUT);
}

void ToneTests::startTestMario() {
	initTestMario();
  Serial.println("startTestMario");
	this->isTestRunning = true;
	playSong(1);
	playSong(2);
	testComplete();
}


//

void ToneTests::playSong(int aSong) {
   // iterate over the notes of the melody:
   if (aSong == 2) {
		Serial.println(" 'Underworld Theme'");
		int size = sizeof(underworld_melody) / sizeof(int);
		for (int thisNote = 0; thisNote < size; thisNote++) {
	        // to calculate the note duration, take one second
	        // divided by the note type.
	        //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
	        int noteDuration = 1000/underworld_tempo[thisNote];

	        buzz(PIEZO, underworld_melody[thisNote],noteDuration);

	        // to distinguish the notes, set a minimum time between them.
	        // the note's duration + 30% seems to work well:
	        int pauseBetweenNotes = noteDuration * 1.30;
	        delay(pauseBetweenNotes);

	        // stop the tone playing:
	        buzz(PIEZO, 0,noteDuration);
    	}
   } else {
     	Serial.println(" 'Mario Theme'");
     	int size = sizeof(melody) / sizeof(int);
     	for (int thisNote = 0; thisNote < size; thisNote++) {
       		// to calculate the note duration, take one second
       		// divided by the note type.
       		//e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
       		int noteDuration = 1000/tempo[thisNote];

       		buzz(PIEZO, melody[thisNote], noteDuration);

       		// to distinguish the notes, set a minimum time between them.
       		// the note's duration + 30% seems to work well:
       		int pauseBetweenNotes = noteDuration * 1.30;
       		delay(pauseBetweenNotes);

       		// stop the tone playing:
       		buzz(PIEZO, 0, noteDuration);
    	}
  	}
}

void ToneTests::buzz(int aPiezoPin, long aFrequency, long aLength) {
  	long delayValue = 1000000 / aFrequency / 2; // calculate the delay value between transitions
  	//// 1 second's worth of microseconds, divided by the frequency, then split in half since
  	//// there are two phases to each cycle
  	long numCycles = aFrequency * aLength / 1000; // calculate the number of cycles for proper timing
  	//// multiply frequency, which is really cycles per second, by the number of seconds to
  	//// get the total number of cycles to produce
  	for (long i = 0; i < numCycles; i++){ // for the calculated length of time...
    	digitalWrite(aPiezoPin, HIGH); // write the buzzer pin high to push out the diaphram
    	delayMicroseconds(delayValue); // wait for the calculated delay value
    	digitalWrite(aPiezoPin, LOW); // write the buzzer pin low to pull back the diaphram
    	delayMicroseconds(delayValue); // wait again or the calculated delay value
  	}
}

}