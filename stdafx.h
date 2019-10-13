#pragma once

#pragma comment(lib, "shlwapi.lib")

#define WIN32_LEAN_AND_MEAN
#define _ALLOW_RTCc_IN_STL

#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <commctrl.h>
#include <string>
#include <algorithm>
#include <shlwapi.h>
#include <vector>
#include <array>
#include <mutex>
#include <tuple>
#include <functional>
#include <assert.h>

#ifdef _DEBUG
#define VERIFY(f) _ASSERTE(f)
#else
#define VERIFY(f) ((void)(f))
#endif