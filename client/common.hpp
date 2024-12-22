#pragma once
#include <common_core.hpp>
#include <logger/log_service.hpp>
#include <utility/winapi.hpp>
//#include "resource.h"

#define VT_GET(ptr, idx) (*(void***)(ptr))[idx]
#define HIGH_ORDER_LOG_HOOK 0

#ifdef _SHIP
#	define SELECT(ship, nonShip) ship
#else
#	define SELECT(ship, nonShip) nonShip
#endif

namespace Client {
	using namespace std::chrono_literals;
	using namespace std::string_literals;

	inline HMODULE g_Module{};
	inline HANDLE g_MainThread{};
	inline DWORD g_MainThreadId{};
	inline std::atomic_bool g_Running{ true };
	inline std::string g_GameModuleName = "?";
}
