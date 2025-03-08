#pragma once

#pragma comment(lib, "Winhttp.lib")
#pragma warning (disable : 4267)

#include "network/ext/regexp.hpp"
#include "network/ext/string_process.hpp"

#include <comutil.h>
#include <windows.h>
#include <Winhttp.h>
#include <string>
#include <vector>

namespace Client::Network::Ext {
	using ProgressProcT = bool(*)(double);

	static const std::uint32_t INT_RETRYTIMES = 3;
	static wchar_t* SZ_AGENT = const_cast<wchar_t*>(L"WinHttpClient");  // has to be casted - side effect of C++17
	static const int INT_BUFFERSIZE = 10240;                            // Initial 10 KB temporary buffer, double if it is not enough.

	class WinHttpClient {
	public:
		WinHttpClient(const std::wstring& url, ProgressProcT progressProc = nullptr);
		~WinHttpClient();

		// It is a synchronized method and may take a long time to finish.
		bool SendHttpRequest(const std::wstring& httpVerb = L"GET", bool disableAutoRedirect = false);
		std::wstring GetResponseHeader();
		std::wstring GetResponseContent();
		std::wstring GetResponseCharset();
		std::wstring GetResponseStatusCode();
		std::wstring GetResponseLocation();
		std::wstring GetRequestHost();
		const std::uint8_t* GetRawResponseContent();
		std::uint32_t GetRawResponseContentLength();
		std::uint32_t GetRawResponseReceivedContentLength();
		bool SaveResponseToFile(const std::wstring& filePath);
		std::wstring GetResponseCookies();
		bool SetAdditionalRequestCookies(const std::wstring& cookies);
		bool SetAdditionalDataToSend(std::uint8_t* data, std::uint32_t dataSize);
		bool UpdateUrl(const std::wstring& url);
		bool ResetAdditionalDataToSend();
		bool SetAdditionalRequestHeaders(const std::wstring& additionalRequestHeaders);
		bool SetRequireValidSslCertificates(bool require);
		bool SetProxy(const std::wstring& proxy);
		DWORD GetLastError();
		bool SetUserAgent(const std::wstring& userAgent);
		bool SetForceCharset(const std::wstring& charset);
		bool SetProxyUsername(const std::wstring& username);
		bool SetProxyPassword(const std::wstring& password);
		bool SetTimeouts(std::uint32_t resolveTimeout = 0, std::uint32_t connectTimeout = 60000,
			std::uint32_t sendTimeout = 30000, std::uint32_t receiveTimeout = 30000);
	private:
		WinHttpClient(const WinHttpClient& other);
		WinHttpClient& operator =(const WinHttpClient& other);
		bool SetProgress(std::uint32_t byteCountReceived);

		HINTERNET m_SessionHandle;
		bool m_RequireValidSSL;
		std::wstring m_RequestURL;
		std::wstring m_RequestHost;
		std::wstring m_ResponseHeader;
		std::wstring m_ResponseContent;
		std::wstring m_ResponseCharset;
		std::uint8_t* m_Response;
		std::uint32_t m_ResponseByteCountReceived;  // Up to 4GB.
		ProgressProcT m_ProcessProc;
		std::uint32_t m_ResponseByteCount;
		std::wstring m_ResponseCookies;
		std::wstring m_AdditionalRequestCookies;
		std::uint8_t* m_DataToSend;
		std::uint32_t m_DataToSendSize;
		std::wstring m_AdditionalRequestHeaders;
		std::wstring m_Proxy;
		DWORD m_LastError;
		std::wstring m_StatusCode;
		std::wstring m_UserAgent;
		bool m_ForceCharset;
		std::wstring m_ProxyUsername;
		std::wstring m_ProxyPassword;
		std::wstring m_Location;
		std::uint32_t m_ResolveTimeout;
		std::uint32_t m_ConnectTimeout;
		std::uint32_t m_SendTimeout;
		std::uint32_t m_ReceiveTimeout;
	};
}
