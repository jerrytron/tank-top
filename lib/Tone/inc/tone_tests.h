
#ifndef TONE_TESTS_H
#define TONE_TESTS_H


namespace mwam
{

class ToneTests {

public:
	ToneTests();

	void init();
	void testComplete();
	void initTestSound();
	void startTestSound();
	void initTestMario();
	void startTestMario();

	bool isTestRunning;
private:
	// Mario Test Functions
	void playSong(int aSong);
	void buzz(int aPiezoPin, long aFrequency, long aLength);

};

}

#endif