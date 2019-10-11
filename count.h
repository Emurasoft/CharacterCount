#pragma once
#include "settings.h"

namespace count {
	enum {
		chars,
		selStart,
		selEnd,
		logicalLines,
		viewLines,
		lf,
		cr,
		crlf,
		width,
		halfwidth,
		fullwidth,
		halfspace,
		fullspace,
		tabCharacters,
		controlCharacters,
		cjk,
		hiragana,
		katakana,
		halfkatakana,
		countsSize
	};

	void wcharToRunes(std::vector<int>* dst, const std::wstring& src);

	void countText(const std::vector<int>& runes,
		std::array<long, countsSize>* count,
		std::function<int(unsigned int ch)> getWidth,
		const std::array<unsigned char, settings::settingsSize>& settings);

	std::array<long, countsSize> count(
		bool* selection,
		HWND editor,
		const std::array<unsigned char, settings::settingsSize>& settings);
}