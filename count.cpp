#include "stdafx.h"
#define EE_EXTERN_ONLY
#define ETL_FRAME_CLASS_NAME CMyFrame
#include "template/etlframe.h"
#include "CMyFrame.h"

#include "settings.h"
#include "count.h"

namespace count {
inline bool isHighSurrogate(wchar_t c) {
	return c >= 0xd800 && c <= 0xdbff;
}

inline bool isLowSurrogate(wchar_t c) {
	return c >= 0xdc00 && c <= 0xdfff;
}

inline unsigned surrogateToScaler(wchar_t* c) {
	if (isLowSurrogate(c[1])) {
		const int offset = 0x10000 - (0xD800 << 10) - 0xDC00;
		return (c[0] << 10) + c[1] + offset;
	}

	return static_cast<unsigned>(*c);
}

inline bool baseCharacter(wchar_t* c) {
	return !(
		(0xdc00 <= *c && *c <= 0xdfff)	// Trailing surrogate
		|| 0x3099 == *c || *c == 0x309a	// Hiragana combination
		|| 0x0e31 == *c					// Thai combination
		|| (0x0e33 <= *c && *c <= 0x0e3a)	// Thai combination
		|| (0x0e47 <= *c && *c <= 0x0e4e)	// Thai combination
		|| (0x0300 <= *c && *c <= 0x036f)	// Combining diacritical marks
		|| (0x1ab0 <= *c && *c <= 0x1aff)	// Combining diacritical marks extended
		|| (0x1dc0 <= *c && *c <= 0x1dff)	// Combining diacritical marks supplement
		|| (0x20d0 <= *c && *c <= 0x20ff)	// Combining diacritical marks for symbols
		|| (0xfe20 <= *c && *c <= 0xfe2f)	// Combining half marks
		|| *c == 0x200d					// ZERO WIDTH JOINER
		|| *c == 0x200c					// ZERO WIDTH NON-JOINER
		|| (0xfe00 <= *c && *c <= 0xfe0f)	// Variation selectors
		|| (0xdb40 <= *c &&					// Variation selectors supplement block
			0xdd00 <= c[1] && c[1] <= 0xddef)
		|| (0x180b <= *c && *c <= 0x180d)	// Mongolian free variation selectors
		|| *c == 0x3035						// VERTICAL KANA REPEAT MARK LOWER HALF
		);
}

// Get width of wchar at c, taking into account surrogate pairs and combining characters.
inline int charWidth(wchar_t* c, wchar_t* begin, std::function<int(unsigned int c)> getWidth) {
	// wchar at c is a trailing surrogate
	if (c != begin && isHighSurrogate(c[-1]) && isLowSurrogate(c[0])) { // TODO should loop a second time for cleaner code
		return 0;
	}

	return getWidth(surrogateToScaler(c));
}

// Macros used in countText()
// Exclude characters on setting
#define ifNotSetting(key,value,match) ( settings[settings::key] == settings::value || !(match) )

// Include characters on setting
#define ifSetting(key,value,match) ( settings[settings::key] == settings::value && (match) )

// character to width cache (0 is undefined)
const size_t widthTableSize = 0x30000;
std::array<int, widthTableSize> widthTable{ {0} };

// Parses through text and increments count.
void countText(wchar_t* text,
	long textSize,
	std::array<long, countsSize>* count,
	std::function<int(unsigned int c)> getWidth,
	const std::array<unsigned char, settings::settingsSize>& settings) {
	wchar_t* end = text + textSize - 1;

	for (wchar_t* pos = text; pos < end; ++pos) {

		// Number of characters
		if (pos == text || baseCharacter(pos)) {
			++(*count)[chars];

			// Halfwidth/fullwidth
			if (*pos < widthTableSize) {
				if (widthTable[*pos] == 0) {
					widthTable[*pos] = charWidth(pos, text, getWidth);
				}

				if (widthTable[*pos] == 1) {
					++(*count)[halfwidth];
				} else if (widthTable[*pos] == 2) {
					++(*count)[fullwidth];
				}
			} else {
				switch (charWidth(pos, text, getWidth)) {
				case 1:
					++(*count)[halfwidth];
					break;
				case 2:
					++(*count)[fullwidth];
				}
			}
		}

		// Control character range
		if ((0x0000 <= *pos && *pos <= 0x001f)
			|| 0x007F <= *pos && *pos <= 0x009f) {
			++(*count)[controlCharacters];

			if (*pos == 0x9) { // Tab character
				++(*count)[tabCharacters];
			} else if (*pos == 0xd) {
				if (*(pos + 1) == 0xa) { // CR+LF
					++(*count)[crlf];
					++pos; // increment counts for LF and skip last loop
					++(*count)[chars];
					++(*count)[controlCharacters];
					++(*count)[halfwidth];
				} else { // Carriage return only
					++(*count)[cr];
				}
			} else if (*pos == 0xa) { // Line feed only
				++(*count)[lf];
			}
		}

		// Space
		else if (*pos == 0x0020) {
			++(*count)[halfspace];
		}

		// Fullwidth space
		else if (*pos == 0x3000) {
			++(*count)[fullspace];
		}

		// Hiragana
		else if (0x3040 <= *pos && *pos <= 0x309F
			&& (
				ifNotSetting(voiced, hiragana, 0x309b <= *pos && *pos <= 0x309c)
				&& ifNotSetting(hiraIteration, hirakata, 0x309d <= *pos && *pos <= 0x309e)
				)
			|| (
				ifSetting(stop, hiragana, 0x3001 <= *pos && *pos <= 0x3002)
				|| ifSetting(halfStop, hiragana, *pos == 0xff61 || *pos == 0xff64)
				|| ifSetting(halfVoiced, hiragana, 0xff9e <= *pos && *pos <= 0xff9f)
				|| ifSetting(prolonged, hiragana, *pos == 0x30fc)
				|| ifSetting(halfProlonged, hiragana, *pos == 0xff70)
				|| ifSetting(repeat, hiragana, 0x3031 <= *pos && *pos <= 0x3035)
				)
			) {
			++(*count)[hiragana];
		}

		// Katakana
		else if (0x30A0 <= *pos && *pos <= 0x30FF
			&& (
				ifNotSetting(prolonged, katakana, *pos == 0x30fc)
				&& ifNotSetting(hiraIteration, hirakata, 0x30fd <= *pos && *pos <= 0x30fe)
				&& ifNotSetting(middle, katahalf, *pos == 0x30fb)
				)
			|| (
				ifSetting(stop, katakana, 0x3001 <= *pos && *pos <= 0x3002)
				|| ifSetting(halfStop, katakana, *pos == 0xff61 || *pos == 0xff64)
				|| ifSetting(halfVoiced, katakana, 0xff9e <= *pos && *pos <= 0xff9f)
				|| ifSetting(halfProlonged, katakana, *pos == 0xff70)
				)
			) {
			++(*count)[katakana];
		}

		// CJK unified ideograph range
		else if (0x4E00 <= *pos && *pos <= 0x9FFF
			|| (
				ifSetting(cjkIteration, cjk, *pos == 0x3005)
				|| ifSetting(closing, cjk, *pos == 0x3006)
				|| ifSetting(numberZero, cjk, *pos == 0x3007)
				)
			) {
			++(*count)[cjk];
		}

		// Halfwidth katakana
		else if (0xFF61 <= *pos && *pos <= 0xFF9F
			&& (
				ifNotSetting(halfStop, halfkana, *pos == 0xff61 || *pos == 0xff64)
				&& ifNotSetting(halfVoiced, halfkana, 0xff9e <= *pos && *pos <= 0xff9f)
				&& ifNotSetting(halfProlonged, halfkana, *pos == 0xff70)
				&& ifNotSetting(corner, halfkana, 0xff62 <= *pos && *pos <= 0xff63)
				&& ifNotSetting(middle, katahalf, *pos == 0xff65)
				)
			) {
			++(*count)[halfkatakana];
		}
	}
}

// Returns the sums of each kind of character.
std::array<long, countsSize>
count(bool* selection,
	HWND editor,
	const std::array<unsigned char, settings::settingsSize>& settings) {
	WCHAR progressTextHalf[50];
	// TODO
	VERIFY(LoadString(EEGetLocaleInstanceHandle(), IDS_PROGRESS, progressTextHalf, 50));

	POINT_PTR start;
	Editor_GetSelStart(editor, POS_LOGICAL_W, &start);

	POINT_PTR end;
	Editor_GetSelEnd(editor, POS_LOGICAL_W, &end);

	// Main table to keep track of counts
	std::array<long, countsSize> counts{ {0} };

	std::function<int(unsigned int c)> getWidth = [&](unsigned int c) {
		return Editor_IsCharHalfOrFull(editor, c);
	};

	if (start.y == end.y && start.x == end.x) { // Whole document
		*selection = false;

		counts[logicalLines] = static_cast<long>(Editor_GetLines(editor, POS_LOGICAL_W));
		counts[viewLines] = static_cast<long>(Editor_GetLines(editor, POS_VIEW));

		GET_LINE_INFO lineInfo;

		for (long i = 0; i < counts[logicalLines]; ++i) {
			lineInfo = { 0, FLAG_LOGICAL | FLAG_WITH_CRLF, static_cast<UINT_PTR>(i), 0 };

			long textSize = static_cast<long>(Editor_GetLineW(editor, &lineInfo, NULL));
			wchar_t* text = new wchar_t[textSize];
			lineInfo.cch = textSize;
			Editor_GetLineW(editor, &lineInfo, text);

			countText(text, textSize, &counts, getWidth, settings);

			if ((i & 0xffff) == 0) {
				std::wstring progressText
					= progressTextHalf
					+ std::to_wstring(static_cast<int>(static_cast<double>(i) / counts[logicalLines]
						* 100))
					+ L"%";
				Editor_SetStatusW(editor, progressText.c_str());
			}
			delete[] text;
		}
		Editor_SetStatusW(editor, L"");
	} else { // Selection
		*selection = true;

		long textSize = static_cast<long>(Editor_GetSelTextW(editor, 0, NULL));
		wchar_t* text = new wchar_t[textSize];
		VERIFY(text);
		Editor_GetSelTextW(editor, textSize, text);

		countText(text, textSize, &counts, getWidth, settings);
		counts[selStart] = (int)start.y + 1;
		counts[selEnd] = (int)end.y + 1;

		counts[logicalLines] = end.y - start.y + 1;
		
		POINT_PTR viewStart;
		Editor_GetSelStart(editor, POS_VIEW, &viewStart);

		POINT_PTR viewEnd;
		Editor_GetSelEnd(editor, POS_VIEW, &viewEnd);

		counts[viewLines] = viewEnd.y - viewStart.y + 1;

		delete[] text;
	}

	// Don't count control characters (except tabs) in character sum
	counts[chars] -= counts[controlCharacters] - counts[tabCharacters];
	counts[halfwidth] -= counts[controlCharacters] - counts[tabCharacters];

	// This shouldn't happpen, but you never know.
	VERIFY(counts[chars] >= 0 && counts[halfwidth] >= 0);
	if (counts[chars] < 0) {
		counts[chars] = 0;
	}

	if (counts[halfwidth] < 0) {
		counts[halfwidth] = 0;
	}

	// Width sum
	counts[width] = counts[halfwidth] + 2 * counts[fullwidth];

	// Count eol as 1 character setting
	if (settings[settings::eol] == settings::one) {
		counts[chars] += counts[logicalLines] - 1;
	}

	return counts;
}
}