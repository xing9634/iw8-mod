#pragma once
#include "common.hpp"
#include "hooks/hook_types.hpp"
#include "memory/iat.hpp"
#include "memory/minhook.hpp"
#include "game/function_types.hpp"

namespace Client {
	namespace Hook {
		class Hooks {
		public:
			// Main hook - detects unpack state
			using HK_SetUnhandledExceptionFilter = HookPlate::StdcallHook<"kernel32/SetUnhandledExceptionFilter", LPTOP_LEVEL_EXCEPTION_FILTER,
				LPTOP_LEVEL_EXCEPTION_FILTER>;
			Memory::IAT* m_SetUnhandledExceptionFilterHK;

			// Lua - we register them, then they actually hook in Game/luaL_openlib
			HookPlate::LuaHookStore m_LuaHookStore{};
			using HK_LuaShared_LuaCall_IsDemoBuild = HookPlate::LuaHook<"LuaShared_LuaCall_IsDemoBuild", "Engine.BGAAHHAGAC">;
			using HK_LUI_CoD_LuaCall_ActivateInitialClient = HookPlate::LuaHook<"LUI_CoD_LuaCall_ActivateInitialClient", "Engine.CDGCBCBAJ">;
			using HK_LUI_CoD_LuaCall_IsBattleNetAuthReady = HookPlate::LuaHook<"LUI_CoD_LuaCall_IsBattleNetAuthReady", "Engine.JBIHDJBH">;
			using HK_LUI_CoD_LuaCall_IsBattleNetLanOnly = HookPlate::LuaHook<"LUI_CoD_LuaCall_IsBattleNetLanOnly", "Engine.BJGAADIDFH">;
			using HK_LUI_CoD_LuaCall_IsConnectedToGameServer = HookPlate::LuaHook<"LUI_CoD_LuaCall_IsConnectedToGameServer", "Engine.DHEJECBEE">;
			using HK_LUI_CoD_LuaCall_IsGameModeAllowed = HookPlate::LuaHook<"LUI_CoD_LuaCall_IsGameModeAllowed", "Engine.CEGDBDIIIE">;
			using HK_LUI_CoD_LuaCall_IsNetworkConnected = HookPlate::LuaHook<"LUI_CoD_LuaCall_IsNetworkConnected", "Engine.BACCCIHGDG">;
			using HK_LUI_CoD_LuaCall_IsPremiumPlayer = HookPlate::LuaHook<"LUI_CoD_LuaCall_IsPremiumPlayer", "Engine.CFHBIHABCB">;
			using HK_LUI_CoD_LuaCall_IsPremiumPlayerReady = HookPlate::LuaHook<"LUI_CoD_LuaCall_IsPremiumPlayerReady", "Engine.ECFHDAEIDA">;

			// Game
			using HK_DB_LoadXFile = HookPlate::FastcallHook<"DB_LoadXFile", int,
				const char*, uintptr_t, uintptr_t, int, bool, int, uintptr_t>;
			Memory::MinHook<Game::Functions::DB_LoadXFileT>* m_DB_LoadXFileHK;

			using HK_Dvar_RegisterBool = HookPlate::FastcallHook<"Dvar_RegisterBool", IW8::dvar_t*,
				const char*, bool, std::uint32_t, const char*>;
			Memory::MinHook<Game::Functions::Dvar_RegisterBoolT>* m_Dvar_RegisterBoolHK;

			using HK_dwGetLogOnStatus = HookPlate::FastcallHook<"dwGetLogOnStatus", std::int64_t,
				int>;
			Memory::MinHook<Game::Functions::dwGetLogOnStatusT>* m_dwGetLogOnStatusHK;

			using HK_Live_GetLocalClientName = HookPlate::FastcallHook<"Live_GetLocalClientName", const char*>;
			Memory::MinHook<Game::Functions::Live_GetLocalClientNameT>* m_Live_GetLocalClientNameHK;

			using HK_Live_IsUserSignedInToDemonware = HookPlate::FastcallHook<"Live_IsUserSignedInToDemonware", bool,
				int>;
			Memory::MinHook<Game::Functions::Live_IsUserSignedInToDemonwareT>* m_Live_IsUserSignedInToDemonwareHK;

			using HK_luaL_openlib = HookPlate::FastcallHook<"luaL_openlib", void,
				IW8::lua_State*, const char*, const IW8::luaL_Reg*, std::uint32_t>;
			Memory::MinHook<Game::Functions::luaL_openlibT>* m_luaL_openlibHK;

			using HK_PartyHost_StartPrivateParty = HookPlate::FastcallHook<"PartyHost_StartPrivateParty", void,
				int, int, bool, int>;
			Memory::MinHook<Game::Functions::PartyHost_StartPrivatePartyT>* m_PartyHost_StartPrivatePartyHK;

			using HK_R_EndFrame = HookPlate::FastcallHook<"R_EndFrame", void>;
			Memory::MinHook<Game::Functions::R_EndFrameT>* m_R_EndFrameHK;

			using HK_SEH_StringEd_GetString = HookPlate::FastcallHook<"SEH_StringEd_GetString", const char*,
				const char*>;
			Memory::MinHook<Game::Functions::SEH_StringEd_GetStringT>* m_SEH_StringEd_GetStringHK;

			using HK_SV_UpdateUserinfo_f = HookPlate::FastcallHook<"SV_UpdateUserinfo_f", void,
				void*>;
			Memory::MinHook<Game::Functions::SV_UpdateUserinfo_fT>* m_SV_UpdateUserinfo_fHK;

			explicit Hooks();
			void PostUnpack();
			~Hooks();

			template <typename T>
			void DeleteHook(Memory::MinHook<T>** hook, std::vector<int> indexes = {}) {
				if (!hook || !*hook) {
					return;
				}

				if (indexes.empty()) {
					(*hook)->Unhook();
				}
				else {
					for (int index : indexes) {
						(*hook)->Unhook(index);
					}
				}

				delete *hook;
				*hook = nullptr;
			}

			void DeleteHook(Memory::IAT** hook) {
				if (!hook || !*hook) {
					return;
				}

				(*hook)->Unhook();

				delete *hook;
				*hook = nullptr;
			}
		};
	}

	inline std::unique_ptr<Hook::Hooks> g_Hooks{};
}
