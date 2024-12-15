#include "common.hpp"
#include "hooks/hook.hpp"

template <>
IW8::dvar_t* Client::Hook::Hooks::HK_Dvar_RegisterBool::hkCallback(const char* dvarName, bool value, std::uint32_t flags, const char* description) {
	bool valuePatched = value;
	if (strcmp(dvarName, "LSTQOKLTRN") == 0) {
		LOG("Game/Dvar_RegisterBool", INFO, "Patched 'force_offline_menus' -> true");
		valuePatched = true;
	}
	if (strcmp(dvarName, "MPSSOTQQPM") == 0) {
		LOG("Game/Dvar_RegisterBool", INFO, "Patched 'force_offline_enabled' -> true");
		valuePatched = true;
	}
	if (strcmp(dvarName, "LNTOKPTKS") == 0) {
		LOG("Game/Dvar_RegisterBool", INFO, "Patched 'lui_cod_points_enabled' -> false");
		valuePatched = false;
	}
	if (strcmp(dvarName, "LRKPTLNQTT") == 0) {
		LOG("Game/Dvar_RegisterBool", INFO, "Patched 'lui_enable_magma_blade_layout' -> false");
		valuePatched = false;
	}
	return m_Original(dvarName, valuePatched, flags, description);
}
