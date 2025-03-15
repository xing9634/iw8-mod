#pragma once
#include "common.hpp"
#include "engine/iw8/enums/LocalClientNum_t.hpp"
#include "engine/iw8/enums/UI_KEYBOARD_RESULT.hpp"

namespace Client::Hook::Util {
	void* InitDbgHelp();
	LONG WINAPI TopLevelExceptionFilter(EXCEPTION_POINTERS* inf);

	void OnPlayerNameInput(IW8::LocalClientNum_t localClientNum, IW8::UI_KEYBOARD_RESULT res, const char* text);

	inline std::vector<std::function<void()>> g_GameThreadQueue{};
	inline bool g_ForceSignInState = false;
	inline bool g_WaitingToHook = true;
	inline bool g_HooksInitialised = false;
}
