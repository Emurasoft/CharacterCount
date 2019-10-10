#include "stdafx.h"
#include "CharacterCount.h"
#include "settings.h"
#include "registry.h"

#include "test.h"
#ifndef _test

#define EE_EXTERN_ONLY
#define ETL_FRAME_CLASS_NAME CMyFrame
#include "template/etlframe.h"
#include "CMyFrame.h"

// Button is pressed on plug-ins toolbar.
void CMyFrame::OnCommand(HWND hwnd) {
	charCount::Window(hwnd, m_hWnd);
}

// Uninstall button.
BOOL CMyFrame::SetUninstall(HWND, LPTSTR, LPTSTR) {
	settings::deleteSettings();
	return UNINSTALL_SIMPLE_DELETE;
}

// Properties shortcut.
BOOL CMyFrame::SetProperties(HWND hwnd) {
	settings::Window settingsWindow(m_hWnd, settings::readSettings(m_hWnd), false);
	settingsWindow.openSettingsWindow(hwnd);
	return TRUE;
}

#endif