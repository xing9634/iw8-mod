#include "common.hpp"
#include "hooks/hook.hpp"

template <>
const char* Client::Hook::Hooks::HK_Live_GetLocalClientName::hkCallback() {
	return "Travailleur Scooter";
}
