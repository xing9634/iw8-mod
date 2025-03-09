#include "common.hpp"
#include "hooks/hook.hpp"
#include "hooks/util/hook_util.hpp"

template <>
const char* Client::Hook::Hooks::HK_Live_GetLocalClientName::hkCallback() {
	return Hook::Util::g_PlayerName.c_str();
}
