#include "common.hpp"
#include "hooks/hook.hpp"

bool flipSwitch = false;
std::int64_t startTime = 0;

template <>
bool Client::Hook::Hooks::HK_DWServicesAccess__isReady::hkCallback(void* _this, const int controllerIndex) {
	_Unreferenced_parameter_(_this);
	_Unreferenced_parameter_(controllerIndex);

	/*
		SEIZURE WARNING: DO NOT RETURN TRUE
	*/
	return false;

	/*if (startTime == 0) {
		startTime = std::time(nullptr);
	}

	if (!flipSwitch && std::time(nullptr) > startTime + 20) {
		flipSwitch = true;
		LOG("Game/DWServicesAccess::isReady", INFO, "Now returning true. WCGW?");
	}

	return flipSwitch;*/
}
