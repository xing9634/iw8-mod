#include "common.hpp"
#include "network/network.hpp"
#include "network/updater.hpp"

#include <utility/strings.hpp>

namespace Client::Network {
	void Updater::CheckForUpdates() {
		std::string apiUrl = "https://api.github.com/repos/xifil/iw8-mod/actions/runs?event=push";
		std::optional<nlohmann::json> responseOpt = GetContentAsJson(apiUrl);
		if (!responseOpt.has_value()) {
			return;
		}

		nlohmann::json response = responseOpt.value();
		if (!response.contains("workflow_runs")) {
			return;
		}

		nlohmann::json workflowRuns = response["workflow_runs"];
		std::string currentCommit = GIT_HASH;

		bool foundCurrentVersion = false;
		std::size_t versionsOutOfDate = 0;
		std::string newReleaseUrl = "";

		for (const auto& run : workflowRuns) {
			if (newReleaseUrl.empty()) {
				newReleaseUrl = run["html_url"].get<std::string>();
			}

			if (run["head_sha"].get<std::string>() == currentCommit) {
				foundCurrentVersion = true;
				break;
			}

			versionsOutOfDate++;
		}

		if (!foundCurrentVersion || versionsOutOfDate == 0) {
			return;
		}

		// not everyone wants to sign in to GitHub
		newReleaseUrl = Common::Utility::Strings::Replace(newReleaseUrl, "github.com", "nightly.link");

		int res = MessageBoxA(NULL, std::format("The current version of iw8-mod you are running is {} version{} out of date."
			" Would you like to open the download to the latest version? Opening the latest version will also close the game.",
			versionsOutOfDate, versionsOutOfDate == 1 ? "" : "s").c_str(),
			"iw8-mod", MB_YESNO | MB_ICONQUESTION);

		if (res == IDYES) {
			ShellExecuteA(NULL, "open", newReleaseUrl.c_str(), NULL, NULL, SW_SHOWNORMAL);
			exit(ERROR_SUCCESS);
		}
	}
}
