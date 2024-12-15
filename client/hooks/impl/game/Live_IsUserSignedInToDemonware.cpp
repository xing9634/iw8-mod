#include "common.hpp"
#include "hooks/hook.hpp"

template <>
bool Client::Hook::Hooks::HK_Live_IsUserSignedInToDemonware::hkCallback(int controllerIndex) {
	return true;
}
