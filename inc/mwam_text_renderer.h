#ifndef MWAM_TEXT_RENDERER_H
#define MWAM_TEXT_RENDERER_H

#include "mwam_constants.h"
#include "mwam_level.h"

namespace mwam
{

class LedSet;

class TextRenderer
{
	public:
		/* Public Methods */
		TextRenderer();
		void initialize(Level* aLevel, bool aMirrorText);

		bool isAnimating();

		int hexToInt(char aHex);
		void increase(byte &aByte, byte aAmount, byte aMax);
		int newMessage(const char *aText, uint16_t aLength, uint8_t aCycles = 1, uint8_t aRepeats = 0, int8_t aIndexOffset = 0, int8_t aOffsetChange = 0);
		void stopMessage();
		void resetText();
		void crossFade(byte aFader, byte aValue, byte &aOutputA, byte &aOutputB);
		int glyphIndexForChar(const char aChar);
		void renderText(TileType aTile);

		/* Public Variables */


	private:
		/* Private Methods */

		/* Private Variables */
		Level *_level;
		LedSet *_ledSet;
		bool _mirrorText;
		bool _animating;
		// text layer, but only one strip around the tube (ledsPerLevel) with the height of the characters (rowsPerGlyph)
		uint8_t _textPixels;
		uint8_t _textLayer[kLedsPerRing * kRowsPerGlyph];
		char* _text;
		uint8_t _cycles;
		uint8_t _repeats;
		int8_t _indexOffset;
		int8_t _offsetChange;
		TileType _tile;
		uint16_t _textLength;
		int _textPixelOffset;
		int _textCycleCount;
		int _repeatCount;

};

}

#endif