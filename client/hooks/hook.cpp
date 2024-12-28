#include "common.hpp"
#include "hooks/hook.hpp"
#include "memory/memory.hpp"
#include "game/game.hpp"

#include <utility/hook.hpp>
#include <utility/memory.hpp>
#include <utility/nt.hpp>

namespace Client::Hook {
	Hooks::Hooks() {
		MH_Initialize();

		this->m_SetUnhandledExceptionFilterHK = new Memory::IAT("kernel32.dll", "SetUnhandledExceptionFilter");
		this->m_SetUnhandledExceptionFilterHK->Hook<HK_SetUnhandledExceptionFilter>();

		this->m_CloseSocketHK = new Memory::IAT("ws2_32.dll", "closesocket");
		this->m_CloseSocketHK->Hook<HK_CloseSocket>();

		this->m_ConnectHK = new Memory::IAT("ws2_32.dll", "connect");
		this->m_ConnectHK->Hook<HK_Connect>();

		this->m_FreeAddrInfoHK = new Memory::IAT("ws2_32.dll", "freeaddrinfo");
		this->m_FreeAddrInfoHK->Hook<HK_FreeAddrInfo>();

		this->m_GetAddrInfoHK = new Memory::IAT("ws2_32.dll", "getaddrinfo");
		this->m_GetAddrInfoHK->Hook<HK_GetAddrInfo>();

		this->m_GetHostByNameHK = new Memory::IAT("ws2_32.dll", "gethostbyname");
		this->m_GetHostByNameHK->Hook<HK_GetHostByName>();

		this->m_GetPeerNameHK = new Memory::IAT("ws2_32.dll", "getpeername");
		this->m_GetPeerNameHK->Hook<HK_GetPeerName>();

		this->m_GetSockNameHK = new Memory::IAT("ws2_32.dll", "getsockname");
		this->m_GetSockNameHK->Hook<HK_GetSockName>();

		this->m_IoctlSocketHK = new Memory::IAT("ws2_32.dll", "ioctlsocket");
		this->m_IoctlSocketHK->Hook<HK_IoctlSocket>();

		this->m_RecvHK = new Memory::IAT("ws2_32.dll", "recv");
		this->m_RecvHK->Hook<HK_Recv>();

		this->m_RecvFromHK = new Memory::IAT("ws2_32.dll", "recvfrom");
		this->m_RecvFromHK->Hook<HK_RecvFrom>();

		this->m_SelectHK = new Memory::IAT("ws2_32.dll", "select");
		this->m_SelectHK->Hook<HK_Select>();

		this->m_SendHK = new Memory::IAT("ws2_32.dll", "send");
		this->m_SendHK->Hook<HK_Send>();

		this->m_SendToHK = new Memory::IAT("ws2_32.dll", "sendto");
		this->m_SendToHK->Hook<HK_SendTo>();
	}

	void MysteryFunctionDetour() {
		LOG("Game/MysteryFunction", INFO, "Function called, ignoring.");
	}

	void Hooks::PostUnpack() {
		auto game = Common::Utility::NT::Library();

		// game_dx12_ship
#		ifdef _SHIP
			Common::Utility::Memory::SafeMemSet(game.GetPtr() + 0x03DF4548, MysteryFunctionDetour);
#		endif

		// game_dx12_ship_replay
#		ifdef _REPLAY
			Common::Utility::Hook::Jump(game.GetPtr() + 0x003061A0, MysteryFunctionDetour);
#		endif

		g_Pointers = std::make_unique<Game::Pointers>();
		LOG("MainThread", INFO, "Pointers initialized.");

		CreateThread(nullptr, 0, [](PVOID _thisPtr) -> DWORD {
			Hooks* _this = (Hooks*)_thisPtr;
#			ifdef _SHIP
				std::this_thread::sleep_for(3s);
#			endif

			_this->m_LuaHookStore.Register<HK_LuaShared_LuaCall_IsDemoBuild>();
			_this->m_LuaHookStore.Register<HK_LUI_CoD_LuaCall_ActivateInitialClient>();
			_this->m_LuaHookStore.Register<HK_LUI_CoD_LuaCall_NotifyServer>();
			_this->m_LuaHookStore.Register<HK_LUI_CoD_LuaCall_ShouldShowDebugInfo>();
#			ifdef _SHIP
				_this->m_LuaHookStore.Register<HK_LUI_CoD_LuaCall_IsBattleNetAuthReady>();
				_this->m_LuaHookStore.Register<HK_LUI_CoD_LuaCall_IsBattleNetLanOnly>();
				_this->m_LuaHookStore.Register<HK_LUI_CoD_LuaCall_IsConnectedToGameServer>();
				_this->m_LuaHookStore.Register<HK_LUI_CoD_LuaCall_IsGameModeAllowed>();
				_this->m_LuaHookStore.Register<HK_LUI_CoD_LuaCall_IsNetworkConnected>();
				_this->m_LuaHookStore.Register<HK_LUI_CoD_LuaCall_IsPremiumPlayer>();
				_this->m_LuaHookStore.Register<HK_LUI_CoD_LuaCall_IsPremiumPlayerReady>();
#			endif

			_this->m_DB_LoadXFileHK = new Memory::MinHook(g_Pointers->m_DB_LoadXFile);
			_this->m_DB_LoadXFileHK->Hook<HK_DB_LoadXFile>();

			_this->m_Dvar_RegisterBoolHK = new Memory::MinHook(g_Pointers->m_Dvar_RegisterBool);
			_this->m_Dvar_RegisterBoolHK->Hook<HK_Dvar_RegisterBool>();

			_this->m_dwGetLogOnStatusHK = new Memory::MinHook(g_Pointers->m_dwGetLogOnStatus);
			_this->m_dwGetLogOnStatusHK->Hook<HK_dwGetLogOnStatus>();

			_this->m_DWServicesAccess__isReadyHK = new Memory::MinHook(g_Pointers->m_DWServicesAccess__isReady);
			_this->m_DWServicesAccess__isReadyHK->Hook<HK_DWServicesAccess__isReady>();

			_this->m_Live_GetLocalClientNameHK = new Memory::MinHook(g_Pointers->m_Live_GetLocalClientName);
			_this->m_Live_GetLocalClientNameHK->Hook<HK_Live_GetLocalClientName>();

			_this->m_Live_IsUserSignedInToDemonwareHK = new Memory::MinHook(g_Pointers->m_Live_IsUserSignedInToDemonware);
			_this->m_Live_IsUserSignedInToDemonwareHK->Hook<HK_Live_IsUserSignedInToDemonware>();

			_this->m_luaL_openlibHK = new Memory::MinHook(g_Pointers->m_luaL_openlib);
			_this->m_luaL_openlibHK->Hook<HK_luaL_openlib>();

			_this->m_PartyHost_StartPrivatePartyHK = new Memory::MinHook(g_Pointers->m_PartyHost_StartPrivateParty);
			_this->m_PartyHost_StartPrivatePartyHK->Hook<HK_PartyHost_StartPrivateParty>();

			_this->m_R_EndFrameHK = new Memory::MinHook(g_Pointers->m_R_EndFrame);
			_this->m_R_EndFrameHK->Hook<HK_R_EndFrame>();

			_this->m_SEH_StringEd_GetStringHK = new Memory::MinHook(g_Pointers->m_SEH_StringEd_GetString);
			_this->m_SEH_StringEd_GetStringHK->Hook<HK_SEH_StringEd_GetString>();

			_this->m_SV_UpdateUserinfo_fHK = new Memory::MinHook(g_Pointers->m_SV_UpdateUserinfo_f);
			_this->m_SV_UpdateUserinfo_fHK->Hook<HK_SV_UpdateUserinfo_f>();

			Common::Utility::Hook::Nop(Memory::SigScan("E8 ? ? ? ? 8B 0D ? ? ? ? 8B 15", g_GameModuleName,
				"Lost Connection Fix #1").Get(), 5);
			Common::Utility::Hook::Nop(Memory::SigScan("E8 ? ? ? ? 80 3D ? ? ? ? ? 75 ? C6 05 ? ? ? ? ? E8", g_GameModuleName,
				"Lost Connection Fix #2").Get(), 5);
			Common::Utility::Hook::Nop(Memory::SigScan("E8 ? ? ? ? 33 D2 48 8D 4D ? 41 B8 ? ? ? ? E8 ? ? ? ? E8", g_GameModuleName,
				"Lost Connection Fix #3").Get(), 5);
			Common::Utility::Hook::Nop(Memory::SigScan("E8 ? ? ? ? 4C 8B 77 ? 48 C7 C6", g_GameModuleName,
				"Lost Connection Fix #4").Get(), 5);

			return 0;
		}, this, 0, nullptr);
	}

	Hooks::~Hooks() {
		this->DeleteHook(&this->m_SV_UpdateUserinfo_fHK);
		this->DeleteHook(&this->m_SEH_StringEd_GetStringHK);
		this->DeleteHook(&this->m_R_EndFrameHK);
		this->DeleteHook(&this->m_PartyHost_StartPrivatePartyHK);
		this->DeleteHook(&this->m_luaL_openlibHK);
		this->DeleteHook(&this->m_Live_IsUserSignedInToDemonwareHK);
		this->DeleteHook(&this->m_Live_GetLocalClientNameHK);
		this->DeleteHook(&this->m_DWServicesAccess__isReadyHK);
		this->DeleteHook(&this->m_dwGetLogOnStatusHK);
		this->DeleteHook(&this->m_Dvar_RegisterBoolHK);
		this->DeleteHook(&this->m_DB_LoadXFileHK);
		this->DeleteHook(&this->m_SetUnhandledExceptionFilterHK);

		g_Pointers.reset();
		LOG("MainThread", INFO, "Pointers uninitialized.");
	}
}
