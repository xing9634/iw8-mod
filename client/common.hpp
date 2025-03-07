#pragma once
#include <common_core.hpp>
#include <logger/log_service.hpp>
#include <utility/winapi.hpp>
//#include "resource.h"

#define VT_GET(ptr, idx) (*(void***)(ptr))[idx]
#define HIGH_ORDER_LOG_HOOK 0

#define ENUM_UNDER(val) static_cast<std::underlying_type<decltype(val)>::type>(val)

namespace Client {
	using namespace std::chrono_literals;
	using namespace std::string_literals;

	enum class GameVersion {
		NONE,
		v0_01_2_7089334,
		v1_03_0_7209368,
		v1_20_4_7623265_REPLAY,
		v1_20_4_7623265_SHIP,
		v1_38_3_9489393,
		v1_44_0_10435696
	};

	struct GameIdentifier {
		std::string m_Version = "Unknown version";
		GameVersion m_Checksum = GameVersion::NONE;
		bool m_Ship = true;
	};

	inline std::unordered_map<std::uint32_t, GameIdentifier> g_GameVersions{
		{ 0x37CCF2A5, { "0.01.2.7089334", GameVersion::v0_01_2_7089334, true } },
		{ 0x641EA3EB, { "1.03.0.7209368", GameVersion::v1_03_0_7209368, true } },
		{ 0xFF380251, { "1.20.4.7623265-replay", GameVersion::v1_20_4_7623265_REPLAY, false } },
		{ 0x6E9C74C8, { "1.20.4.7623265-ship", GameVersion::v1_20_4_7623265_SHIP, true } },
		{ 0xCE698CA4, { "1.38.3.9489393", GameVersion::v1_38_3_9489393, true } },
		{ 0xD6C4C029, { "1.44.0.10435696", GameVersion::v1_44_0_10435696, true } }
	};

	inline HMODULE g_Module{};
	inline HANDLE g_MainThread{};
	inline DWORD g_MainThreadId{};
	inline std::atomic_bool g_Running{ true };
	inline std::string g_GameModuleName = "?";

	inline GameIdentifier g_GameIdentifier{};

	template<typename... Args>
	bool GameVersionIsAny(Args... args) {
		return ((args == g_GameIdentifier.m_Checksum) || ...);
	}

	// runtime settings
	inline bool g_ShowDebugInfo = false;

	// other things
	std::string AndRel(std::uintptr_t address);
}
