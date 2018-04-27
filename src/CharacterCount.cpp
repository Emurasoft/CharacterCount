/*
Copyright (c) 2018 Emurasoft Inc.
Licensed under the MIT license. See LICENSE for details.
*/

#include "stdafx.h"

#include "count.h"
#include "registry.h"
#include "CharacterCount.h"

#define EE_EXTERN_ONLY
#define ETL_FRAME_CLASS_NAME CMyFrame
#include "etlframe.h"
#include "CMyFrame.h"

namespace charCount {

// Centers window. Copied over from other projects.
void CenterWindow( HWND hDlg )
{
	RECT myrt, prrt;
	HWND hWndParent = GetParent( hDlg );
	if( !hWndParent || IsIconic( hWndParent ) ) {
		hWndParent = GetDesktopWindow();
	}
	if( GetWindowRect( hWndParent, &prrt ) && GetWindowRect( hDlg, &myrt ) ) {
		SetWindowPos( hDlg, NULL,
					  prrt.left + (((prrt.right - prrt.left) - (myrt.right - myrt.left)) / 2),
					  prrt.top + (((prrt.bottom - prrt.top) - (myrt.bottom - myrt.top)) / 2),
					  0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE );
	}
}

// Translates control based on selection state and window size.
void moveControl( HWND hwnd, double cy, bool selection )
{
	RECT clientRect;
	GetClientRect( hwnd,&clientRect );
	MapWindowPoints( hwnd, GetParent(hwnd), (LPPOINT)&clientRect, 2 );
	SetWindowPos( hwnd, NULL,
				  clientRect.left, clientRect.top - static_cast<int>(!selection * cy * 1.6),
				  0, 0, SWP_NOSIZE );
}

// Converts count index to row and column on gui.
std::tuple<int,int> rowAndCol( int i, bool skip23 )
{
	int row;
	if (i >= count::halfspace)
		row = i - count::halfspace;
	else if (skip23 && i > count::selEnd)
		row = i - 2;
	else
		row = i;

	int col;
	if (i >= count::halfspace) {
		col = 1;
	}
	else {
		col = 0;

		if (i >= count::selStart)
			row++;
		if (i >= count::width)
			row++;
	}

	return std::tuple<int,int>(row, col);
}

// Returns position and size for left column text.
RECT leftColumn( double cy, int i, bool skip23, bool narrow )
{
	std::tuple<int,int> rowAndCol_ = rowAndCol(i, skip23);

	double left = cy * (0.8 + 0.7 * narrow + std::get<1>(rowAndCol_) * 8.5);
	double top = cy * 2.0 + std::get<0>(rowAndCol_) * cy * 0.8;
	double width_ = cy * 6.0;
	double height = cy * 0.7;

	return { (LONG)left,(LONG)top,(LONG)width_,(LONG)height };
}

// Returns position and size for right column text.
RECT rightColumn( double cy, int i, bool skip23, bool narrow )
{
	std::tuple<int,int> rowAndCol_ = rowAndCol(i, skip23);

	double left = cy * (5.2 + 0.65 * narrow + std::get<1>(rowAndCol_) * 9.1);
	double top = cy * 2.03 + std::get<0>(rowAndCol_) * cy * 0.8;
	double width_ = cy * 3.3 - cy * 1.5 * narrow;
	double height = cy * 0.7;

	return { (LONG)left,(LONG)top,(LONG)width_,(LONG)height };
}

// Adds thousands commas
std::wstring withCommas(std::wstring number)
{
	size_t length = number.length();

	for (size_t i = 0; i < (length-1) / 3; i++)
		number.insert(length - (i+1) * 3, L",");
	
	return number;
}

// Initialize dialog window. Returns text output for copy button.
std::wstring initDialog( HWND hwnd, HWND editor,
						 const std::array<unsigned char,settings::settingsSize>& settings )
{
	CenterWindow( hwnd );

	// Title
	HINSTANCE localeInstanceHandle = EEGetLocaleInstanceHandle();
	WCHAR title[25];
	VERIFY( LoadString( localeInstanceHandle, IDS_PLUGIN_NAME, title, 25 ) );
	SetWindowText( hwnd, title );

	// Button text
	WCHAR closeText[25];
	VERIFY( LoadString( localeInstanceHandle, IDS_CLOSE, closeText, 25 ) );
	HWND closeButton = GetDlgItem( hwnd, IDCANCEL );
	SetWindowText( closeButton, closeText );

	WCHAR copyText[25];
	VERIFY( LoadString( localeInstanceHandle, IDS_COPY, copyText, 25 ) );
	HWND copyButton = GetDlgItem( hwnd, IDCOPY );
	SetWindowText( copyButton, copyText );

	WCHAR settingsText[25];
	VERIFY( LoadString( localeInstanceHandle, IDS_SETTINGS, settingsText, 25 ) );
	std::wstring settingsLinkText = L"<a href=\"settings\">" + std::wstring(settingsText) + L"</a>";
	HWND settingsLink = GetDlgItem( hwnd, IDSETTINGSLINK );
	SetWindowText( settingsLink, settingsLinkText.c_str() );

	// Count
	bool selection;
	std::array<long,count::countsSize> counts = count::count( &selection, editor, settings );

	// DPI dependent measurement unit
	RECT buttonRect;
	GetClientRect( closeButton, &buttonRect );
	double cy = buttonRect.bottom;

	// Resize window
	RECT windowRect;
	GetWindowRect( hwnd, &windowRect );
	SetWindowPos( hwnd, NULL, 0, 0,
				  static_cast<int>(cy * 18.9318182),
				  windowRect.bottom - windowRect.top - static_cast<int>(!selection * cy * 1.6),
				  SWP_NOMOVE );

	moveControl( closeButton, cy, selection );
	moveControl( copyButton, cy, selection );
	moveControl( settingsLink, cy, selection );

	// Heading
	WCHAR heading[25];
	int idsHeading;
	if( selection )
		idsHeading = IDS_HEADINGSELECTED;
	else
		idsHeading = IDS_HEADINGDOCUMENT;
	VERIFY( LoadString( localeInstanceHandle, idsHeading, heading, 25 ) );

	RECT clientRect;
	GetClientRect( hwnd, &clientRect );
	HFONT font = (HFONT)SendMessage( hwnd, WM_GETFONT, 0, 0 );
	HWND text = CreateWindow( L"STATIC", heading, WS_VISIBLE | WS_CHILD | SS_CENTER,
							  0, static_cast<int>(cy / 2),
							  clientRect.right - clientRect.left, static_cast<int>(cy),
							  hwnd, NULL, NULL, NULL );
	SendMessage( text, WM_SETFONT, (WPARAM)font, NULL );

	// If small numbers, decrease space between columns
	bool narrowUI = *std::max_element( counts.begin(), counts.end() ) < 100000;

	// for testing GUI
	#ifdef _DEBUG
		//std::fill(counts.begin(), counts.end(), INT_MAX); narrowUI = false;
		//std::fill(counts.begin(), counts.end(), 80000);
	#endif

	// Draw text
	std::wstring textOutput;

	for( int i = 0; i < count::countsSize; i++ ) {
		if( selection || (i != count::selStart && i != count::selEnd) ) {

			WCHAR label[25];
			VERIFY( LoadString( localeInstanceHandle, IDS_LABEL100 + i, label, 25 ) );
			RECT lCol = leftColumn( cy, i, !selection, narrowUI );
			text = CreateWindow( L"STATIC", label, WS_VISIBLE | WS_CHILD,
								 lCol.left, lCol.top, lCol.right, lCol.bottom,
								 hwnd, NULL, NULL, NULL );
			SendMessage( text, WM_SETFONT, (WPARAM)font, NULL );

			std::wstring number = std::to_wstring(counts[i]);

			RECT rCol = rightColumn( cy, i, !selection, narrowUI );
			text = CreateWindow( L"EDIT", withCommas(number).c_str(),
								 WS_CHILD | WS_VISIBLE | ES_READONLY | ES_RIGHT,
								 rCol.left, rCol.top, rCol.right, rCol.bottom,
								 hwnd, NULL, NULL, NULL );
			SendMessage( text, WM_SETFONT, (WPARAM)font, NULL );

			// Used in copyInfo
			textOutput += label + std::wstring( L" " ) + number + std::wstring( L"\n" );
		}
	}

	return textOutput;
}

// When copy button is pressed.
void copyInfo( HWND hwnd, HWND editor, const std::wstring& textOutput )
{
	if( Editor_InsertClip( editor, textOutput.c_str(),
			Editor_GetClipPos( editor ), SEL_TYPE_LINE ) == -1 ) {
		WCHAR errorMsg[25];
		VERIFY( LoadString( EEGetLocaleInstanceHandle(), IDS_NOTCOPIED, errorMsg, 25 ) );
		MessageBox( hwnd, errorMsg, NULL, MB_OK );
	}
	else { // success
		WCHAR copiedText[25];
		VERIFY( LoadString( EEGetLocaleInstanceHandle(), IDS_COPIED, copiedText, 25 ) );
		SendMessage( GetDlgItem( hwnd, IDCOPY ), WM_SETTEXT, NULL, (LPARAM)copiedText );
	}
	SendMessage( hwnd, WM_NEXTDLGCTL, (WPARAM)GetDlgItem( hwnd, IDCANCEL ), TRUE );
}

// Message handler for main window.
INT_PTR CALLBACK Window::DialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{	
	switch (msg) {

	case WM_INITDIALOG: {
		SetWindowLongPtr( hwnd, GWLP_USERDATA, lParam );
		Window* window = (Window*)lParam;
		window->textOutput = initDialog( hwnd, window->editor, window->settings );
		if( !window->showWindow ) // closes window immediately for unit testing
			EndDialog( hwnd, 0 );
		break;
	}

	case WM_CLOSE: {
		EndDialog( hwnd, 0 );
		break;
	}

	case WM_COMMAND: {
		switch( wParam ) {

		// Close button
		case IDCANCEL:
			EndDialog( hwnd, 0 );
			break;

		// Copy button
		case IDCOPY:
			Window* window = (Window*)GetWindowLongPtr( hwnd, GWLP_USERDATA );
			copyInfo( hwnd, window->editor, window->textOutput );

		}
		break;
	}
	
	// Settings link
	case WM_NOTIFY: {
		UINT code = ((LPNMHDR)lParam)->code;
		if( code == NM_CLICK || code == NM_RETURN ) {
			Window* window = (Window*)GetWindowLongPtr( hwnd, GWLP_USERDATA );
			settings::Window settingsWindow( window->editor, window->settings, true );
			window->settings = settingsWindow.openSettingsWindow( hwnd );
		}
		break;
	}

	default: {

		Window* window = (Window*)GetWindowLongPtr( hwnd, GWLP_USERDATA );

		// To receive keyboard inputs in dialog
		if( window && GetForegroundWindow() == hwnd && window->keyboardLock.try_lock()  ) {

			// ctrl-c
			if( GetKeyState( VK_CONTROL ) < 0 && GetKeyState( 0x43 ) < 0 )
				copyInfo( hwnd, window->editor, window->textOutput );

			window->keyboardLock.unlock();
		}
	}
	}

	return 0;
}

Window::Window( HWND window, HWND editor, bool showWindow )
	: editor(editor)
	, showWindow(showWindow)
{
	settings = settings::readSettings( editor );

	DialogBoxParam( EEGetLocaleInstanceHandle(), MAKEINTRESOURCE( IDD_DIALOG1 ), window, DialogProc,
					(LPARAM)this );
};

}