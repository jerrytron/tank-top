/*
* @Author: jerrytron
* @Date:   2015-08-17 13:19:41
* @Last Modified by:   jerrytron
* @Last Modified time: 2015-08-19 21:53:54
*/

#include "mwam_text_renderer.h"
#include "mwam_manager.h"
#include "mwam_led_set.h"

namespace mwam
{

const byte kFadeBase = 140;
const uint8_t kTextIntensity = 255; // intensity of last column of text (where text appears)
const uint8_t kTextRepeats = 15; // text displays until faded down to almost zero
const uint8_t kFadePerRepeat = 15; // how much to fade down per repeat

/* Public Methods */

TextRenderer::TextRenderer() {
}

void TextRenderer::initialize(Level* aLevel, bool aMirrorText) {
	_level = aLevel;
	_ledSet = Manager::getInstance().hardwareManager->ledSet();
	_text = NULL;
	_mirrorText = aMirrorText;
	_textPixels = kLedsPerRing * kRowsPerGlyph;
	_cycles = 0;
	_repeats = 0;
	_animating = false;
	_tile = TILE_BACKGROUND;
}

bool TextRenderer::isAnimating() {
	return _animating;
}

int TextRenderer::hexToInt(char aHex) {
	if (aHex<'0') {
		return 0;
	}
	aHex -= '0';
	if (aHex > 9) {
		aHex -= 7;
	}
	if (aHex > 15) {
		return 0;
	}
	return aHex;
}

void TextRenderer::increase(byte &aByte, byte aAmount, byte aMax = 255) {
	int r = aByte+aAmount;
	if (r > aMax) {
		aByte = aMax;
	} else {
		aByte = (byte)r;
	}
}

int TextRenderer::newMessage(const char *aText, uint16_t aLength, uint8_t aCycles, uint8_t aRepeats, int8_t aIndexOffset, int8_t aOffsetChange) {
	if (_textLength && _text) {
		delete [] _text;
		_text = NULL;
		_textLength = 0;
	}

	_cycles = aCycles;
	_repeats = aRepeats;
	_animating = true;
	_indexOffset = aIndexOffset;
	_offsetChange = aOffsetChange;
	_text = new char[aLength];
	memcpy(_text, aText, aLength);
	_textLength = aLength;

	// URL decode
	//text = "";
	int i = 0;
	char c;
	while (i < aLength) {
		if (_text[i] == '%') {
			if (aLength <= i + 2) {
				break; // end of text
			}
			// get hex
			c = (hexToInt(_text[i + 1]) << 4) + hexToInt(_text[i + 2]);
			i += 2;
		}
		// Ä = C3 84
		// Ö = C3 96
		// Ü = C3 9C
		// ä = C3 A4
		// ö = C3 B6
		// ü = C3 BC
		else if (_text[i] == 0xC3) {
			if (aLength <= i + 1) {
				break; // end of text
			}
			switch (_text[i + 1]) {
				case 0x84: c = 0x80; break; // Ä
				case 0x96: c = 0x81; break; // Ö
				case 0x9C: c = 0x82; break; // Ü
				case 0xA4: c = 0x83; break; // ä
				case 0xB6: c = 0x84; break; // ö
				case 0xBC: c = 0x85; break; // ü
				default: c = 0x7F; break; // unknown
    		}
    		i += 1;
		} else {
			c = _text[i];
		}
	    // put to output string
	    DEBUG("Char: %c", c);
		//text += String(c);
		i++;
	}
	// initiate display of new text
	_textPixelOffset = -kLedsPerRing;
	_textCycleCount = 0;
	_repeatCount = 0;
	return 1;
}


void TextRenderer::stopMessage() {
	_level->clearLevel();
	resetText();
	if (_textLength && _text) {
		delete [] _text;
		_text = NULL;
		_textLength = 0;
	}
}


void TextRenderer::resetText() {
	/*if (_textLength && _text) {
		delete [] _text;
		_text = NULL;
		_textLength = 0;
	}*/
	_cycles = 0;
	_repeats = 0;
	_tile = TILE_BACKGROUND;
	for (int i = 0; i < _textPixels; i++) {
		_level->setTileAtIndex(TILE_BACKGROUND, i);
	}
}


void TextRenderer::crossFade(byte aFader, byte aValue, byte &aOutputA, byte &aOutputB) {
	byte baseBrightness = (aValue * kFadeBase) >> 8;
	byte varBrightness = aValue - baseBrightness;
	byte fade = (varBrightness * aFader) >> 8;
	aOutputB = baseBrightness + fade;
	aOutputA = baseBrightness + (varBrightness - fade);
}


int TextRenderer::glyphIndexForChar(const char aChar) {
	int i = aChar - 0x20;
	if (i < 0 || i >= kNumGlyphs) {
		i = 95; // ASCII 0x7F-0x20
	}
	return i;
}


void TextRenderer::renderText(TileType aTile) {
	_tile = aTile;

	// fade between rows
	byte maxBright = kTextIntensity - _repeatCount * kFadePerRepeat;
	byte thisBright, nextBright;
	crossFade(255 * _textCycleCount / _cycles, maxBright, thisBright, nextBright);
	// generate vertical rows
	uint8_t activeCols = kLedsPerRing;// - 2;
	// calculate text length in pixels
	uint16_t totalTextPixels = 0;

	for (int i = 0; i < _textLength; i++) {
		// sum up width of individual chars
		totalTextPixels += kFontGlyphs[glyphIndexForChar(_text[i])].width + kGlyphSpacing;
	}
	//DEBUG("TTP: %d", totalTextPixels);

	for (int x = 0; x < kLedsPerRing; x++) {
		uint8_t column = 0;
		// determine font column
		if (x < activeCols) {
			//DEBUG("Active");
			int colPixelOffset = _textPixelOffset + x;
			//DEBUG("Col Off: %d", colPixelOffset);
			if (colPixelOffset >= 0) {
				// visible column
				// - calculate character index
				int charIndex = 0;
				int glyphOffset = colPixelOffset;
				const glyph_t *glyphP = NULL;
				while (charIndex < _textLength) {
					glyphP = &kFontGlyphs[glyphIndexForChar(_text[charIndex])];
					int cw = glyphP->width + kGlyphSpacing;
					if (glyphOffset < cw) {
						break; // found char
					}
					glyphOffset -= cw;
					charIndex++;
				}
				// now we have
				// - glyphP = the glyph,
				// - glyphOffset=column offset within that glyph (but might address a spacing column not stored in font table)
				if (charIndex < _textLength) {
					// is a column of a visible char
					if (glyphOffset < glyphP->width) {
						// fetch glyph column
						column = glyphP->cols[glyphOffset];
					}
				}
			}
		}
		// now render columns
		for (int glyphRow = 0; glyphRow < kRowsPerGlyph; glyphRow++) {
			int i;
			//int leftstep;
			if (_mirrorText) {
				i = (glyphRow + 1) * kLedsPerRing - 1 - x; // LED index, x-direction mirrored
				//leftstep = 1;
			} else {
				i = glyphRow * kLedsPerRing + x; // LED index
				//leftstep = -1;
			}
			i += _indexOffset;
			if (i >= kLedCount) {
				i -= kLedCount;
			} else if (i < 0) {
				i += kLedCount;
			}
			if (glyphRow < kRowsPerGlyph) {
				if (column & (0x40 >> glyphRow)) {
					//textLayer[i] = thisBright; DELETE
					//DEBUG("I: %d", i);
					/*i += _indexOffset;
					if (i >= kLedCount) {
						i -= kLedCount;
					} else if (i < 0) {
						i += kLedCount;
					}*/
					_level->setTileAtIndex(_tile, i);
					// also adjust pixel left to this one
					/*if (x > 0) {
						increase(textLayer[i + leftstep], nextBright, maxBright);
					}*/
					continue;
				}
			}
			_level->setTileAtIndex(TILE_BACKGROUND, i); // no text
		}
	}
	// increment
	_textCycleCount++;
	if (_textCycleCount >= _cycles) {
		_textCycleCount = 0;
		_textPixelOffset++;
		if (_textPixelOffset > totalTextPixels) {
			// text shown, check for repeats
			_repeatCount++;
			_indexOffset += _offsetChange;
			if (_repeats != 0 && _repeatCount >= _repeats) {
				// done
				if (_textLength && _text) {
					delete [] _text;
					_text = NULL;
					_textLength = 0;
					_animating = false;
				}
			} else {
				// show again
				_textPixelOffset = -kLedsPerRing;
				_textCycleCount = 0;
			}
		}
	}
}

}