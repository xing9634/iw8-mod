#pragma once
#include "common_core.hpp"

namespace Common {
	class Console {
	public:
		HANDLE GetOutputHandle();
		DWORD GetConsoleMode();
		void SetConsoleMode(DWORD consoleMode);

		void Write(std::string text);
		std::vector<std::string> m_Lines{};
		std::string m_CurrentLine{};
	};

	inline Console g_Console{};
}
