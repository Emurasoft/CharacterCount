#include "stdafx.h"
#define EE_EXTERN_ONLY
#define ETL_FRAME_CLASS_NAME CMyFrame
#include "template/etlframe.h"
#include "CMyFrame.h"

#include "settings.h"
#include "registry.h"

namespace settings {
// Reads settings from registry.
std::array<unsigned char, settingsSize> readSettings(HWND editor) {
	DWORD dataSize = settingsSize;
	BYTE readData[settingsSize];
	if (Editor_RegQueryValue(editor, EEREG_EMEDITORPLUGIN, L"CharacterCount", L"settings",
		REG_BINARY, readData, &dataSize, 0)
		!= ERROR_SUCCESS) {
		return defaultSettings;
	};

	if (dataSize != settingsSize)
		return defaultSettings;

	std::array<unsigned char, settingsSize> loadSettings;
	for (unsigned char i = 0; i < settingsSize; ++i) {
		if (std::find(menus[i].begin(), menus[i].end(), readData[i]) != menus[i].end()) {
			loadSettings[i] = readData[i];
		} else {
			return defaultSettings;
		}
	}
	return loadSettings;
}

// Writes settings to registry and returns true if successful.
bool writeSettings(HWND editor, const std::array<unsigned char, settingsSize>& settings) {
	return Editor_RegSetValue(editor, EEREG_EMEDITORPLUGIN, L"CharacterCount", L"settings",
		REG_BINARY, settings.data(), static_cast<DWORD>(settings.size()), EE_REG_VARIABLE_SIZE)
		== ERROR_SUCCESS;
}

// Deletes registry key.
void deleteSettings() {
	HKEY key;
	RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\EmSoft\\EmEditorPlugIns",
		0, KEY_SET_VALUE, &key);
	if (key != NULL) {
		RegDeleteKey(key, L"CharacterCount");
		RegFlushKey(key);
		RegCloseKey(key);
	}
}

}