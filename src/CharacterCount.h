/*
Copyright (c) 2018 Emurasoft Inc.
Licensed under the MIT license. See LICENSE for details.
*/

/*
Unit test:
	#define _test in test.h

To update version:
	Edit post-build event copy paths
	Edit IDS_VERSION
	Update all readme files
*/

#pragma once
#include "settings.h"

namespace charCount {

	// Centers window relative to parent window.
	void CenterWindow( HWND hDlg );

	class Window
	{
	public:
		HWND editor;
		std::array<unsigned char,settings::settingsSize> settings;
		std::wstring textOutput;
		bool showWindow;
		std::mutex keyboardLock;

		static INT_PTR CALLBACK DialogProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam );

		Window( HWND window, HWND editor, bool showWindow = true );
	};
}