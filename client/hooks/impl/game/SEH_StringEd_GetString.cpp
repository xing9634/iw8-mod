#include "common.hpp"
#include "game/game.hpp"
#include "hooks/hook.hpp"

template <>
const char* Client::Hook::Hooks::HK_SEH_StringEd_GetString::hkCallback(const char* pszReference) {
	const char* ret = m_Original(pszReference);

	if (!pszReference[1]) {
		if ((*pszReference & 0x80) != 0) {
			return "t";
		}
		return pszReference;
	}

	g_Pointers->m_GamerProfile_SetDataByName(0, "acceptedEULA", 1);
	g_Pointers->m_GamerProfile_SetDataByName(0, "hasEverPlayed_MainMenu", 1);

	bool isMapUnavailable = false;
	isMapUnavailable |= strcmp(pszReference, "LUA_MENU/MAPNAME_ANIYAH") == 0;
	isMapUnavailable |= strstr(pszReference, "LUA_MENU/MAPNAME_M_CAGE") != nullptr;
	isMapUnavailable |= strstr(pszReference, "LUA_MENU/MAPNAME_CAVE_AM") != nullptr;
	isMapUnavailable |= strstr(pszReference, "LUA_MENU/MAPNAME_CAVE") != nullptr;
	isMapUnavailable |= strstr(pszReference, "LUA_MENU/MAPNAME_M_CARGO") != nullptr;
	isMapUnavailable |= strstr(pszReference, "LUA_MENU/MAPNAME_CRASH2") != nullptr;
	isMapUnavailable |= strstr(pszReference, "LUA_MENU/MAPNAME_M_OVERUNDER") != nullptr;
	isMapUnavailable |= strstr(pszReference, "LUA_MENU/MAPNAME_EUPHRATES") != nullptr;
	isMapUnavailable |= strstr(pszReference, "LUA_MENU/MAPNAME_RAID") != nullptr;
	isMapUnavailable |= strstr(pszReference, "LUA_MENU/MAPNAME_M_SHOWERS") != nullptr;
	isMapUnavailable |= strstr(pszReference, "LUA_MENU/MAPNAME_RUNNER_AM") != nullptr;
	isMapUnavailable |= strstr(pszReference, "LUA_MENU/MAPNAME_RUNNER") != nullptr;
	isMapUnavailable |= strstr(pszReference, "LUA_MENU/MAPNAME_HACKNEY_AM") != nullptr;
	isMapUnavailable |= strstr(pszReference, "LUA_MENU/MAPNAME_HACKNEY_YARD") != nullptr;
	isMapUnavailable |= strstr(pszReference, "LUA_MENU/MAPNAME_M_HILL") != nullptr;
	isMapUnavailable |= strstr(pszReference, "LUA_MENU/MAPNAME_PICCADILLY") != nullptr;
	isMapUnavailable |= strstr(pszReference, "LUA_MENU/MAPNAME_M_PINE") != nullptr;
	isMapUnavailable |= strstr(pszReference, "LUA_MENU/MAPNAME_SPEAR_AM") != nullptr;
	isMapUnavailable |= strstr(pszReference, "LUA_MENU/MAPNAME_SPEAR") != nullptr;
	isMapUnavailable |= strstr(pszReference, "LUA_MENU/MAPNAME_PETROGRAD") != nullptr;
	isMapUnavailable |= strstr(pszReference, "LUA_MENU/MAPNAME_M_STACK") != nullptr;
	isMapUnavailable |= strstr(pszReference, "LUA_MENU/MAPNAME_VACANT") != nullptr;
	if (isMapUnavailable) {
		return g_Pointers->m_j_va("%s ^1(Unavailable)", ret);
	}

	bool isLaunchOptionUnavailable = false;
	isLaunchOptionUnavailable |= strcmp(pszReference, "MENU_SP/CAMPAIGN") == 0;
	isLaunchOptionUnavailable |= strcmp(pszReference, "LUA_MENU/LOCAL_COOP_CAPS") == 0;
	if (isLaunchOptionUnavailable) {
		return g_Pointers->m_j_va("^1%s", ret);
	}

	if (strcmp(pszReference, "LUA_MENU/CAMPAIGN_DESC") == 0) {
		return "^3Campaign is not available in this build of the game.";
	}

	if (strcmp(pszReference, "LUA_MENU/LOCAL_COOP_DESC") == 0) {
		return "^3CO-OP is not available in this build of the game.";
	}

	if (strcmp(pszReference, "LUA_MENU/LOCAL_MULTIPLAYER_CAPS") == 0) {
		return "MULTIPLAYER";
	}

	if (strcmp(pszReference, "MENU/POPUP_CONNECTING_OFFLINE") == 0) {
		return "Welcome to iw8-mod. Loading...";
	}

	if (strcmp(pszReference, "LUA_MENU/PROTANOPIA") == 0) {
		return g_Pointers->m_j_va("%s (lifix gta 7 mode)", ret);
	}

	if (strcmp(pszReference, "MENU/STATUS") == 0) {
		return "iw8-mod: " GIT_DESCRIBE ", on " SELECT("ship", "non-ship") " build";
	}

	//if (strstr(pszReference, "LOCAL") != nullptr || strstr(pszReference, "local") != nullptr) {
	//	return g_Pointers->m_j_va("%s (%s)", ret, pszReference);
	//}
	//else {
		return ret;
	//}
}
