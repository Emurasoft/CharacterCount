#pragma once

namespace settings {

	const unsigned char
	zero = 0,
	one = 1,
	neither = 2,
	hiragana = 3,
	katakana = 4,
	hirakata = 5,
	halfkana = 6,
	cjk = 7,
	katahalf = 8,

	settingsSize = 14,
	eol = 0,
	stop = 1,
	halfStop = 2,
	voiced = 3,
	halfVoiced = 4,
	prolonged = 5,
	halfProlonged = 6,
	corner = 7,
	middle = 8,
	repeat = 9,
	hiraIteration = 10,
	cjkIteration = 11,
	closing = 12,
	numberZero = 13;

	const std::array<unsigned char,settingsSize> defaultSettings =
	{	zero,		// End-of-line (CR,LF,CRLF)
		neither,	// Comma and full stop (、,。)
		halfkana,	// Halfwidth full stop and comma (｡,､)
		hiragana,	// Voiced Sound Mark (゛,゜)
		halfkana,	// Halfwidth Voiced Sound Mark (ﾞ,ﾟ)
		katakana,	// Prolonged Sound Mark (ー)
		halfkana,	// Halfwidth Prolonged Sound Mark (ｰ)
		halfkana,	// Halfwidth Corner Brackets (｢,｣)
		katahalf,	// Katakana Middle Dots (・,･)
		neither,	// Vertical Kana Repeat Marks (〱,〲,〳,〴,〵)
		hirakata,	// Hiragana and Katakana Iteration Marks (ゝ,ゞ,ヽ,ヾ)
		neither,	// IDEOGRAPHIC ITERATION MARK (々)
		neither,	// IDEOGRAPHIC CLOSING MARK (〆)
		neither		// IDEOGRAPHIC NUMBER ZERO (〇)
	};

	const std::array<std::vector<unsigned char>, settingsSize> menus {{
		{zero,one},								// End-of-line (CR,LF,CRLF)
		{neither, hiragana, katakana},			// Comma and full stop (、,。)
		{neither, hiragana, katakana, halfkana},// Halfwidth full stop and comma (｡,､)
		{neither, hiragana},					// Voiced Sound Mark (゛,゜)
		{neither, hiragana, katakana, halfkana},// Halfwidth Voiced Sound Mark (ﾞ,ﾟ)
		{neither, hiragana, katakana},			// Prolonged Sound Mark (ー)
		{neither, hiragana, katakana, halfkana},// Halfwidth Prolonged Sound Mark (ｰ)
		{neither, halfkana},					// Halfwidth Corner Brackets (｢,｣)
		{neither, katahalf},					// Katakana Middle Dots (・,･)
		{neither, hiragana, katakana},			// Vertical Kana Repeat Marks (〱,〲,〳,〴,〵)
		{neither, hirakata},					// Hiragana and Katakana Iteration Marks (ゝ,ゞ,ヽ,ヾ)
		{neither, cjk},							// IDEOGRAPHIC ITERATION MARK (々)
		{neither, cjk},							// IDEOGRAPHIC CLOSING MARK (〆)
		{neither, cjk}							// IDEOGRAPHIC NUMBER ZERO (〇)
	}};

	class Window
	{
	HWND editor;
	std::array<unsigned char,settingsSize> settings;
	std::array<HWND,settingsSize> comboBoxes;
	bool reopen;

	static INT_PTR CALLBACK SettingsProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam );

	public:
		Window(	HWND editor,
				std::array<unsigned char,settingsSize> settings,
				bool reopen )
			: editor(editor)
			, settings(settings)
			, reopen(reopen)
		{};

		std::array<unsigned char,settingsSize> openSettingsWindow( HWND hwnd );
	};

}