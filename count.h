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

	void countText(wchar_t* text,
		long textSize,
		std::array<long, countsSize>* count,
		HWND editor,
		const std::array<unsigned char, settings::settingsSize>& settings);

	std::array<long, countsSize> count(
		bool* selection,
		HWND editor,
		const std::array<unsigned char, settings::settingsSize>& settings);
}