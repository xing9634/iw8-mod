#include "common.hpp"
#include "discord/discord_rpc.hpp"
#include "game/functions.hpp"
#include "game/game.hpp"
#include "game/map_validator.hpp"
#include "hooks/hook.hpp"
#include "hooks/util/hook_util.hpp"
#include "memory/iat.hpp"
#include "network/updater.hpp"

#include <utility/flags.hpp>
#include <utility/nt.hpp>
#include <utility/memory.hpp>
#include <utility/strings.hpp>

BOOL APIENTRY DllMain(HMODULE hMod, DWORD reason, PVOID) {
	using namespace Client;
	if (reason == DLL_PROCESS_ATTACH) {
		DisableThreadLibraryCalls(hMod);
		auto game = Common::Utility::NT::Library();

		g_Module = hMod;
		std::uint32_t gameChecksum = game.GetChecksum();
		if (g_GameVersions.contains(gameChecksum)) {
			g_GameIdentifier = g_GameVersions[gameChecksum];
		}

		Common::g_LogService = std::make_unique<Common::LogService>();
		Common::WinAPI::_SetConsoleTitle(std::format("iw8-mod: " GIT_DESCRIBE " - Call of Duty: Modern Warfare v{}", g_GameIdentifier.m_Version));
		g_GameModuleName = game.GetName();
		LOG("MainThread", INFO, "IW8-Mod injected. (base: 0x{:016X})", reinterpret_cast<std::size_t>(game.GetPtr()));

		if (Common::Utility::Flags::HasFlag("updatecheck")) {
			Network::Updater::CheckForUpdates();
		}

		if (remove("Data/data/CASCRepair.mrk") == 0) {
			LOG("MainThread", INFO, "Prevented TACT error E_REPAIR (28) by deleting CASCRepair.mrk.");
		}

		g_Hooks = std::make_unique<Hook::Hooks>();
		LOG("MainThread", INFO, "Hooks initialized.");

		Common::g_Console.m_OnInput = [](std::string input) {
			if (input == "enable debug info") {
				g_ShowDebugInfo = true;
				LOG("Console/OnInput", INFO, "Enabled debug info.");
			}
			else if (input == "disable debug info") {
				g_ShowDebugInfo = false;
				LOG("Console/OnInput", INFO, "Disabled debug info.");
			}
			else if (Common::Utility::Strings::StartsWith(input, "/")) {
				std::string cbuf = input.substr(1);
				std::vector<std::string> cmdParts = Common::Utility::Strings::Split(cbuf, ' ');
				std::string baseCmd = cmdParts.at(0);

				if (baseCmd == "openmenu") {
					if (cmdParts.size() >= 2) {
						std::string menuName = cmdParts.at(1);
						LOG("Console/OnInput", INFO, "Opening menu: {}", menuName);
						Hook::Util::g_GameThreadQueue.push_back([=]() {
							g_Pointers->m_LUI_OpenMenu(IW8::LocalClientNum_t::LOCAL_CLIENT_0, menuName.c_str(), true, false, false);
						});
					}
					else {
						LOG("Console/OnInput", INFO, "openmenu: A menu name must be supplied.");
					}
				}
				else if (baseCmd == "getdvarstring") {
					if (cmdParts.size() >= 2) {
						std::string dvarName = cmdParts.at(1);
						LOG("Console/OnInput", INFO, "{} = {}", dvarName, g_Pointers->m_Dvar_GetStringSafe(dvarName.c_str()));
					}
					else {
						LOG("Console/OnInput", INFO, "getdvarstring: A dvar name must be supplied.");
					}
				}
				else if (baseCmd == "glcsis") {
					if (cmdParts.size() >= 2) {
						if (cmdParts.at(1) == "true") {
							Hook::Util::g_ForceSignInState = true;
							LOG("Console/OnInput", INFO, "Forcing sign in state.");
						}
						else {
							Hook::Util::g_ForceSignInState = false;
							LOG("Console/OnInput", INFO, "Not forcing sign in state.");
						}
					}
					else {
						LOG("Console/OnInput", INFO, "CL_GetLocalClientSignInState(0) = {}", g_Pointers->m_CL_GetLocalClientSignInState(0));
					}
				}
				else if (baseCmd == "name") {
					if (cmdParts.size() >= 2) {
						std::string newName = cbuf.substr(baseCmd.size() + 1);
						LOG("Console/OnInput", INFO, "Setting name to \"{}\"", newName);
						Hook::Util::g_PlayerName = newName;
					}
					else {
						LOG("Console/OnInput", INFO, "name: A name must be supplied.");
					}
				}
				else if (baseCmd == "namesel") {
					Hook::Util::g_GameThreadQueue.push_back([=]() {
						g_Pointers->m_UI_ShowKeyboard(0, "Name", Hook::Util::g_PlayerName.c_str(), 64, false, false, true, 
							IW8::UI_KEYBOARD_TYPE::UI_KEYBOARD_TYPE_NORMAL, Hook::Util::OnPlayerNameInput, true, true);
					});
				}
				else if (baseCmd == "unlockall") {
					Game::Cbuf_AddText("seta unlockAllItems 1");
					LOG("Console/OnInput", INFO, "All items should now be unlocked.");
				}
				else {
					Game::Cbuf_AddText(cbuf.c_str());
					LOG("Console/OnInput", INFO, "Executed command in Cbuf.");
				}
			}
			else {
				LOG("Console/OnInput", INFO, "Unknown input received: {}", input);
			}
		};
		Common::Console::StartInputThread();
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
	if (Client::g_GameIdentifier.m_Checksum == Client::GameVersion::v1_20_4_7623265_REPLAY) {
		CreateThread(nullptr, 0, Client::Game::MapValidator::Thread, nullptr, 0, nullptr);
	}

	LOG("Proxy/DiscordCreate", INFO, "DiscordCreate called, returning 1 (ServiceUnavailable).");
	return 1 /* DiscordResult_ServiceUnavailable */;
}
