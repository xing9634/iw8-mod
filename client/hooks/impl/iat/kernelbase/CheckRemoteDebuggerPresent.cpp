#include "common.hpp"
#include "hooks/hook.hpp"

template <>
BOOL Client::Hook::Hooks::HK_CheckRemoteDebuggerPresent::hkCallback(HANDLE hProcess, PBOOL pbDebuggerPresent) {
	if (pbDebuggerPresent != nullptr) {
		*pbDebuggerPresent = false;
	}

	return true;
}
