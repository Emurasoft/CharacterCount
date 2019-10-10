#include "stdafx.h"
#include "gtest/gtest.h"
#include "count.h"

namespace {

std::function<int(unsigned int c)> getWidth = [](unsigned int c) {
	if (c == 0x3042 || c == 0x30A2 || c == 0x4e00 || c == 0x3000 || c == 0x3001 || c == 0xff61 || c == 0x309b || c == 0x30fc || c == 0x30fb
		|| c == 0x3031 || c == 0x309d || c == 3005 || c == 0x3006 || c == 0x3007) {
		return 2;
	}
	return 1;
};

std::string countsString(const std::array<long, count::countsSize>& counts) {
	return
		std::string("Characters: ") + std::to_string(counts[count::chars]) +
		"\nSelection start:" + std::to_string(counts[count::selStart]) +
		"\nSelection end:" + std::to_string(counts[count::selEnd]) +
		"\nLogical lines: " + std::to_string(counts[count::logicalLines]) +
		"\nView lines: " + std::to_string(counts[count::viewLines]) +
		"\nLF: " + std::to_string(counts[count::lf]) +
		"\nCR: " + std::to_string(counts[count::cr]) +
		"\nCR+LF: " + std::to_string(counts[count::crlf]) +
		"\nWidth: " + std::to_string(counts[count::width]) +
		"\nHalfwidth character: " + std::to_string(counts[count::halfwidth]) +
		"\nFullwidth character: " + std::to_string(counts[count::fullwidth]) +
		"\nSpace: " + std::to_string(counts[count::halfspace]) +
		"\nIdeographic space: " + std::to_string(counts[count::fullspace]) +
		"\nTab character: " + std::to_string(counts[count::tabCharacters]) +
		"\nControl character: " + std::to_string(counts[count::controlCharacters]) +
		"\nCJK Unified Ideograph: " + std::to_string(counts[count::cjk]) +
		"\nHiragana: " + std::to_string(counts[count::hiragana]) +
		"\nKatakana: " + std::to_string(counts[count::katakana]) +
		"\nHalfwidth katakana: " + std::to_string(counts[count::halfkatakana]) + "\n";
}

TEST(count, countText) {
	std::wstring text = L"àあアｱ一	 　\r\n、｡゛ﾞーｰ｢・〱ゝ々〆〇";
	std::array<long, count::countsSize> result{};

	count::countText(const_cast<wchar_t*>(text.c_str()), text.size() + 1, &result, getWidth, settings::defaultSettings);

	result[count::logicalLines] = 2;
	result[count::viewLines] = 2;

	result[count::chars] -= result[count::controlCharacters] - result[count::tabCharacters];
	result[count::halfwidth] -= result[count::controlCharacters] - result[count::tabCharacters];

	result[count::width] = result[count::halfwidth] + 2 * result[count::fullwidth];

	if (settings::defaultSettings[settings::eol] == settings::one) {
		result[count::chars] += result[count::logicalLines] - 1;
	}

	std::array<long, count::countsSize> expected{ {21,0,0,2,2,0,0,1,34,8,13,1,1,1,3,1,3,3,5} };
	ASSERT_EQ(result, expected);
}

}