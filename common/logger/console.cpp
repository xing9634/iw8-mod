#include "common_core.hpp"
#include "logger/console.hpp"
#include "logger/log_service.hpp"
#include "utility/utility.hpp"

namespace Common {
	HANDLE Console::GetOutputHandle() {
		return GetStdHandle(STD_OUTPUT_HANDLE);
	}

	DWORD Console::GetConsoleMode() {
		DWORD mode = 0;
		::GetConsoleMode(this->GetOutputHandle(), &mode);
		return mode;
	}

	void Console::SetConsoleMode(DWORD consoleMode) {
		::SetConsoleMode(this->GetOutputHandle(), consoleMode);
	}

	void Console::Write(std::string text) {
		this->SetConsoleMode(this->GetConsoleMode() | ENABLE_VIRTUAL_TERMINAL_PROCESSING);

		std::cout << text << std::flush;
		if (Utility::EndsWith(text, "\n")) {
			std::regex ansiEscape(R"(\x1B\[[0-?9;]*[mG])");
			std::string lineNoAnsi = std::regex_replace(this->m_CurrentLine + text, ansiEscape, "");
			this->m_Lines.push_back(lineNoAnsi);
			this->m_CurrentLine = "";
		}
		else {
			this->m_CurrentLine += text;
		}
	}
}
