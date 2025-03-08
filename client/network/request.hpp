#pragma once
#include "common.hpp"
#include "network/ext/winhttpclient.hpp"

#include <utility/strings.hpp>

namespace Client::Network {
	class Response {
	public:
		Response(const std::wstring& header, const std::wstring& content)
			: m_Header(header)
			, m_Content(content)
		{}

		Response(const std::string& header, const std::string& content)
			: Response(Common::Utility::Strings::Convert(header), Common::Utility::Strings::Convert(content))
		{}

		Response(Ext::WinHttpClient& client)
			: Response(client.GetResponseHeader(), client.GetResponseContent())
		{}

		std::wstring GetHeaderW() {
			return this->m_Header;
		}

		std::wstring GetContentW() {
			return this->m_Content;
		}

		std::string GetHeaderA() {
			return Common::Utility::Strings::Convert(this->GetHeaderW());
		}

		std::string GetContentA() {
			return Common::Utility::Strings::Convert(this->GetContentW());
		}
	private:
		const std::wstring m_Header;
		const std::wstring m_Content;
	};

	class Request {
	public:
		Request(const wchar_t* userAgent, bool certificates) {
			wcsncpy_s(m_UserAgent, userAgent, sizeof(userAgent));
			m_Certificates = certificates;
		}

		Response Post(const std::wstring& url, const char* postData, ...) {
			constexpr int bufferSize = SHRT_MAX + 1;
			static char buffer[bufferSize];

			va_list args;
			va_start(args, postData);
			int length = vsnprintf(buffer, bufferSize, postData, args);
			va_end(args);

			Ext::WinHttpClient client(url);

			client.SetRequireValidSslCertificates(this->m_Certificates);
			client.SetUserAgent(this->m_UserAgent);
			std::string data(buffer);
			client.SetAdditionalDataToSend((std::uint8_t*)data.c_str(), data.size());

			wchar_t szSize[50] = L"";
			swprintf_s(szSize, L"%zd", data.size());
			std::wstring headers = L"Content-Length: ";
			headers += szSize;
			headers += L"\r\nContent-Type: application/x-www-form-urlencoded\r\n";
			client.SetAdditionalRequestHeaders(headers);

			client.SendHttpRequest(L"POST");

			return { client };
		}

		Response Get(const std::wstring& url) {
			Ext::WinHttpClient client(url);

			client.SetRequireValidSslCertificates(this->m_Certificates);
			client.SetUserAgent(this->m_UserAgent);
			client.SendHttpRequest();

			return { client };
		}

		bool DownloadFile(const std::wstring& url, const std::wstring& filePath) {
			Ext::WinHttpClient client(url);

			client.SetUserAgent(this->m_UserAgent);
			return client.SaveResponseToFile(filePath);
		}
	private:
		wchar_t m_UserAgent[255];
		bool m_Certificates;
	};
}
