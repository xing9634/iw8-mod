#include "common.hpp"
#include "game/game.hpp"
#include "memory/memory.hpp"
#include <utility/memory.hpp>
#include <utility/nt.hpp>

#define SETUP_POINTER(name) #name, (void**)&this->m_##name
#define SETUP_MOD(mod) [](Memory::ScannedResult<void> r) { return r.##mod##; }

namespace Client::Game {
	void Pointers::PointerList::Apply() {
		for (PointerCalculator ptr : this->m_Pointers) {
			if (!ptr.TargetsVersion(g_GameIdentifier.m_Checksum)) {
				continue;
			}
			Memory::ScannedResult<void> res = Memory::SigScan(ptr.m_Calculator.m_Signature, this->m_Module, ptr.m_Name);
			if (res) {
				ptr.m_Calculator.m_Mod(res).Apply(ptr.m_Pointer);
			}
		}
	}

	Pointers::PointerList Pointers::GetPointerList() {
		return {
			g_GameModuleName, {
				// AddBaseDrawTextCmd
				{
					{ GameVersion::v1_38_3_9489393, GameVersion::v1_44_0_10435696 },
					{ "E8 ? ? ? ? 48 8B D0 48 85 C0 0F 84 ? ? ? ? 48 8B 8C 24", SETUP_MOD(Add(1).Rip()) },
					SETUP_POINTER(AddBaseDrawTextCmd)
				},
				// CG_WorldPosToScreenPosReal
				{
					{ GameVersion::v1_20_4_7623265_REPLAY, GameVersion::v1_20_4_7623265_SHIP, GameVersion::v1_38_3_9489393, GameVersion::v1_44_0_10435696 },
					{ "E8 ? ? ? ? 4C 8D 4D ? 49 8B D6 4C 8D 45 E7", SETUP_MOD(Add(1).Rip()) },
					SETUP_POINTER(CG_WorldPosToScreenPosReal)
				},
				// CL_PlayerData_GetDDLBuffer
				{
					{ GameVersion::v1_20_4_7623265_REPLAY, GameVersion::v1_20_4_7623265_SHIP, GameVersion::v1_38_3_9489393, GameVersion::v1_44_0_10435696 },
					{ "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B E9 49 63 F8" },
					SETUP_POINTER(CL_PlayerData_GetDDLBuffer)
				},
				// Com_GameInfo_GetGameTypeForInternalName
				{
					{ GameVersion::v1_20_4_7623265_REPLAY, GameVersion::v1_20_4_7623265_SHIP, GameVersion::v1_38_3_9489393, GameVersion::v1_44_0_10435696 },
					{ "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 41 56 8B 35 ? ? ? ? 45 33 DB 48 8B E9" },
					SETUP_POINTER(Com_GameInfo_GetGameTypeForInternalName)
				},
				// Com_GameInfo_GetMapInfoForLoadName
				{
					{ GameVersion::v1_20_4_7623265_REPLAY, GameVersion::v1_20_4_7623265_SHIP, GameVersion::v1_38_3_9489393, GameVersion::v1_44_0_10435696 },
					{ "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 41 56 8B 3D ? ? ? ? 45 33 DB 48 8B E9" },
					SETUP_POINTER(Com_GameInfo_GetMapInfoForLoadName)
				},
				// Com_ParseNavStrings
				{
					{ GameVersion::v1_20_4_7623265_REPLAY, GameVersion::v1_20_4_7623265_SHIP, GameVersion::v1_38_3_9489393, GameVersion::v1_44_0_10435696 },
					{ "E8 ? ? ? ? 44 8B 44 24 ? 33 F6 45 85 C0 7E", SETUP_MOD(Add(1).Rip()) },
					SETUP_POINTER(Com_ParseNavStrings)
				},
				// Com_SetErrorMessage
				{
					{ GameVersion::v1_20_4_7623265_REPLAY, GameVersion::v1_20_4_7623265_SHIP, GameVersion::v1_38_3_9489393, GameVersion::v1_44_0_10435696 },
					{ "E8 ? ? ? ? EB ? 40 38 2D", SETUP_MOD(Add(1).Rip()) },
					SETUP_POINTER(Com_SetErrorMessage)
				},
				// Content_DoWeHaveContentPack
				{
					{ GameVersion::v1_38_3_9489393, GameVersion::v1_44_0_10435696},
					{ "8B D1 83 F9 ? 75" },
					SETUP_POINTER(Content_DoWeHaveContentPack)
				},
				// DB_LoadXFile
				{
					{ GameVersion::v1_20_4_7623265_REPLAY, GameVersion::v1_20_4_7623265_SHIP },
					{ "E8 ? ? ? ? 8B F8 44 38 AB", SETUP_MOD(Add(1).Rip()) },
					SETUP_POINTER(DB_LoadXFile)
				},
				{
					{ GameVersion::v1_38_3_9489393, GameVersion::v1_44_0_10435696 },
					{ "E8 ? ? ? ? 8B F8 33 ED 40 38 B3", SETUP_MOD(Add(1).Rip()) },
					SETUP_POINTER(DB_LoadXFile)
				},
				// DB_Zones_PerformZoneLoad
				{
					{ GameVersion::v1_20_4_7623265_REPLAY, GameVersion::v1_20_4_7623265_SHIP },
					{ "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 54 41 55 41 56 41 57 48 83 EC ? 45 8B F9 41 0F B6 F0" },
					SETUP_POINTER(DB_Zones_PerformZoneLoad)
				},
				{
					{ GameVersion::v1_38_3_9489393, GameVersion::v1_44_0_10435696 },
					{ "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 54 41 55 41 56 41 57 48 83 EC ? 45 8B E1 41 0F B6 E8" },
					SETUP_POINTER(DB_Zones_PerformZoneLoad)
				},
				// DDL_GetEnum
				{
					{ GameVersion::v1_20_4_7623265_REPLAY, GameVersion::v1_20_4_7623265_SHIP, GameVersion::v1_38_3_9489393, GameVersion::v1_44_0_10435696 },
					{ "48 89 5C 24 ? 57 48 83 EC ? 48 8B FA 48 8B D9 E8 ? ? ? ? 84 C0 74 ? 4C 8B 4B ? 48 8B CF 41 8B 41 ? 99 41 F7 79 ? 45 8B 49 ? 8B 53 ? 44 8B C0 E8 ? ? ? ? EB ? 33 C0 8B D0 48 8B CB 48 8B 5C 24" },
					SETUP_POINTER(DDL_GetEnum)
				},
				// DDL_GetInt
				{
					{ GameVersion::v1_20_4_7623265_REPLAY, GameVersion::v1_20_4_7623265_SHIP, GameVersion::v1_38_3_9489393, GameVersion::v1_44_0_10435696 },
					{ "48 89 5C 24 ? 57 48 83 EC ? 48 8B FA 48 8B D9 E8 ? ? ? ? 84 C0 74 ? 4C 8B 4B ? 48 8B CF 41 8B 41 ? 99 41 F7 79 ? 45 8B 49 ? 8B 53 ? 44 8B C0 E8 ? ? ? ? EB ? 33 C0 48 8B 4B" },
					SETUP_POINTER(DDL_GetInt)
				},
				// DDL_GetRootState
				{
					{ GameVersion::v1_20_4_7623265_REPLAY, GameVersion::v1_20_4_7623265_SHIP, GameVersion::v1_38_3_9489393, GameVersion::v1_44_0_10435696 },
					{ "33 C0 C7 41 ? ? ? ? ? 48 89 41 ? 89 41 ? 48 8B C1 48 89 51" },
					SETUP_POINTER(DDL_GetRootState)
				},
				// DDL_GetString
				{
					{ GameVersion::v1_20_4_7623265_REPLAY, GameVersion::v1_20_4_7623265_SHIP, GameVersion::v1_38_3_9489393, GameVersion::v1_44_0_10435696 },
					{ "40 55 56 48 83 EC ? 48 8B EA" },
					SETUP_POINTER(DDL_GetString)
				},
				// DDL_MoveToPath
				{
					{ GameVersion::v1_20_4_7623265_REPLAY, GameVersion::v1_20_4_7623265_SHIP, GameVersion::v1_38_3_9489393, GameVersion::v1_44_0_10435696 },
					{ "48 89 74 24 ? 48 89 7C 24 ? 41 56 48 83 EC ? 4D 8B F1 48 8B FA 48 85 C9 74 ? 48 85 D2 74 ? 48 8B 41 ? 48 89 42 ? 48 8B 41 ? 48 89 42 ? 8B 41 ? 89 42 ? 8B 41 ? 89 42 ? 0F B6 01 88 02 48 89 5C 24 ? 49 63 F0 45 85 C0 7E ? 33 DB 48 8B CF E8 ? ? ? ? 4D 8B 04 DE" },
					SETUP_POINTER(DDL_MoveToPath)
				},
				// DDL_SetEnum
				{
					{ GameVersion::v1_20_4_7623265_REPLAY, GameVersion::v1_20_4_7623265_SHIP, GameVersion::v1_38_3_9489393, GameVersion::v1_44_0_10435696 },
					{ "48 89 5C 24 ? 57 48 83 EC ? 48 8B FA 48 8B D9 33 D2 49 8B C8" },
					SETUP_POINTER(DDL_SetEnum)
				},
				// DDL_SetInt
				{
					{ GameVersion::v1_20_4_7623265_REPLAY, GameVersion::v1_20_4_7623265_SHIP, GameVersion::v1_38_3_9489393, GameVersion::v1_44_0_10435696 },
					{ "E9 ? ? ? ? 85 C9 75 ? 8D 42", SETUP_MOD(Add(1).Rip()) },
					SETUP_POINTER(DDL_SetInt)
				},
				// DDL_SetString
				{
					{ GameVersion::v1_20_4_7623265_REPLAY, GameVersion::v1_20_4_7623265_SHIP, GameVersion::v1_38_3_9489393, GameVersion::v1_44_0_10435696 },
					{ "48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 49 8B D8 48 8B F2 48 8B F9 E8" },
					SETUP_POINTER(DDL_SetString)
				},
				// Dvar_FindVarByName
				{
					{ GameVersion::v1_20_4_7623265_REPLAY, GameVersion::v1_20_4_7623265_SHIP, GameVersion::v1_38_3_9489393, GameVersion::v1_44_0_10435696 },
					{ "E8 ? ? ? ? 48 8B CB 48 63 50", SETUP_MOD(Add(1).Rip()) },
					SETUP_POINTER(Dvar_FindVarByName)
				},
				// Dvar_GetIntSafe
				{
					{ GameVersion::v1_20_4_7623265_REPLAY, GameVersion::v1_20_4_7623265_SHIP, GameVersion::v1_38_3_9489393, GameVersion::v1_44_0_10435696 },
					{ "E8 ? ? ? ? 8B D0 85 C0 75 ? 38 05", SETUP_MOD(Add(1).Rip()) },
					SETUP_POINTER(Dvar_GetIntSafe)
				},
				// Dvar_GetStringSafe
				{
					{ GameVersion::v1_20_4_7623265_REPLAY, GameVersion::v1_20_4_7623265_SHIP, GameVersion::v1_38_3_9489393, GameVersion::v1_44_0_10435696 },
					{ "48 83 EC ? E8 ? ? ? ? 8B C8 E8 ? ? ? ? 48 85 C0 75 ? 48 8D 05 ? ? ? ? 48 83 C4 ? C3 80 78" },
					SETUP_POINTER(Dvar_GetStringSafe)
				},
				// Dvar_RegisterBool
				{
					{ GameVersion::v1_20_4_7623265_REPLAY, GameVersion::v1_20_4_7623265_SHIP, GameVersion::v1_38_3_9489393, GameVersion::v1_44_0_10435696 },
					{ "E8 ? ? ? ? 48 8B F0 F6 46", SETUP_MOD(Add(1).Rip()) },
					SETUP_POINTER(Dvar_RegisterBool)
				},
				// dwGetLogOnStatus
				{
					{ GameVersion::v1_20_4_7623265_REPLAY, GameVersion::v1_20_4_7623265_SHIP, GameVersion::v1_38_3_9489393 },
					{ "40 53 48 83 EC ? 48 63 C1 BA" },
					SETUP_POINTER(dwGetLogOnStatus)
				},
				{
					{ GameVersion::v1_44_0_10435696 },
					{ "40 53 48 83 EC ? 48 63 C1 BA ? ? ? ? 48 69 D8" },
					SETUP_POINTER(dwGetLogOnStatus)
				},
				// DWServicesAccess__isReady
				{
					{ GameVersion::v1_20_4_7623265_REPLAY, GameVersion::v1_20_4_7623265_SHIP, GameVersion::v1_38_3_9489393, GameVersion::v1_44_0_10435696 },
					{ "48 83 EC ? 85 D2 78 ? 48 63 C2 48 3B 81 ? ? ? ? 73 ? 48 69 C0 ? ? ? ? 48 81 C1 ? ? ? ? 48 89 5C 24 ? 48 03 C8 32 DB 48 8B 01" },
					SETUP_POINTER(DWServicesAccess__isReady)
				},
				// FS_ReadFile
				{
					{ GameVersion::v1_20_4_7623265_REPLAY, GameVersion::v1_20_4_7623265_SHIP, GameVersion::v1_38_3_9489393, GameVersion::v1_44_0_10435696 },
					{ "40 53 57 41 57 48 83 EC ? 48 8B FA" },
					SETUP_POINTER(FS_ReadFile)
				},
				// GamerProfile_SetDataByName
				{
					{ GameVersion::v1_20_4_7623265_REPLAY, GameVersion::v1_20_4_7623265_SHIP, GameVersion::v1_38_3_9489393, GameVersion::v1_44_0_10435696 },
					{ "48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 8B F1 0F 29 74 24" },
					SETUP_POINTER(GamerProfile_SetDataByName)
				},
				// I_irand
				{
					{ GameVersion::v1_20_4_7623265_REPLAY, GameVersion::v1_20_4_7623265_SHIP, GameVersion::v1_38_3_9489393, GameVersion::v1_44_0_10435696 },
					{ "69 05 ? ? ? ? ? ? ? ? 2B D1 48 63 D2" },
					SETUP_POINTER(I_irand)
				},
				// Info_ValueForKey
				{
					{ GameVersion::v1_20_4_7623265_REPLAY, GameVersion::v1_20_4_7623265_SHIP, GameVersion::v1_38_3_9489393, GameVersion::v1_44_0_10435696 },
					{ "48 89 74 24 ? 57 48 83 EC ? 48 8B FA 48 8B F1 48 85 C9 74" },
					SETUP_POINTER(Info_ValueForKey)
				},
				// j_va
				{
					{ GameVersion::v1_20_4_7623265_REPLAY, GameVersion::v1_20_4_7623265_SHIP, GameVersion::v1_38_3_9489393, GameVersion::v1_44_0_10435696 },
					{ "48 89 4C 24 ? 48 89 54 24 ? 4C 89 44 24 ? 4C 89 4C 24 ? 53 48 83 EC ? B9" },
					SETUP_POINTER(j_va)
				},
				// Live_GetLocalClientName
				{
					{ GameVersion::v1_20_4_7623265_REPLAY, GameVersion::v1_20_4_7623265_SHIP, GameVersion::v1_38_3_9489393, GameVersion::v1_44_0_10435696 },
					{ "E8 ? ? ? ? 4C 8B 4B ? 48 8D 0D", SETUP_MOD(Add(1).Rip()) },
					SETUP_POINTER(Live_GetLocalClientName)
				},
				// Live_IsUserSignedInToDemonware
				{
					{ GameVersion::v1_20_4_7623265_REPLAY, GameVersion::v1_20_4_7623265_SHIP, GameVersion::v1_38_3_9489393, GameVersion::v1_44_0_10435696 },
					{ "E8 ? ? ? ? 84 C0 74 ? 4C 8D 43 ? 8B D7", SETUP_MOD(Add(1).Rip()) },
					SETUP_POINTER(Live_IsUserSignedInToDemonware)
				},
				// lua_getfield
				{
					{ GameVersion::v1_20_4_7623265_REPLAY, GameVersion::v1_20_4_7623265_SHIP, GameVersion::v1_38_3_9489393, GameVersion::v1_44_0_10435696 },
					{ "48 89 5C 24 ? 57 48 83 EC ? 4D 8B D0 48 8B D9 E8 ? ? ? ? 48 8B F8 49 C7 C0 ? ? ? ? 90 49 FF C0 43 80 3C 02 ? 75 ? 49 8B D2 48 8B CB E8 ? ? ? ? 48 B9 ? ? ? ? ? ? ? ? 4C 8D 44 24 ? 48 0B C1 48 8B D7 48 8B CB 48 89 44 24 ? E8 ? ? ? ? 48 85 C0" },
					SETUP_POINTER(lua_getfield)
				},
				// lua_pushboolean
				{
					{ GameVersion::v1_20_4_7623265_REPLAY, GameVersion::v1_20_4_7623265_SHIP, GameVersion::v1_38_3_9489393, GameVersion::v1_44_0_10435696 },
					{ "E8 ? ? ? ? EB ? 85 D2 78", SETUP_MOD(Add(1).Rip()) },
					SETUP_POINTER(lua_pushboolean)
				},
				// lua_pushstring
				{
					{ GameVersion::v1_20_4_7623265_REPLAY, GameVersion::v1_20_4_7623265_SHIP, GameVersion::v1_38_3_9489393, GameVersion::v1_44_0_10435696 },
					{ "48 89 5C 24 ? 57 48 83 EC ? 48 8B FA 48 8B D9 48 85 D2 75 ? 48 8B 41" },
					SETUP_POINTER(lua_pushstring)
				},
				// lua_remove
				{
					{ GameVersion::v1_20_4_7623265_REPLAY, GameVersion::v1_20_4_7623265_SHIP, GameVersion::v1_38_3_9489393, GameVersion::v1_44_0_10435696 },
					{ "4C 8B C1 85 D2 7E ? 8D 42 ? 48 63 D0 48 8B 41 ? 48 8B 49" },
					SETUP_POINTER(lua_remove)
				},
				// lua_toboolean
				{
					{ GameVersion::v1_20_4_7623265_REPLAY, GameVersion::v1_20_4_7623265_SHIP, GameVersion::v1_38_3_9489393, GameVersion::v1_44_0_10435696 },
					{ "48 83 EC ? E8 ? ? ? ? 48 8B 08 33 C0" },
					SETUP_POINTER(lua_toboolean)
				},
				// lua_tointeger32
				{
					{ GameVersion::v1_20_4_7623265_REPLAY, GameVersion::v1_20_4_7623265_SHIP, GameVersion::v1_38_3_9489393, GameVersion::v1_44_0_10435696 },
					{ "E8 ? ? ? ? 8B F0 80 FB", SETUP_MOD(Add(1).Rip()) },
					SETUP_POINTER(lua_tointeger32)
				},
				// lua_tolstring
				{
					{ GameVersion::v1_20_4_7623265_REPLAY, GameVersion::v1_20_4_7623265_SHIP, GameVersion::v1_38_3_9489393, GameVersion::v1_44_0_10435696 },
					{ "48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 49 8B F8 8B DA 48 8B F1" },
					SETUP_POINTER(lua_tolstring)
				},
				// luaL_openlib
				{
					{ GameVersion::v1_20_4_7623265_REPLAY, GameVersion::v1_20_4_7623265_SHIP, GameVersion::v1_38_3_9489393, GameVersion::v1_44_0_10435696 },
					{ "48 89 5C 24 ? 55 56 41 56 48 83 EC ? 48 8B 41" },
					SETUP_POINTER(luaL_openlib)
				},
				// LuaShared_PCall
				{
					{ GameVersion::v1_20_4_7623265_REPLAY, GameVersion::v1_20_4_7623265_SHIP },
					{ "E8 ? ? ? ? 8B E8 F7 D6", SETUP_MOD(Add(1).Rip()) },
					SETUP_POINTER(LuaShared_PCall)
				},
				{
					{ GameVersion::v1_38_3_9489393, GameVersion::v1_44_0_10435696 },
					{ "E8 ? ? ? ? 8B F8 85 C0 74 ? 4C 8D 44 24", SETUP_MOD(Add(1).Rip()) },
					SETUP_POINTER(LuaShared_PCall)
				},
				// LUI_BeginTable
				{
					{ GameVersion::v1_20_4_7623265_REPLAY, GameVersion::v1_20_4_7623265_SHIP, GameVersion::v1_38_3_9489393, GameVersion::v1_44_0_10435696 },
					{ "E8 ? ? ? ? 85 ED 74 ? BB", SETUP_MOD(Add(1).Rip()) },
					SETUP_POINTER(LUI_BeginTable)
				},
				// LUI_EndTable
				{
					{ GameVersion::v1_20_4_7623265_REPLAY, GameVersion::v1_20_4_7623265_SHIP },
					{ "E8 ? ? ? ? 44 3B 76", SETUP_MOD(Add(1).Rip()) },
					SETUP_POINTER(LUI_EndTable)
				},
				{
					{ GameVersion::v1_38_3_9489393, GameVersion::v1_44_0_10435696 },
					{ "E8 ? ? ? ? 8B DF 3B BE", SETUP_MOD(Add(1).Rip()) },
					SETUP_POINTER(LUI_EndTable)
				},
				// LUI_OpenMenu
				{
					{ GameVersion::v1_20_4_7623265_REPLAY, GameVersion::v1_20_4_7623265_SHIP, GameVersion::v1_38_3_9489393, GameVersion::v1_44_0_10435696 },
					{ "48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 41 8B F1 41 8B D8" },
					SETUP_POINTER(LUI_OpenMenu)
				},
				// PartyHost_StartPrivateParty
				{
					{ GameVersion::v1_20_4_7623265_REPLAY, GameVersion::v1_20_4_7623265_SHIP, GameVersion::v1_38_3_9489393, GameVersion::v1_44_0_10435696 },
					{ "E8 ? ? ? ? 48 8D 0D ? ? ? ? E8 ? ? ? ? 33 C9 85 C0", SETUP_MOD(Add(1).Rip()) },
					SETUP_POINTER(PartyHost_StartPrivateParty)
				},
				// ProcessScriptFile
				{
					{ GameVersion::v1_20_4_7623265_REPLAY, GameVersion::v1_20_4_7623265_SHIP, GameVersion::v1_38_3_9489393, GameVersion::v1_44_0_10435696 },
					{ "48 89 5C 24 ? 48 89 74 24 ? 55 57 41 54 41 56 41 57 48 8B EC 48 83 EC ? 8B 5A" },
					SETUP_POINTER(ProcessScriptFile)
				},
				// R_AddCmdDrawText
				{
					{ GameVersion::v1_20_4_7623265_REPLAY, GameVersion::v1_20_4_7623265_SHIP },
					{ "E8 ? ? ? ? 48 8B 05 ? ? ? ? 0F 28 DF", SETUP_MOD(Add(1).Rip()) },
					SETUP_POINTER(R_AddCmdDrawText)
				},
				// R_EndFrame
				{
					{ GameVersion::v1_20_4_7623265_REPLAY, GameVersion::v1_20_4_7623265_SHIP },
					{ "48 8B 15 ? ? ? ? 45 33 D2 4C 8B 0D" },
					SETUP_POINTER(R_EndFrame)
				},
				{
					{ GameVersion::v1_38_3_9489393, GameVersion::v1_44_0_10435696 },
					{ "48 83 EC ? E8 ? ? ? ? 48 8B 15 ? ? ? ? 45 33 D2" },
					SETUP_POINTER(R_EndFrame)
				},
				// SEH_StringEd_GetString
				{
					{ GameVersion::v1_20_4_7623265_REPLAY, GameVersion::v1_20_4_7623265_SHIP },
					{ "E8 ? ? ? ? 48 8B CB 48 8B F8 E8 ? ? ? ? 48 8B D0 48 8B CF E8 ? ? ? ? 4C 8B 05 ? ? ? ? 48 8D 0D ? ? ? ? 48 8B D0 E8 ? ? ? ? 48 8B 0D", SETUP_MOD(Add(1).Rip()) },
					SETUP_POINTER(SEH_StringEd_GetString)
				},
				{
					{ GameVersion::v1_38_3_9489393, GameVersion::v1_44_0_10435696 },
					{ "E8 ? ? ? ? 48 8B D0 80 FB", SETUP_MOD(Add(1).Rip()) },
					SETUP_POINTER(SEH_StringEd_GetString)
				},
				// SV_Cmd_Argv
				{
					{ GameVersion::v1_20_4_7623265_REPLAY, GameVersion::v1_20_4_7623265_SHIP },
					{ "E8 ? ? ? ? 4C 8B D0 4C 2B D3", SETUP_MOD(Add(1).Rip()) },
					SETUP_POINTER(SV_Cmd_Argv)
				},
				{
					{ GameVersion::v1_38_3_9489393, GameVersion::v1_44_0_10435696 },
					{ "E8 ? ? ? ? 48 89 44 24 ? 33 F6 0F", SETUP_MOD(Add(1).Rip()) },
					SETUP_POINTER(SV_Cmd_Argv)
				},
				// SV_UpdateUserinfo_f
				{
					{ GameVersion::v1_20_4_7623265_REPLAY, GameVersion::v1_20_4_7623265_SHIP, GameVersion::v1_38_3_9489393, GameVersion::v1_44_0_10435696 },
					{ "40 53 48 83 EC ? 48 8B D9 B9 ? ? ? ? E8 ? ? ? ? 48 8D 8B" },
					SETUP_POINTER(SV_UpdateUserinfo_f)
				},
				// Sys_Microseconds
				{
					{ GameVersion::v1_20_4_7623265_REPLAY, GameVersion::v1_20_4_7623265_SHIP, GameVersion::v1_38_3_9489393, GameVersion::v1_44_0_10435696 },
					{ "83 3D ? ? ? ? ? 49 B8 ? ? ? ? ? ? ? ? F2 0F 10 15" },
					SETUP_POINTER(Sys_Microseconds)
				},
				// clientUIActives
				{
					{ GameVersion::v1_20_4_7623265_REPLAY, GameVersion::v1_20_4_7623265_SHIP, GameVersion::v1_38_3_9489393, GameVersion::v1_44_0_10435696 },
					{ "48 8D 15 ? ? ? ? 8B 4C 10", SETUP_MOD(Add(3).Rip()) },
					SETUP_POINTER(clientUIActives)
				},
				// cmd_args
				{
					{ GameVersion::v1_20_4_7623265_REPLAY, GameVersion::v1_20_4_7623265_SHIP, GameVersion::v1_38_3_9489393, GameVersion::v1_44_0_10435696 },
					{ "48 63 05 ? ? ? ? 33 C9 41 89 AC 87", SETUP_MOD(Add(3).Rip()) },
					SETUP_POINTER(cmd_args)
				},
				// g_entities
				{
					{ GameVersion::v1_20_4_7623265_REPLAY, GameVersion::v1_20_4_7623265_SHIP, GameVersion::v1_38_3_9489393, GameVersion::v1_44_0_10435696 },
					{ "48 8B 0D ? ? ? ? 8B F2 4C 69 E0", SETUP_MOD(Add(3).Rip()) },
					SETUP_POINTER(g_entities)
				},
				// holdrand
				{
					{ GameVersion::v1_20_4_7623265_REPLAY, GameVersion::v1_20_4_7623265_SHIP, GameVersion::v1_38_3_9489393, GameVersion::v1_44_0_10435696 },
					{ "89 05 ? ? ? ? C1 E8 ? F3 48 0F 2A C0", SETUP_MOD(Add(2).Rip()) },
					SETUP_POINTER(holdrand)
				},
				// LUI_luaVM
				{
					{ GameVersion::v1_20_4_7623265_REPLAY, GameVersion::v1_20_4_7623265_SHIP },
					{ "4C 8B 05 ? ? ? ? 48 8D 0D ? ? ? ? 48 8B D0 E8 ? ? ? ? BA ? ? ? ? 48 8B CE E8 ? ? ? ? 4D 8D 7F", SETUP_MOD(Add(3).Rip()) },
					SETUP_POINTER(LUI_luaVM)
				},
				{
					{ GameVersion::v1_38_3_9489393, GameVersion::v1_44_0_10435696 },
					{ "48 8B 05 ? ? ? ? 45 33 C0 44 8B 4C 24 ? 48 89 44 24 ? 48 8B 5C 24", SETUP_MOD(Add(3).Rip()) },
					SETUP_POINTER(LUI_luaVM)
				},
				// s_luaInFrontend
				{
					{ GameVersion::v1_20_4_7623265_REPLAY, GameVersion::v1_20_4_7623265_SHIP, GameVersion::v1_38_3_9489393, GameVersion::v1_44_0_10435696 },
					{ "0F B6 05 ? ? ? ? 75", SETUP_MOD(Add(3).Rip()) },
					SETUP_POINTER(s_luaInFrontend)
				},
				// s_presenceData
				{
					{ GameVersion::v1_20_4_7623265_REPLAY, GameVersion::v1_20_4_7623265_SHIP, GameVersion::v1_38_3_9489393, GameVersion::v1_44_0_10435696 },
					{ "48 8D 05 ? ? ? ? 4C 8D 05 ? ? ? ? 48 39 08 74 ? FF C2 48 05", SETUP_MOD(Add(3).Rip()) },
					SETUP_POINTER(s_presenceData)
				},
				// sharedUiInfo_assets
				{
					{ GameVersion::v1_20_4_7623265_REPLAY, GameVersion::v1_20_4_7623265_SHIP },
					{ "48 89 05 ? ? ? ? 48 8D 0D ? ? ? ? E8 ? ? ? ? B2 ? 48 89 05 ? ? ? ? 48 8D 0D ? ? ? ? E8 ? ? ? ? 33 D2", SETUP_MOD(Add(3).Rip()) },
					SETUP_POINTER(sharedUiInfo_assets)
				},
				{
					{ GameVersion::v1_38_3_9489393, GameVersion::v1_44_0_10435696 },
					{ "48 89 05 ? ? ? ? 48 8D 0D ? ? ? ? E8 ? ? ? ? B2 ? 48 89 05 ? ? ? ? 48 8D 0D ? ? ? ? E8 ? ? ? ? 80 3D", SETUP_MOD(Add(3).Rip()) },
					SETUP_POINTER(sharedUiInfo_assets)
				},
				// Unk_AuthCheck1
				{
					{ GameVersion::v1_20_4_7623265_REPLAY, GameVersion::v1_20_4_7623265_SHIP, GameVersion::v1_38_3_9489393, GameVersion::v1_44_0_10435696 },
					{ "80 3D ? ? ? ? ? 74 ? 48 89 7C 24", SETUP_MOD(Add(2).Rip().Add(1)) },
					SETUP_POINTER(Unk_AuthCheck1)
				},
				// Unk_AuthCheck2
				{
					{ GameVersion::v1_20_4_7623265_REPLAY },
					{ "48 8D 2D ? ? ? ? 84 C0 74 ? 48 69 C7", SETUP_MOD(Add(3).Rip()) },
					SETUP_POINTER(Unk_AuthCheck2)
				},
				{
					{ GameVersion::v1_20_4_7623265_SHIP },
					{ "83 3D ? ? ? ? ? 7E ? 33 C9", SETUP_MOD(Add(2).Rip()) },
					SETUP_POINTER(Unk_AuthCheck2)
				},
				// Unk_XUIDCheck1
				{
					{ GameVersion::v1_20_4_7623265_REPLAY, GameVersion::v1_20_4_7623265_SHIP },
					{ "48 8D 3D ? ? ? ? 0F 1F 44 00 ? 48 63 C3", SETUP_MOD(Add(3).Rip()) },
					SETUP_POINTER(Unk_XUIDCheck1)
				},
				{
					{ GameVersion::v1_44_0_10435696 },
					{ "48 8D 1D ? ? ? ? 40 88 35", SETUP_MOD(Add(3).Rip()) },
					SETUP_POINTER(Unk_XUIDCheck1)
				},
				// Unk_XUIDCheck2
				{
					{ GameVersion::v1_20_4_7623265_REPLAY },
					{ "48 8D 0D ? ? ? ? E8 ? ? ? ? 4C 8D 0D ? ? ? ? 48 89 44 24", SETUP_MOD(Add(3).Rip()) },
					SETUP_POINTER(Unk_XUIDCheck2)
				},
				// Unk_BNetClass
				{
					{ GameVersion::v1_20_4_7623265_REPLAY, GameVersion::v1_20_4_7623265_SHIP, GameVersion::v1_38_3_9489393, GameVersion::v1_44_0_10435696 },
					{ "83 3D ? ? ? ? ? 74 ? B8 ? ? ? ? C3", SETUP_MOD(Add(2).Rip().Add(1)) },
					SETUP_POINTER(Unk_BNetClass)
				}
			}
		};
	}

	Pointers::Pointers() {
		this->GetPointerList().Apply();
	}
}