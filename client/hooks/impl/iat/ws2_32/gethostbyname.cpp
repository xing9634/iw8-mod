#include "common.hpp"
#include "hooks/hook.hpp"

template <>
hostent* WSAAPI Client::Hook::Hooks::HK_GetHostByName::hkCallback(const char* name) {
	LOG("Game/gethostbyname", DEBUG, "hello from GHBN hook: {}", name);
	return gethostbyname(name);
}
