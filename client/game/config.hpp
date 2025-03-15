#pragma once
#include "common.hpp"

#include <nlohmann/json.hpp>
#include <lmcons.h>

namespace Client {
	namespace Game {
		class Config {
		public:
			Config(const std::string& configFile)
				: m_ConfigFile(configFile)
			{
				this->Load();
			}

			void Load() {
				if (!std::filesystem::exists(this->m_ConfigFile)) {
					this->m_JsonData["PlayerName"] = GetDefaultPlayerName();
					this->Save();
					return;
				}

				std::ifstream file(this->m_ConfigFile);

				if (file.is_open()) {
					try {
						file >> this->m_JsonData;
					}
					catch (const nlohmann::json::parse_error& e) {
						LOG("Config", ERROR, "Parsing error: {}", e.what());
					}
					file.close();
				}
			}

			void Save() const {
				std::ofstream file(this->m_ConfigFile);
				if (file.is_open()) {
					file << this->m_JsonData.dump(4);
					file.close();
				}
			}

			std::string GetPlayerName() const {
				return this->m_JsonData["PlayerName"].get<std::string>();
			}

			void SetPlayerName(const std::string& name) {
				this->m_JsonData["PlayerName"] = name;
				this->Save();
			}

		private:

			std::string GetDefaultPlayerName() {
				char username[UNLEN + 1];
				DWORD usernameLen = UNLEN + 1;

				if (GetUserNameA(username, &usernameLen)) {
					return std::string(username);
				}
				else {
					return "UnnamedPlayer";
				}
			}

			std::string m_ConfigFile;
			nlohmann::json m_JsonData;
		};
	}

	inline Game::Config g_Config{ "iw8-mod-config.json" };
}
