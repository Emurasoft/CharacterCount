#pragma once

namespace settings {
	std::array<unsigned char,settings::settingsSize> readSettings( HWND editor );
	bool writeSettings( HWND editor, const std::array<unsigned char,settings::settingsSize>& settings );
	void deleteSettings();
}