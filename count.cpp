#include "stdafx.h"
#define EE_EXTERN_ONLY
#define ETL_FRAME_CLASS_NAME CMyFrame
#include "template/etlframe.h"
#include "CMyFrame.h"

#include "settings.h"
#include "count.h"

namespace count {
inline bool baseCharacter(int c) {
	return !(
		(0xdc00 <= c && c <= 0xdfff)	// Trailing surrogate
		|| 0x3099 == c || c == 0x309a	// Hiragana combination
		|| 0x0e31 == c					// Thai combination
		|| (0x0e33 <= c && c <= 0x0e3a)	// Thai combination
		|| (0x0e47 <= c && c <= 0x0e4e)	// Thai combination
		|| (0x0300 <= c && c <= 0x036f)	// Combining diacritical marks
		|| (0x1ab0 <= c && c <= 0x1aff)	// Combining diacritical marks extended
		|| (0x1dc0 <= c && c <= 0x1dff)	// Combining diacritical marks supplement
		|| (0x20d0 <= c && c <= 0x20ff)	// Combining diacritical marks for symbols
		|| (0xfe20 <= c && c <= 0xfe2f)	// Combining half marks
		|| c == 0x200d					// ZERO WIDTH JOINER
		|| c == 0x200c					// ZERO WIDTH NON-JOINER
		|| (0xfe00 <= c && c <= 0xfe0f)	// Variation selectors
		|| (0xe0100 <= c && c <= 0xe01ef) // Variation selectors supplement block
		|| (0x180b <= c && c <= 0x180d)	// Mongolian free variation selectors
		|| c == 0x3035						// VERTICAL KANA REPEAT MARK LOWER HALF
		);
}

// character to width cache (0 is undefined)
std::array<int, 0x30000> widthTable{ {0} };

// Parses through text and increments count.
void countText(
	const std::vector<int>& runes,
	std::array<long, countsSize>* count,
	std::function<int(unsigned int c)> getWidth,
	const std::array<unsigned char, settings::settingsSize>& settings
) {
	for (size_t i = 0; i < runes.size(); ++i) {
		if (baseCharacter(runes[i])) {
			++(*count)[chars];

			// Halfwidth/fullwidth
			if (runes[i] < widthTable.size()) { // TODO turn widthTable into a class
				if (widthTable[runes[i]] == 0) {
					widthTable[runes[i]] = getWidth(runes[i]);
				}

				switch (widthTable[runes[i]]) {
				case 1:
					++(*count)[halfwidth];
					break;
				case 2:
					++(*count)[fullwidth];
				}
			} else {
				switch (getWidth(runes[i])) {
				case 1:
					++(*count)[halfwidth];
					break;
				case 2:
					++(*count)[fullwidth];
				}
			}
		}

		// Control character range
		if ((0x0000 <= runes[i] && runes[i] <= 0x001f)
			|| 0x007F <= runes[i] && runes[i] <= 0x009f) {
			++(*count)[controlCharacters];

			if (runes[i] == '\t') {
				++(*count)[tabCharacters];
			} else if (runes[i] == '\r') {
				if (i + 1 < runes.size() && runes[i + 1] == '\n') { // CR+LF
					++(*count)[crlf];
					++i; // increment counts for LF and skip last loop
					++(*count)[chars];
					++(*count)[controlCharacters];
					++(*count)[halfwidth];
				} else { // Carriage return only
					++(*count)[cr];
				}
			} else if (runes[i] == '\n') { // Line feed only
				++(*count)[lf];
			}
		}

		// Space
		else if (runes[i] == 0x0020) {
			++(*count)[halfspace];
		}

		// Fullwidth space
		else if (runes[i] == 0x3000) {
			++(*count)[fullspace];
		}

		// Hiragana
		else if (
			0x3040 <= runes[i] && runes[i] <= 0x309F
			// Exclude characters if not setting
			&& (settings[settings::voiced] == settings::hiragana || !(0x309b == runes[i] || runes[i] == 0x309c))
			&& (settings[settings::hiraIteration] == settings::hirakata || !(runes[i] == 0x309d || runes[i] == 0x309e))
			// Include characters on setting
			|| (settings[settings::stop] == settings::hiragana && (runes[i] == 0x3001 || runes[i] == 0x3002))
			|| (settings[settings::halfStop] == settings::hiragana && (runes[i] == 0xff61 || runes[i] == 0xff64))
			|| (settings[settings::halfVoiced] == settings::hiragana && (runes[i] == 0xff9e || runes[i] == 0xff9f))
			|| (settings[settings::prolonged] == settings::hiragana && runes[i] == 0x30fc)
			|| (settings[settings::halfProlonged] == settings::hiragana && runes[i] == 0xff70)
			|| (settings[settings::repeat] == settings::hiragana && 0x3031 <= runes[i] && runes[i] <= 0x3035)
			) {
			++(*count)[hiragana];
		}

		// Katakana
		else if (
			0x30A0 <= runes[i] && runes[i] <= 0x30FF
			&& (settings[settings::prolonged] == settings::katakana || !(runes[i] == 0x30fc))
			&& (settings[settings::hiraIteration] == settings::hirakata || !(0x30fd <= runes[i] && runes[i] <= 0x30fe))
			&& (settings[settings::middle] == settings::katahalf || !(runes[i] == 0x30fb))
			|| (settings[settings::stop] == settings::katakana && (runes[i] == 0x3001 || runes[i] == 0x3002))
			|| (settings[settings::halfStop] == settings::katakana && (runes[i] == 0xff61 || runes[i] == 0xff64))
			|| (settings[settings::halfVoiced] == settings::katakana && (runes[i] == 0xff9e || runes[i] == 0xff9f))
			|| (settings[settings::halfProlonged] == settings::katakana && runes[i] == 0xff70)
			) {
			++(*count)[katakana];
		}

		// CJK unified ideograph range
		else if (
			0x4E00 <= runes[i] && runes[i] <= 0x9FFF
			|| (settings[settings::cjkIteration] == settings::cjk && runes[i] == 0x3005)
			|| (settings[settings::closing] == settings::cjk && runes[i] == 0x3006)
			|| (settings[settings::numberZero] == settings::cjk && runes[i] == 0x3007)
			) {
			++(*count)[cjk];
		}

		// Halfwidth katakana
		else if (
			0xFF61 <= runes[i] && runes[i] <= 0xFF9F
			&& (settings[settings::halfStop] == settings::halfkana || !(runes[i] == 0xff61 || runes[i] == 0xff64))
			&& (settings[settings::halfVoiced] == settings::halfkana || !(runes[i] == 0xff9e || runes[i] == 0xff9f))
			&& (settings[settings::halfProlonged] == settings::halfkana || !(runes[i] == 0xff70))
			&& (settings[settings::corner] == settings::halfkana || !(runes[i] == 0xff62 || runes[i] == 0xff63))
			&& (settings[settings::middle] == settings::katahalf || !(runes[i] == 0xff65))
			) {
			++(*count)[halfkatakana];
		}
	}
}

inline bool isLowSurrogate(wchar_t c) {
	return c >= 0xdc00 && c <= 0xdfff;
}

void wcharToRunes(std::vector<int>* dst, const std::wstring& src) {
	dst->clear();
	dst->reserve(src.size());

	for (size_t srcI = 0; srcI < src.size();) {
		if (srcI + 1 < src.size() && isLowSurrogate(src[srcI + 1])) {
			const int offset = 0x10000 - (0xD800 << 10) - 0xDC00;
			dst->push_back((src[srcI] << 10) + src[srcI + 1] + offset);
			srcI += 2;
		} else {
			dst->push_back(src[srcI]);
			srcI++;
		}
	}
}

// Returns the sums of each kind of character.
countResult
count(
	HWND editor,
	const std::array<unsigned char, settings::settingsSize>& settings
) {
	WCHAR progressTextHalf[50];
	VERIFY(LoadString(EEGetLocaleInstanceHandle(), IDS_PROGRESS, progressTextHalf, std::size(progressTextHalf)));

	POINT_PTR start;
	Editor_GetSelStart(editor, POS_LOGICAL_W, &start);

	POINT_PTR end;
	Editor_GetSelEnd(editor, POS_LOGICAL_W, &end);

	// Main table to keep track of counts
	std::array<long, countsSize> counts{ {0} };

	std::function<int(unsigned int c)> getWidth = [&](unsigned int c) {
		return Editor_IsCharHalfOrFull(editor, c);
	};

	std::wstring text;
	std::vector<int> runes;

	if (start.y == end.y && start.x == end.x) { // Whole document
		counts[logicalLines] = static_cast<long>(Editor_GetLines(editor, POS_LOGICAL_W));
		counts[viewLines] = static_cast<long>(Editor_GetLines(editor, POS_VIEW));

		GET_LINE_INFO lineInfo;

		for (long i = 0; i < counts[logicalLines]; ++i) {
			lineInfo = { 0, FLAG_LOGICAL | FLAG_WITH_CRLF, static_cast<UINT_PTR>(i), 0 };

			text.resize(static_cast<long>(Editor_GetLineW(editor, &lineInfo, NULL)) - 1);

			lineInfo.cch = text.size() + 1;
			Editor_GetLineW(editor, &lineInfo, text.data());

			wcharToRunes(&runes, text);

			countText(runes, &counts, getWidth, settings);

			if ((i & 0xffff) == 0) {
				std::wstring progressText
					= progressTextHalf
					+ std::to_wstring(static_cast<int>(static_cast<double>(i) / counts[logicalLines]
						* 100))
					+ L"%";
				Editor_SetStatusW(editor, progressText.c_str());
			}
		}
		Editor_SetStatusW(editor, L"");
	} else { // Selection
		long textSize = static_cast<long>(Editor_GetSelTextW(editor, 0, NULL));
		text.resize(textSize - 1);

		Editor_GetSelTextW(editor, textSize, text.data());

		wcharToRunes(&runes, text);

		countText(runes, &counts, getWidth, settings);
		counts[selStart] = (int)start.y + 1;
		counts[selEnd] = (int)end.y + 1;

		counts[logicalLines] = end.y - start.y + 1;

		POINT_PTR viewStart;
		Editor_GetSelStart(editor, POS_VIEW, &viewStart);

		POINT_PTR viewEnd;
		Editor_GetSelEnd(editor, POS_VIEW, &viewEnd);

		counts[viewLines] = viewEnd.y - viewStart.y + 1;
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

	return countResult{counts, start.y != end.y || start.x != end.x };
}
}