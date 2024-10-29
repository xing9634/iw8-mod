#include "common.hpp"
#include "game/game.hpp"
#include <utility/nt.hpp>

BOOL APIENTRY DllMain(HMODULE hMod, DWORD reason, PVOID) {
	using namespace Client;
	if (reason == DLL_PROCESS_ATTACH) {
		DisableThreadLibraryCalls(hMod);
		g_Module = hMod;
		g_MainThread = CreateThread(nullptr, 0, [](PVOID) -> DWORD {
			Common::g_LogService = std::make_unique<Common::LogService>();
			g_LaunchInfo = Common::GameLaunchType::GetLaunchInfo();
			Common::WinAPI::_SetConsoleTitle("t9-mod: "s + GIT_DESCRIBE + " (" + Common::GameLaunchType::GetDisplayName(g_LaunchInfo.first) + ")");
			Common::Utility::NT::Library().Unprotect();
			LOG("MainThread", INFO, "T9-Mod injected.");

			while (g_Running) {
				// no, we are not planning on unloading the mod, that will cause
				// absolute disaster. instead, we just unload it when the dll is
				// called for detach, therefore safely exiting.
				std::this_thread::sleep_for(1s);
			}

			if (g_Pointers) {
				g_Pointers.reset();
				LOG("MainThread", INFO, "Pointers uninitialized.");
			}

			Common::g_LogService.reset();
			return 0;
		}, nullptr, 0, &g_MainThreadId);
	}
	else if (reason == DLL_PROCESS_DETACH) {
		g_Running = false;
	}

	return TRUE;
}

bool s_CalledMainEntryPoint = false;
void MainEntryPoint() {
	if (s_CalledMainEntryPoint) {
		return;
	}
	s_CalledMainEntryPoint = true;
	using namespace Client;

	g_Pointers = std::make_unique<Game::Pointers>();
	LOG("MainThread", INFO, "Pointers initialized.");
}

extern "C" __declspec(dllexport) int /* EDiscordResult */ /* DISCORD_API */ DiscordCreate(int /* DiscordVersion */ version, struct DiscordCreateParams* params, struct IDiscordCore** result) {
	_Unreferenced_parameter_(version);
	_Unreferenced_parameter_(params);
	_Unreferenced_parameter_(result);

	MainEntryPoint();

	LOG("Proxy/DiscordCreate", INFO, "DiscordCreate called, returning 1 (ServiceUnavailable).");
	return 1 /* DiscordResult_ServiceUnavailable */;
}
