#include "stdafx.h"
#include "gtest/gtest.h"
#include "count.h"

HINSTANCE EEGetLocaleInstanceHandle() { // Resolves linking issue
	return NULL;
}

namespace {

TEST(count, wcharToRunes) {
	struct test {
		const std::wstring src;
		const std::vector<int> expected;
	};

	std::vector<test> tests{
		{
			L"",
			{},
		},
		{
			L"a",
			{'a'},
		},
		{
			L"aa",
			{'a', 'a'},
		},
		{
			L"\xd800\xdc00",
			{0x10000},
		},
		{
			L"\xd800\xdc00\xd800\xdc00",
			{0x10000,0x10000},
		},
		{
			L"\xdc00",
			{0xdc00},
		},
	};

	std::vector<int> dst;
	for (size_t i = 0; i < tests.size(); ++i) {
		count::wcharToRunes(&dst, tests[i].src);
		EXPECT_EQ(dst, tests[i].expected) << "i=" << i;
	}
}

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
	std::vector<std::tuple<std::array<unsigned char, settings::settingsSize>, std::array<long, count::countsSize>>> tests{
		{ // 00. default
			settings::defaultSettings,
			{21,0,0,2,2,0,0,1,34,8,13,1,1,1,3,1,3,3,5},
		},
		{ // 1. eol counted as character
			{
				settings::one,settings::neither,settings::halfkana,settings::hiragana,settings::halfkana,settings::katakana,
				settings::halfkana,settings::halfkana,settings::katahalf,settings::neither, settings::hirakata,
				settings::neither,settings::neither,settings::neither,
			},
			{22,0,0,2,2,0,0,1,34,8,13,1,1,1,3,1,3,3,5},
		},
		{ // 2. comma becomes hiragana
			{
				settings::zero,settings::hiragana,settings::halfkana,settings::hiragana,settings::halfkana,
				settings::katakana,settings::halfkana,settings::halfkana,settings::katahalf,settings::neither,
				settings::hirakata,settings::neither,settings::neither,settings::neither,
			},
			{21,0,0,2,2,0,0,1,34,8,13,1,1,1,3,1,4,3,5},
		},
		{ // 3. comma becomes katakana
			{
				settings::zero,settings::katakana,settings::halfkana,settings::hiragana,settings::halfkana,
				settings::katakana,settings::halfkana,settings::halfkana,settings::katahalf,settings::neither,
				settings::hirakata,settings::neither,settings::neither,settings::neither,
			},
			{21,0,0,2,2,0,0,1,34,8,13,1,1,1,3,1,3,4,5},
		},
		{ // 4. halfwidth stop is neither
			{
				settings::zero,settings::neither,settings::neither,settings::hiragana,settings::halfkana,
				settings::katakana,settings::halfkana,settings::halfkana,settings::katahalf,settings::neither,
		settings::hirakata,settings::neither,settings::neither,settings::neither,
			},
			{21,0,0,2,2,0,0,1,34,8,13,1,1,1,3,1,3,3,4},
		},
		{ // 5. halfwidth stop is hiragana
			{
				settings::zero,settings::neither,settings::hiragana,settings::hiragana,settings::halfkana,
				settings::katakana,settings::halfkana,settings::halfkana,settings::katahalf,settings::neither,
		settings::hirakata,settings::neither,settings::neither,settings::neither,
			},
			{21,0,0,2,2,0,0,1,34,8,13,1,1,1,3,1,4,3,4},
		},
		{ // 6. voiced sound mark is neither
			{
				settings::zero,settings::neither,settings::halfkana,settings::neither,settings::halfkana,
				settings::katakana,settings::halfkana,settings::halfkana,settings::katahalf,settings::neither,
		settings::hirakata,settings::neither,settings::neither,settings::neither,
			},
			{21,0,0,2,2,0,0,1,34,8,13,1,1,1,3,1,2,3,5},
		},
		{ // 7. halfwidth voiced sound mark is katakana
			{
				settings::zero,settings::neither,settings::halfkana,settings::hiragana,settings::katakana,
				settings::katakana,settings::halfkana,settings::halfkana,settings::katahalf,settings::neither,
		settings::hirakata,settings::neither,settings::neither,settings::neither,
			},
			{21,0,0,2,2,0,0,1,34,8,13,1,1,1,3,1,3,4,4},
		},
		{ // 8. prolonged sound mark is hiragana
			{
				settings::zero,settings::neither,settings::halfkana,settings::hiragana,settings::halfkana,
				settings::hiragana,settings::halfkana,settings::halfkana,settings::katahalf,settings::neither,
		settings::hirakata,settings::neither,settings::neither,settings::neither,
			},
			{21,0,0,2,2,0,0,1,34,8,13,1,1,1,3,1,4,2,5},
		},
		{ // 9. halfwidth prolonged sound mark is hiragana
			{
				settings::zero,settings::neither,settings::halfkana,settings::hiragana,settings::halfkana,
				settings::katakana,settings::hiragana,settings::halfkana,settings::katahalf,settings::neither,
		settings::hirakata,settings::neither,settings::neither,settings::neither,
			},
			{21,0,0,2,2,0,0,1,34,8,13,1,1,1,3,1,4,3,4},
		},
		{ // 10. halfwidth corner brackets is neither
			{
				settings::zero,settings::neither,settings::halfkana,settings::hiragana,settings::halfkana,
				settings::katakana,settings::halfkana,settings::neither,settings::katahalf,settings::neither,
		settings::hirakata,settings::neither,settings::neither,settings::neither,
			},
			{{21,0,0,2,2,0,0,1,34,8,13,1,1,1,3,1,3,3,4}},
		},
		{ // 11. settings::katakana middle dot is neither
			{
				settings::zero,settings::neither,settings::halfkana,settings::hiragana,settings::halfkana,
				settings::katakana,settings::halfkana,settings::halfkana,settings::neither,settings::neither,
		settings::hirakata,settings::neither,settings::neither,settings::neither,
			},
			{21,0,0,2,2,0,0,1,34,8,13,1,1,1,3,1,3,2,5},
		},
		{ // 12. the three other options are all cjk
			{
				settings::zero,settings::neither,settings::halfkana,settings::hiragana,settings::halfkana,
				settings::katakana,settings::halfkana,settings::halfkana,settings::katahalf,settings::neither,
		settings::hirakata,settings::cjk,settings::cjk,settings::cjk,
			},
			{21,0,0,2,2,0,0,1,34,8,13,1,1,1,3,4,3,3,5},
		},

		/*
		13. eol is 1 char, comma is hiragana, halfwidth full stop is katakana, voiced sound mark
		is neither, halfwidth voiced sound mark is hiragana, halfwidth corner brackets is neither
		-> (+1 char; +1 hiragana; -1 halfwidth katakana, +1 katakana; -1 hiragana; -1 halfwidth
			katakana, +1 hiragana; -1 halfwidth katakana)
		-> (+1 char, +1 hiragana, +1 katakana, -3 halfwidth katakana)
		*/
		{
			{
				settings::one,settings::hiragana,settings::katakana,settings::neither,settings::hiragana,
				settings::katakana,settings::halfkana,settings::neither,settings::katahalf,settings::neither,
		settings::hirakata,settings::neither,settings::neither,settings::neither,
			},
			{22,0,0,2,2,0,0,1,34,8,13,1,1,1,3,1,4,4,2},
		},
	};

	for (size_t i = 0; i < tests.size(); ++i) {
		std::wstring text = L"àあアｱ一	 　\r\n、｡゛ﾞーｰ｢・〱ゝ々〆〇";
		std::array<long, count::countsSize> result{};

		count::countText(const_cast<wchar_t*>(text.c_str()), text.size() + 1, &result, getWidth, std::get<0>(tests[i]));

		result[count::logicalLines] = 2;
		result[count::viewLines] = 2;

		result[count::chars] -= result[count::controlCharacters] - result[count::tabCharacters];
		result[count::halfwidth] -= result[count::controlCharacters] - result[count::tabCharacters];

		result[count::width] = result[count::halfwidth] + 2 * result[count::fullwidth];

		if (std::get<0>(tests[i])[settings::eol] == settings::one) {
			result[count::chars] += result[count::logicalLines] - 1;
		}

		EXPECT_EQ(result, std::get<1>(tests[i])) << "i=" << i;
	}
}

}