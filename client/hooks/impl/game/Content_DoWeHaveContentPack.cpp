#include "common.hpp"
#include "hooks/hook.hpp"

template <>
bool Client::Hook::Hooks::HK_Content_DoWeHaveContentPack::hkCallback(int contentPack) {
	_Unreferenced_parameter_(contentPack);
	return true;
}
