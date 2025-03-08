#include "common.hpp"
#include "network/network.hpp"
#include "network/request.hpp"

#include <curl/curl.h>

namespace Client {
	/*size_t Network::WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
		reinterpret_cast<std::string*>(userp)->append(reinterpret_cast<char*>(contents), size * nmemb);
		return size * nmemb;
	}*/

	/*std::optional<std::string> Network::GetContent(const std::string& url) {
		CURL* curl;
		CURLcode res;
		std::string readBuffer;

		curl_global_init(CURL_GLOBAL_DEFAULT);
		curl = curl_easy_init();

		if (curl) {
			curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

			res = curl_easy_perform(curl);

			if (res != CURLE_OK) {
				LOG("Network", ERROR, "curl_easy_perform() failed: {}", curl_easy_strerror(res));
			}

			curl_easy_cleanup(curl);
		}
		curl_global_cleanup();

		return readBuffer.empty() ? std::nullopt : std::optional{ readBuffer };
	}*/

	std::optional<std::string> Network::GetContent(const std::string& url) {
		std::string result{};

		try {
			result = Request(L"Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:128.0) Gecko/20100101 Firefox/128.0", false)
				.Get(Common::Utility::Strings::Convert(url)).GetContentA();
			LOG("Updater", DEBUG, "[{}] result = {}", url, result);
		}
		catch (...) {
			LOG("Network", DEBUG, "[{}] failed to content", url);
		}

		return result.empty() ? std::nullopt : std::optional{ result };
	}
}
