#include "stdafx.h"

#define EE_EXTERN_ONLY
#define ETL_FRAME_CLASS_NAME CMyFrame
#include "template/etlframe.h"
#include "CMyFrame.h"

#include "CharacterCount.h"
#include "registry.h"
#include "settings.h"

namespace settings {

// Returns position and size for left column text.
RECT settingsLeftColumn(double cy, double i) {
	double left = cy * 1;
	double top = cy * 2.0 + i * cy * 1.0;
	double width = cy * 15;
	double height = cy * 0.7;

	return { (LONG)left,(LONG)top,(LONG)width,(LONG)height };
}

// Returns position and size for right column menu.
RECT settingsRightColumn(double cy, double i) {
	double left = cy * 25 / 2;
	double top = cy * 2.0 + i * cy * 1.0;
	double width = cy * 12 / 2;
	double height = cy * 1.0;

	return { (LONG)left,(LONG)top,(LONG)width,(LONG)height };
}

// Updates all combo boxes.
void updateComboBoxes(std::array<HWND, settingsSize> comboBoxes,
	const std::array<unsigned char, settingsSize>& settings) {
	for (int row = 0; row < settingsSize; ++row) {
		int selectIndex = -1;
		for (UINT i = 0; i < menus[row].size(); ++i) {
			if (menus[row][i] == settings[row]) {
				selectIndex = i;
				break;
			}
		}
		VERIFY(selectIndex != -1);
		if (selectIndex == -1)
			selectIndex = 0;

		SendMessage(comboBoxes[row], CB_SETCURSEL, (WPARAM)selectIndex, NULL);
	}
}

// Creates combo box and returns handle to combox box.
HWND createComboBox(HWND hwnd, int cy, int row, HFONT font, HINSTANCE localeInstanceHandle) {
	RECT rCol = settingsRightColumn(cy, row);
	HWND comboBox = CreateWindow(WC_COMBOBOX, NULL,
		WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | CBS_HASSTRINGS,
		rCol.left, rCol.top, rCol.right, rCol.bottom,
		hwnd, NULL, NULL, NULL);

	SendMessage(comboBox, WM_SETFONT, (WPARAM)font, NULL);

	for (int labelIndex : menus[row]) {
		WCHAR label[25];
		VERIFY(LoadString(localeInstanceHandle, IDS_LABEL300 + labelIndex, label, 25));
		SendMessage(comboBox, CB_ADDSTRING, NULL, (LPARAM)label);
	}

	return comboBox;
}

// Initializes settings window. Returns the handles to created combo boxes.
std::array<HWND, settingsSize> initSettings(HWND hwnd,
	const std::array<unsigned char, settingsSize>& settings) {
	charCount::CenterWindow(hwnd);

	// Add text
	HINSTANCE localeInstanceHandle = EEGetLocaleInstanceHandle();
	WCHAR title[25];
	VERIFY(LoadString(localeInstanceHandle, IDS_PLUGIN_NAME, title, 25));
	SetWindowText(hwnd, title);

	WCHAR okText[25];
	VERIFY(LoadString(localeInstanceHandle, IDS_OK, okText, 25));
	SetWindowText(GetDlgItem(hwnd, IDOK), okText);

	WCHAR resetText[25];
	HWND resetButton = GetDlgItem(hwnd, IDRESET);
	VERIFY(LoadString(localeInstanceHandle, IDS_RESET, resetText, 25));
	SetWindowText(resetButton, resetText);

	HFONT font = (HFONT)SendMessage(hwnd, WM_GETFONT, 0, 0);

	RECT buttonRect;
	GetClientRect(resetButton, &buttonRect);
	int cy = buttonRect.bottom;

	// Resize window
	RECT windowRect;
	GetWindowRect(hwnd, &windowRect);
	SetWindowPos(hwnd, NULL, 0, 0,
		static_cast<int>(cy * 20.0227273),
		windowRect.bottom - windowRect.top,
		SWP_NOMOVE);

	// Heading
	RECT clientRect;
	GetClientRect(hwnd, &clientRect);

	WCHAR heading[50];
	VERIFY(LoadString(localeInstanceHandle, IDS_HEADINGSETTINGS, heading, 50));
	HWND text = CreateWindow(L"STATIC", heading, WS_VISIBLE | WS_CHILD | SS_CENTER, 0, cy / 2,
		clientRect.right - clientRect.left, cy, hwnd, NULL, NULL, NULL);
	SendMessage(text, WM_SETFONT, (WPARAM)font, NULL);

	// Draw text and controls
	std::array<HWND, settingsSize> comboBoxes;

	for (int i = 0; i < settingsSize; ++i) {
		WCHAR label[100];
		VERIFY(LoadString(localeInstanceHandle, IDS_LABEL200 + i, label, 100));
		RECT lCol = settingsLeftColumn(cy, i);
		text = CreateWindow(L"STATIC", label, WS_VISIBLE | WS_CHILD,
			lCol.left, lCol.top, lCol.right, lCol.bottom,
			hwnd, NULL, NULL, NULL);
		SendMessage(text, WM_SETFONT, (WPARAM)font, NULL);

		comboBoxes[i] = createComboBox(hwnd, cy, i, font, localeInstanceHandle);
	}

	updateComboBoxes(comboBoxes, settings);

	return comboBoxes;
}

// Returns the new settings.
std::array<unsigned char, settingsSize>
storeSelSettings(HWND hwnd,
	const std::array<HWND, settingsSize>& comboBoxes,
	HWND editor) {
	std::array<unsigned char, settingsSize> newSettings;

	for (int i = 0; i < settingsSize; ++i) {
		int selectedIndex = (int)SendMessage(comboBoxes[i], CB_GETCURSEL, NULL, NULL);
		newSettings[i] = menus[i][selectedIndex];
	}

	if (!writeSettings(editor, newSettings)) {
		WCHAR message[50];
		VERIFY(LoadString(EEGetLocaleInstanceHandle(), IDS_COULDNTSAVE, message, 50));
		MessageBox(hwnd, message, NULL, MB_ICONWARNING);
	};

	return newSettings;
}

// Returns true if OK pressed on confirmation.
bool confirmReset(HWND hwnd) {
	WCHAR message[50];
	VERIFY(LoadString(EEGetLocaleInstanceHandle(), IDS_CONFIRMRESET, message, 50));
	return MessageBox(hwnd, message, NULL, MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2)
		== IDYES;
}

// Save selected options and close window.
void saveClose(HWND window, const std::array<HWND, settingsSize>& comboBoxes,
	bool reopen, HWND editor) {
	storeSelSettings(window, comboBoxes, editor);
	HWND parent = GetParent(window);
	EndDialog(window, 0);

	if (reopen) {
		EndDialog(parent, 0);
		GetFrame(parent)->OnCommand(parent);
	}
}

// Message handler for settings window.
INT_PTR CALLBACK Window::SettingsProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_INITDIALOG: {
		SetWindowLongPtr(hwnd, GWLP_USERDATA, lParam);

		Window* window = (Window*)lParam;
		window->comboBoxes = initSettings(hwnd, window->settings);

		break;
	}

	case WM_CLOSE: {
		EndDialog(hwnd, 0);
		break;
	}

	case WM_COMMAND: {
		switch (wParam) {
		case IDOK: {
			Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
			saveClose(hwnd, window->comboBoxes, window->reopen, window->editor);
			break;
		}

		case IDCANCEL: {
			EndDialog(hwnd, 0);
			break;
		}

		case IDRESET: {
			Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
			updateComboBoxes(window->comboBoxes, defaultSettings);
			break;
		}
		}
	}
	}

	return 0;
}

std::array<unsigned char, settingsSize> Window::openSettingsWindow(HWND hwnd) {
	DialogBoxParam(EEGetLocaleInstanceHandle(), MAKEINTRESOURCE(IDD_DIALOG2), hwnd,
		SettingsProc, (LPARAM)this);
	return settings;
}

}