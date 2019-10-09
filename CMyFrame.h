#pragma once
#include "resource.h"

class CMyFrame : public CETLFrame<CMyFrame> {
public:
	enum {
		_USE_LOC_DLL = FALSE,
		_IDS_MENU = IDS_PLUGIN_NAME,
		_IDS_STATUS = IDS_PLUGIN_NAME,
		_IDS_NAME = IDS_PLUGIN_NAME,
		_IDS_VER = IDS_VERSION,
		_IDB_BITMAP = IDB_ICONSMALL16C,
		_IDB_16C_24 = IDB_ICONLARGE16C,
		_IDB_TRUE_16_DEFAULT = IDB_ICONSMALL16C,
		_IDB_TRUE_16_HOT = IDB_ICONSMALL16C,
		_IDB_TRUE_16_BW = IDB_ICONSMALLBW,
		_IDB_TRUE_24_DEFAULT = IDB_ICONLARGETRUE,
		_IDB_TRUE_24_HOT = IDB_ICONLARGETRUE,
		_IDB_TRUE_24_BW = IDB_ICONLARGEBW,
		_MASK_TRUE_COLOR = 0,
		_ALLOW_OPEN_SAME_GROUP = 0,
		_ALLOW_MULTIPLE_INSTANCES = 0,
		_MAX_EE_VERSION = 0,
		_MIN_EE_VERSION = 0,
		_SUPPORT_EE_PRO = 0,
		_SUPPORT_EE_STD = 0
	};

	void OnCommand(HWND hwnd);

	void OnEvents(HWND, UINT, LPARAM) {
	}

	BOOL QueryStatus(HWND, LPBOOL pbChecked) {
		*pbChecked = FALSE; return TRUE;
	}

	BOOL DisableAutoComplete(HWND) {
		return FALSE;
	}

	BOOL UseDroppedFiles(HWND) {
		return FALSE;
	}

	BOOL QueryUninstall(HWND) {
		return TRUE;
	};

	BOOL SetUninstall(HWND, LPTSTR, LPTSTR);

	BOOL QueryProperties(HWND) {
		return TRUE;
	}

	BOOL SetProperties(HWND hwnd);

	BOOL PreTranslateMessage(HWND, MSG*) {
		return FALSE;
	}

	LRESULT UserMessage(HWND, WPARAM, LPARAM) {
		return NULL;
	}
};