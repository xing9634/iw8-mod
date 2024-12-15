#include "common.hpp"
#include "hooks/hook.hpp"

template <>
std::int64_t Client::Hook::Hooks::HK_dwGetLogOnStatus::hkCallback(int controllerIndex) {
	return 2;
}
