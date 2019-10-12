#include "stdafx.h"
#include "resource.h"
#include "count.h"
#include "registry.h"
#include "settings.h"
#include "CharacterCount.h"

#include "test.h"
#ifdef _test

#define EE_EXTERN_ONLY
#define ETL_FRAME_CLASS_NAME CMyFrame
#include "template/etlframe.h"
#include "CMyFrame.h"

using namespace std::string_literals;

// To test CharacterCount.
namespace test {

// Test deleting and writing to registry
std::wstring testRegistry(HWND hwnd, HWND editor) {
	const std::wstring failedMsg = L"Registry test failed.\n"s;

	settings::deleteSettings();

	HKEY key;
	if (RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\EmSoft\\EmEditorPlugIns\\CharacterCount",
		0, KEY_SET_VALUE, &key) == ERROR_SUCCESS)
		return failedMsg + L"\tRegistry values were not deleted."s;

	settings::writeSettings(editor, settings::defaultSettings);
	charCount::Window charcount(hwnd, editor, false);
	if (charcount.settings != settings::defaultSettings)
		return failedMsg + L"\tRegistry values are incorrect.";

	return L"Registry test passed."s;
}

std::wstring convertCountsToOutput(const std::array<int, count::countsSize>& counts) {
	return
		L"Characters: "s + std::to_wstring(counts[0]) +
		L"\nLogical lines: " + std::to_wstring(counts[1]) +
		L"\nView lines: " + std::to_wstring(counts[2]) +
		L"\nLF: " + std::to_wstring(counts[3]) +
		L"\nCR: " + std::to_wstring(counts[4]) +
		L"\nCR+LF: " + std::to_wstring(counts[5]) +
		L"\nWidth: " + std::to_wstring(counts[6]) +
		L"\nHalfwidth character: " + std::to_wstring(counts[7]) +
		L"\nFullwidth character: " + std::to_wstring(counts[8]) +
		L"\nSpace: " + std::to_wstring(counts[9]) +
		L"\nIdeographic space: " + std::to_wstring(counts[10]) +
		L"\nTab character: " + std::to_wstring(counts[11]) +
		L"\nControl character: " + std::to_wstring(counts[12]) +
		L"\nCJK Unified Ideograph: " + std::to_wstring(counts[13]) +
		L"\nHiragana: " + std::to_wstring(counts[14]) +
		L"\nKatakana: " + std::to_wstring(counts[15]) +
		L"\nHalfwidth katakana: " + std::to_wstring(counts[16]) + L"\n";
}

// [Files,DifferentSettings,(setOfSettings,output)]
std::vector<std::vector<std::tuple<std::array<unsigned char, settings::settingsSize>, std::wstring>>>
getSettingsAndOutputs() {
	using namespace settings;
	std::vector<std::vector<std::tuple<std::array<unsigned char, settings::settingsSize>,
		std::array<int, count::countsSize>>>>
		settingsAndCounts
	{
		// Empty document
		{
		{settings::defaultSettings,
			{{0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0}}},
		},

		// LF
		{
		{settings::defaultSettings,
			{{0,3,3,2,0,0,0,0,0,0,0,0,2,0,0,0,0}}}
		},

		// CR
		{
		{settings::defaultSettings,
			{{0,3,3,0,2,0,0,0,0,0,0,0,2,0,0,0,0}}}
		},

		// CR+LF
		{
		{settings::defaultSettings,
			{{0,3,3,0,0,2,0,0,0,0,0,0,4,0,0,0,0}}}
		},

		/*
		testDocument.txt
		àあアｱ一	 　
		、｡゛ﾞーｰ｢・〱ゝ々〆〇

		Under default settings:
		à	= halfwidth character
		あ	= fullwidth hiragana
		ア	= fullwidth katakana
		ｱ	= halfwidth katakana
		一	= fullwidth CJK
			= halfwidth tab control
			= halfwidth space
		　	= fullwidth ideographic space
			= 1 CR+LF, 1 line, 2 controls
		、	= fullwidth
		｡	= halfwidth katakana
		゛	= fullwidth hiragana
		ﾞ	= halfwidth katakana
		ー	= fullwidth katakana
		ｰ	= halfwidth katakana
		｢	= halfwidth katakana
		・	= fullwidth katakana
		〱	= fullwidth
		ゝ	= fullwidth hiragana
		々	= fullwidth
		〆	= fullwidth
		〇	= fullwidth
		*/
		{
			// 00. default
			{
				settings::defaultSettings,
				{{21,2,2,0,0,1,34,8,13,1,1,1,3,1,3,3,5}},
			},

			// 01. eol counted as character
			{
				{{one,neither,halfkana,hiragana,halfkana,katakana,halfkana,halfkana,katahalf,neither,
			hirakata,neither,neither,neither}},
				{{22,2,2,0,0,1,34,8,13,1,1,1,3,1,3,3,5}},
			},

			// 02. comma becomes hiragana
			{
				{{zero,hiragana,halfkana,hiragana,halfkana,katakana,halfkana,halfkana,katahalf,neither,
			hirakata,neither,neither,neither}},
				{{21,2,2,0,0,1,34,8,13,1,1,1,3,1,4,3,5}},
			},

			// 03. comma becomes katakana
			{
				{{zero,katakana,halfkana,hiragana,halfkana,katakana,halfkana,halfkana,katahalf,neither,
			hirakata,neither,neither,neither}},
				{{21,2,2,0,0,1,34,8,13,1,1,1,3,1,3,4,5}},
			},

			// 04. halfwidth stop is neither
			{
				{{zero,neither,neither,hiragana,halfkana,katakana,halfkana,halfkana,katahalf,neither,
			hirakata,neither,neither,neither}},
				{{21,2,2,0,0,1,34,8,13,1,1,1,3,1,3,3,4}},
			},

			// 05. halfwidth stop is hiragana
			{
				{{zero,neither,hiragana,hiragana,halfkana,katakana,halfkana,halfkana,katahalf,neither,
			hirakata,neither,neither,neither}},
				{{21,2,2,0,0,1,34,8,13,1,1,1,3,1,4,3,4}},
			},

			// 06. voiced sound mark is neither
			{
				{{zero,neither,halfkana,neither,halfkana,katakana,halfkana,halfkana,katahalf,neither,
			hirakata,neither,neither,neither}},
				{{21,2,2,0,0,1,34,8,13,1,1,1,3,1,2,3,5}},
			},

			// 07. halfwidth voiced sound mark is katakana
			{
				{{zero,neither,halfkana,hiragana,katakana,katakana,halfkana,halfkana,katahalf,neither,
			hirakata,neither,neither,neither}},
				{{21,2,2,0,0,1,34,8,13,1,1,1,3,1,3,4,4}},
			},

			// 08. prolonged sound mark is hiragana
			{
				{{zero,neither,halfkana,hiragana,halfkana,hiragana,halfkana,halfkana,katahalf,neither,
			hirakata,neither,neither,neither}},
				{{21,2,2,0,0,1,34,8,13,1,1,1,3,1,4,2,5}},
			},

			// 09. halfwidth prolonged sound mark is hiragana
			{
				{{zero,neither,halfkana,hiragana,halfkana,katakana,hiragana,halfkana,katahalf,neither,
			hirakata,neither,neither,neither}},
				{{21,2,2,0,0,1,34,8,13,1,1,1,3,1,4,3,4}},
			},

			// 10. halfwidth corner brackets is neither
			{
				{{zero,neither,halfkana,hiragana,halfkana,katakana,halfkana,neither,katahalf,neither,
			hirakata,neither,neither,neither}},
				{{21,2,2,0,0,1,34,8,13,1,1,1,3,1,3,3,4}},
			},

			// 11. katakana middle dot is neither
			{
				{{zero,neither,halfkana,hiragana,halfkana,katakana,halfkana,halfkana,neither,neither,
			hirakata,neither,neither,neither}},
				{{21,2,2,0,0,1,34,8,13,1,1,1,3,1,3,2,5}},
			},

			// 12. the three other options are all CJK
			{
				{{zero,neither,halfkana,hiragana,halfkana,katakana,halfkana,halfkana,katahalf,neither,
			hirakata,cjk,cjk,cjk}},
				{{21,2,2,0,0,1,34,8,13,1,1,1,3,4,3,3,5}},
			},

			/*
			13. eol is 1 char, comma is hiragana, halfwidth full stop is katakana, voiced sound mark
			is neither, halfwidth voiced sound mark is hiragana, halfwidth corner brackets is neither
			-> (+1 char; +1 hiragana; -1 halfwidth katakana, +1 katakana; -1 hiragana; -1 halfwidth
				katakana, +1 hiragana; -1 halfwidth katakana)
			-> (+1 char, +1 hiragana, +1 katakana, -3 halfwidth katakana)
			*/
			{
				{{one,hiragana,katakana,neither,hiragana,katakana,halfkana,neither,katahalf,neither,
			hirakata,neither,neither,neither}},
				{{22,2,2,0,0,1,34,8,13,1,1,1,3,1,4,4,2}},
			}
		}
	};

	std::vector<std::vector<std::tuple<std::array<unsigned char, settings::settingsSize>, std::wstring>>>
		returnVec(settingsAndCounts.size());
	for (int fileI = 0; fileI < settingsAndCounts.size(); ++fileI) {

		returnVec[fileI] =
			std::vector<std::tuple<std::array<unsigned char, settings::settingsSize>, std::wstring>>(
				settingsAndCounts[fileI].size());

		for (int settingI = 0; settingI < settingsAndCounts[fileI].size(); ++settingI) {

			returnVec[fileI][settingI]
				= std::make_tuple(
					std::get<0>(settingsAndCounts[fileI][settingI]),
					convertCountsToOutput(std::get<1>(settingsAndCounts[fileI][settingI]))
				);
		}
	}

	return returnVec;
}

std::wstring docPath() {
	wchar_t dirChar[300];
	// Get directory to DLL, including filename
	GetModuleFileName(EEGetLocaleInstanceHandle(), dirChar, 300);
	std::wstring docPath(dirChar);
	return docPath.substr(0, docPath.find(L"build\\x64\\Debug"s))
		+ L"testDocument.txt"s;
}

std::wstring testSettings(
	std::vector<std::tuple<std::array<unsigned char, settings::settingsSize>, std::wstring>>
	settingsAndOutputs,
	HWND hwnd,
	HWND editor
) {
	std::wstring errors;
	int i = 0;

	for (auto settingsAndOutput : settingsAndOutputs) {

		settings::writeSettings(editor, std::get<0>(settingsAndOutput));
		charCount::Window charcount(hwnd, editor, false);
		if (charcount.textOutput != std::get<1>(settingsAndOutput))
			errors += L"\t\tSetting "s + std::to_wstring(i)
			+ L" resulted in incorrect values.\n"s;
		++i;
	}

	return errors;
}

// Delete all and insert text.
void replace(HWND editor, LPCWSTR text) {
	Editor_ExecCommand(editor, EEID_EDIT_SELECT_ALL);
	Editor_ExecCommand(editor, EEID_DELETE);
	Editor_InsertW(editor, text);
}

// Test counting with various settings on various documents.
std::wstring countTest(HWND hwnd, HWND editor) {
	auto settingsAndOutputs = getSettingsAndOutputs();
	auto settingsAndOutputsIter = settingsAndOutputs.begin();

	std::wstring output;

	// Empty file test
	Editor_EditTemp(editor, L"", L"test", NULL, NULL, 65001/*UTF-8*/);
	std::wstring errors1 = testSettings(*settingsAndOutputsIter++, hwnd, editor);
	if (errors1.empty())
		output += L"Empty file test passed.\n";
	else
		output += L"Empty file test failed.\n" + errors1;

	// LR test
	replace(editor, L"\n\n");
	std::wstring errors2 = testSettings(*settingsAndOutputsIter++, hwnd, editor);
	if (errors2.empty())
		output += L"LR test passed.\n";
	else
		output += L"LR test failed.\n" + errors2;

	// CR test
	replace(editor, L"\r\r");
	std::wstring errors3 = testSettings(*settingsAndOutputsIter++, hwnd, editor);
	if (errors3.empty())
		output += L"CR test passed.\n";
	else
		output += L"CR test failed.\n" + errors3;

	// CR+LF test
	replace(editor, L"\r\n\r\n");
	std::wstring errors4 = testSettings(*settingsAndOutputsIter++, hwnd, editor);
	if (errors4.empty())
		output += L"CR+LF test passed.\n";
	else
		output += L"CR+LF test failed.\n" + errors4;

	// Document test.
	std::wstring docPath_ = docPath();
	if (Editor_LoadFileW(editor, reinterpret_cast<LOAD_FILE_INFO_EX*>(NULL), docPath_.data())) {
		std::wstring errors5 = testSettings(*settingsAndOutputsIter++, hwnd, editor);

		if (errors5.empty())
			output += L"testDocument test passed.\n";
		else
			output += L"testDocument test failed.\n" + errors5;
	} else {
		output += L"testDocument test failed.\n\ttestDocument.txt doesn't exist under \""s
			+ docPath_ + L"\"."s;
	}

	return output;
}

std::wstring test(HWND hwnd, HWND editor) {
	return L"CharacterCount testing results:\n"s
		+ testRegistry(hwnd, editor) + L"\n"s
		+ countTest(hwnd, editor);

	/*
	To check by hand:
		All controls show correct text on the interface and are working
		Keyboard shortcuts are working
		GUI looks the same in any DPI
		Page wrapping results in higher view lines count
	*/
}
}

void CMyFrame::OnCommand(HWND hwnd) {
	std::wstring output = test::test(hwnd, m_hWnd);
	Editor_EditTemp(hwnd, L"", L"CharacterCount unit test", NULL, NULL, 65001);
	Editor_InsertW(hwnd, output.c_str());
}

BOOL CMyFrame::SetUninstall(HWND /*hwnd*/, LPTSTR /*command*/, LPTSTR /*param*/) {
	settings::deleteSettings();
	return UNINSTALL_SIMPLE_DELETE;
}

BOOL CMyFrame::SetProperties(HWND hwnd) {
	settings::Window settingsWindow(m_hWnd, settings::readSettings(m_hWnd), false);
	settingsWindow.openSettingsWindow(hwnd);
	return TRUE;
}

#endif