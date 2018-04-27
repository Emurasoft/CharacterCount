/*
Copyright (c) 2018 Emurasoft Inc.
Licensed under the MIT license. See LICENSE for details.
*/

#pragma once

namespace settings {
	std::array<unsigned char,settings::settingsSize> readSettings( HWND editor );
	bool writeSettings( HWND editor, const std::array<unsigned char,settings::settingsSize>& settings );
	void deleteSettings();
}