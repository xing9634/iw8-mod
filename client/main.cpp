#include "common.hpp"
#include "discord/discord_rpc.hpp"
#include "game/game.hpp"
#include "hooks/hook.hpp"
#include "memory/iat.hpp"

#include <utility/nt.hpp>
#include <utility/memory.hpp>

BOOL APIENTRY DllMain(HMODULE hMod, DWORD reason, PVOID) {
	using namespace Client;
	if (reason == DLL_PROCESS_ATTACH) {
		DisableThreadLibraryCalls(hMod);
		g_Module = hMod;

		Common::g_LogService = std::make_unique<Common::LogService>();
		Common::WinAPI::_SetConsoleTitle("iw8-mod: " GIT_DESCRIBE);
		auto game = Common::Utility::NT::Library();
		g_GameModuleName = game.GetName();
		LOG("MainThread", INFO, "IW8-Mod injected. (base: 0x{:016X})", reinterpret_cast<std::size_t>(game.GetPtr()));

		if (remove("Data/data/CASCRepair.mrk") == 0) {
			LOG("MainThread", INFO, "Prevented TACT error E_REPAIR (28) by deleting CASCRepair.mrk.");
		}

		g_Hooks = std::make_unique<Hook::Hooks>();
		LOG("MainThread", INFO, "Hooks initialized.");
	}
	else if (reason == DLL_PROCESS_DETACH) {
		Client::Discord::Destroy();
		g_Hooks.reset();
		g_Running = false;
	}

	return TRUE;
}

extern "C" __declspec(dllexport) int /* EDiscordResult */ /* DISCORD_API */ DiscordCreate(int /* DiscordVersion */ version, struct DiscordCreateParams* params, struct IDiscordCore** result) {
	_Unreferenced_parameter_(version);
	_Unreferenced_parameter_(params);
	_Unreferenced_parameter_(result);

	CreateThread(nullptr, 0, Client::Discord::Thread, nullptr, 0, nullptr);

	LOG("Proxy/DiscordCreate", INFO, "DiscordCreate called, returning 1 (ServiceUnavailable).");
	return 1 /* DiscordResult_ServiceUnavailable */;
}
