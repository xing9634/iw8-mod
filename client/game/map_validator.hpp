#pragma once
#include "common.hpp"

namespace Client::Game::MapValidator {
	inline std::map<std::string, bool> g_LoadedFastfiles{};
	inline std::string g_QueuedErrorMessage{};
	DWORD Thread(PVOID);
}
