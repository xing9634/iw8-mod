#include "common.hpp"
#include "hooks/hook.hpp"

std::map<std::pair<const char*, const char*>, bool> s_PatchedBooleanDvars = {
	{ { "cg_autoSkipSplashScreen", "NPLRKNKKOP" }, true },
	{ { "cg_drawBuildName", "LSSSQMQPNL" }, true },
	{ { "cg_drawFastfileDebugInfo", "MOSSSSTTNL" }, true },
	{ { "cg_drawFPS", "OLNTNRTPPL" }, true },
	{ { "cg_drawFrontendSceneDebugInfo", "OMPMKKTORN" }, true },
	{ { "cl_waterMarkEnabled", "LRKNROSQPM" }, true },
	{ { "com_checkIfGameModeInstalled", "RLSPOOTTT" }, false },
	{ { "com_force_premium", "MROLPRPTPO" }, true },
	{ { "con_minicon", "LMSLLSMONN" }, true },
	{ { "force_offline_enabled", "MPSSOTQQPM" }, true },
	{ { "force_offline_menus", "LSTQOKLTRN" }, true },
	{ { "lui_cod_points_enabled", "LNTOKPTKS" }, false },
	{ { "lui_enable_magma_blade_layout", "LRKPTLNQTT" }, false },
	{ { "lui_force_online_menus", "LMMRONPQMO" }, false },
	{ { "ui_onlineRequired", "MTSTMKPMRM" }, false }
};

template <>
IW8::dvar_t* Client::Hook::Hooks::HK_Dvar_RegisterBool::hkCallback(const char* dvarName, bool value, std::uint32_t flags, const char* description) {
	bool valuePatched = value;
	for (const auto& [names, val] : s_PatchedBooleanDvars) {
		if (strcmp(dvarName, names.second) == 0) {
			LOG("Game/Dvar_RegisterBool", INFO, "Patched '{}' -> {}", names.first, val ? "true" : "false");
			valuePatched = val;
		}
	}
	return m_Original(dvarName, valuePatched, flags, description);
}
