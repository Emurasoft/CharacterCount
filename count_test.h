#include "stdafx.h"
#include "gtest/gtest.h"
#include "count.h"

namespace {

std::function<int(unsigned int c)> getWidth = [](unsigned int c) {
	return 1;
};

TEST(count, countText) {
	std::wstring text = L"àあアｱ一\n、｡゛ﾞーｰ｢・〱ゝ々〆〇";
	std::array<long, count::countsSize> result{};

	count::countText(const_cast<wchar_t*>(text.c_str()), text.size(), &result, getWidth, settings::defaultSettings);
	std::array<long, count::countsSize> expected = { {0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0} };
	ASSERT_EQ(result, expected);
}

}