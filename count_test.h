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

std::string convertCountsToOutput(const std::array<long, count::countsSize>& counts) {
	return
		std::string("Characters: ") + std::to_string(counts[0]) +
		"\nLogical lines: " + std::to_string(counts[1]) +
		"\nView lines: " + std::to_string(counts[2]) +
		"\nLF: " + std::to_string(counts[3]) +
		"\nCR: " + std::to_string(counts[4]) +
		"\nCR+LF: " + std::to_string(counts[5]) +
		"\nWidth: " + std::to_string(counts[6]) +
		"\nHalfwidth character: " + std::to_string(counts[7]) +
		"\nFullwidth character: " + std::to_string(counts[8]) +
		"\nSpace: " + std::to_string(counts[9]) +
		"\nIdeographic space: " + std::to_string(counts[10]) +
		"\nTab character: " + std::to_string(counts[11]) +
		"\nControl character: " + std::to_string(counts[12]) +
		"\nCJK Unified Ideograph: " + std::to_string(counts[13]) +
		"\nHiragana: " + std::to_string(counts[14]) +
		"\nKatakana: " + std::to_string(counts[15]) +
		"\nHalfwidth katakana: " + std::to_string(counts[16]) + "\n";
}

TEST(count, countText) {
	std::wstring text = L"àあアｱ一\n、｡゛ﾞーｰ｢・〱ゝ々〆〇";
	std::array<long, count::countsSize> result{};

	count::countText(const_cast<wchar_t*>(text.c_str()), text.size(), &result, getWidth, settings::defaultSettings);
	std::array<long, count::countsSize> expected = { {21,2,2,0,0,1,34,8,13,1,1,1,3,1,3,3,5} };

	std::cout << convertCountsToOutput(result);
	ASSERT_EQ(result, expected);
}

}