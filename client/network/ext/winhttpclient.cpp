#include "common.hpp"
#include "network/ext/winhttpclient.hpp"

namespace Client::Network::Ext {
	WinHttpClient::WinHttpClient(const std::wstring& url, ProgressProcT progressProc)
		: m_RequestURL(url)
		, m_SessionHandle(nullptr)
		, m_RequireValidSSL(false)
		, m_ResponseHeader(L"")
		, m_ResponseContent(L"")
		, m_ResponseCharset(L"")
		, m_RequestHost(L"")
		, m_Response(nullptr)
		, m_ResponseByteCountReceived(0)
		, m_ProcessProc(progressProc)
		, m_ResponseByteCount(0)
		, m_ResponseCookies(L"")
		, m_AdditionalRequestCookies(L"")
		, m_DataToSend(nullptr)
		, m_DataToSendSize(0)
		, m_Proxy(L"")
		, m_LastError(0)
		, m_StatusCode(L"")
		, m_UserAgent(SZ_AGENT)
		, m_ForceCharset(false)
		, m_ProxyUsername(L"")
		, m_ProxyPassword(L"")
		, m_Location(L"")
		, m_ResolveTimeout(0)
		, m_ConnectTimeout(60000)
		, m_SendTimeout(30000)
		, m_ReceiveTimeout(30000)
	{}

	WinHttpClient::~WinHttpClient() {
		if (this->m_Response != nullptr) {
			delete[] this->m_Response;
		}

		if (this->m_DataToSend != nullptr) {
			delete[] this->m_DataToSend;
		}

		if (this->m_SessionHandle != nullptr) {
			::WinHttpCloseHandle(this->m_SessionHandle);
		}
	}

	bool WinHttpClient::SendHttpRequest(const std::wstring& httpVerb, bool disableAutoRedirect) {
		if (this->m_RequestURL.size() <= 0) {
			this->m_LastError = ERROR_PATH_NOT_FOUND;
			return false;
		}

		// Make verb uppercase.
		std::wstring verb = httpVerb;
		if (_wcsicmp(verb.c_str(), L"GET") == 0) {
			verb = L"GET";
		}
		else if (_wcsicmp(verb.c_str(), L"POST") == 0) {
			verb = L"POST";
		}
		else {
			this->m_LastError = ERROR_INVALID_PARAMETER;
			return false;
		}

		bool retVal = true;

		if (this->m_SessionHandle == nullptr) {
			this->m_SessionHandle = ::WinHttpOpen(this->m_UserAgent.c_str(), WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
			if (this->m_SessionHandle == nullptr) {
				this->m_LastError = ::GetLastError();
				return false;
			}
		}

		::WinHttpSetTimeouts(this->m_SessionHandle, this->m_ResolveTimeout, this->m_ConnectTimeout, this->m_SendTimeout, this->m_ReceiveTimeout);

		wchar_t hostName[MAX_PATH] = L"";
		wchar_t urlPath[MAX_PATH * 5] = L"";
		URL_COMPONENTS urlComp;
		memset(&urlComp, 0, sizeof(urlComp));
		urlComp.dwStructSize = sizeof(urlComp);
		urlComp.lpszHostName = hostName;
		urlComp.dwHostNameLength = MAX_PATH;
		urlComp.lpszUrlPath = urlPath;
		urlComp.dwUrlPathLength = MAX_PATH * 5;
		urlComp.dwSchemeLength = 1; // None zero

		if (::WinHttpCrackUrl(this->m_RequestURL.c_str(), this->m_RequestURL.size(), 0, &urlComp)) {
			this->m_RequestHost = hostName;
			HINTERNET connect = ::WinHttpConnect(this->m_SessionHandle, hostName, urlComp.nPort, 0);
			if (connect != nullptr) {
				DWORD openRequestFlag = (urlComp.nScheme == INTERNET_SCHEME_HTTPS) ? WINHTTP_FLAG_SECURE : 0;
				HINTERNET request = ::WinHttpOpenRequest(connect, verb.c_str(), urlComp.lpszUrlPath, nullptr, WINHTTP_NO_REFERER,
					WINHTTP_DEFAULT_ACCEPT_TYPES, openRequestFlag);
				if (request != nullptr) {
					// If HTTPS, then client is very susceptable to invalid certificates
					// Easiest to accept anything for now
					if (!this->m_RequireValidSSL && urlComp.nScheme == INTERNET_SCHEME_HTTPS) {
						DWORD options = SECURITY_FLAG_IGNORE_CERT_CN_INVALID | SECURITY_FLAG_IGNORE_CERT_DATE_INVALID | SECURITY_FLAG_IGNORE_UNKNOWN_CA;
						::WinHttpSetOption(request, WINHTTP_OPTION_SECURITY_FLAGS, (LPVOID)&options, sizeof(DWORD));
					}

					bool getResponseSucceed = false;
					std::size_t retryTimes = 0;

					// Retry for several times if fails.
					while (!getResponseSucceed && retryTimes++ < INT_RETRYTIMES) {
						if (this->m_AdditionalRequestHeaders.size() > 0) {
							if (!::WinHttpAddRequestHeaders(request, this->m_AdditionalRequestHeaders.c_str(), this->m_AdditionalRequestHeaders.size(),
								WINHTTP_ADDREQ_FLAG_COALESCE_WITH_SEMICOLON))
							{
								this->m_LastError = ::GetLastError();
							}
						}
						if (this->m_AdditionalRequestCookies.size() > 0) {
							std::wstring cookies = L"Cookie: ";
							cookies += this->m_AdditionalRequestCookies;
							if (!::WinHttpAddRequestHeaders(request, cookies.c_str(), cookies.size(), WINHTTP_ADDREQ_FLAG_COALESCE_WITH_SEMICOLON)) {
								this->m_LastError = ::GetLastError();
							}
						}
						if (this->m_Proxy.size() > 0) {
							WINHTTP_PROXY_INFO proxyInfo;
							memset(&proxyInfo, 0, sizeof(proxyInfo));
							proxyInfo.dwAccessType = WINHTTP_ACCESS_TYPE_NAMED_PROXY;
							wchar_t szProxy[MAX_PATH] = L"";
							wcscpy_s(szProxy, MAX_PATH, this->m_Proxy.c_str());
							proxyInfo.lpszProxy = szProxy;

							if (!::WinHttpSetOption(request, WINHTTP_OPTION_PROXY, &proxyInfo, sizeof(proxyInfo))) {
								this->m_LastError = ::GetLastError();
							}

							if (this->m_ProxyUsername.size() > 0) {
								if (!::WinHttpSetOption(request, WINHTTP_OPTION_PROXY_USERNAME, (LPVOID)this->m_ProxyUsername.c_str(),
									this->m_ProxyUsername.size() * sizeof(wchar_t)))
								{
									this->m_LastError = ::GetLastError();
								}
								if (this->m_ProxyPassword.size() > 0) {
									if (!::WinHttpSetOption(request, WINHTTP_OPTION_PROXY_PASSWORD, (LPVOID)this->m_ProxyPassword.c_str(),
										this->m_ProxyPassword.size() * sizeof(wchar_t)))
									{
										this->m_LastError = ::GetLastError();
									}
								}
							}
						}

						if (disableAutoRedirect) {
							DWORD dwDisableFeature = WINHTTP_DISABLE_REDIRECTS;
							if (!::WinHttpSetOption(request, WINHTTP_OPTION_DISABLE_FEATURE, &dwDisableFeature, sizeof(dwDisableFeature))) {
								this->m_LastError = ::GetLastError();
							}
						}

						bool bSendRequestSucceed = false;
						if (::WinHttpSendRequest(request, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, NULL)) {
							bSendRequestSucceed = true;
						}
						else {
							// Query the proxy information from IE setting and set the proxy if any.
							WINHTTP_CURRENT_USER_IE_PROXY_CONFIG proxyConfig;
							memset(&proxyConfig, 0, sizeof(proxyConfig));
							if (::WinHttpGetIEProxyConfigForCurrentUser(&proxyConfig)) {
								if (proxyConfig.lpszAutoConfigUrl != nullptr) {
									WINHTTP_AUTOPROXY_OPTIONS autoProxyOptions;
									memset(&autoProxyOptions, 0, sizeof(autoProxyOptions));
									autoProxyOptions.dwFlags = WINHTTP_AUTOPROXY_AUTO_DETECT | WINHTTP_AUTOPROXY_CONFIG_URL;
									autoProxyOptions.dwAutoDetectFlags = WINHTTP_AUTO_DETECT_TYPE_DHCP;
									autoProxyOptions.lpszAutoConfigUrl = proxyConfig.lpszAutoConfigUrl;
									autoProxyOptions.fAutoLogonIfChallenged = TRUE;
									autoProxyOptions.dwReserved = 0;
									autoProxyOptions.lpvReserved = nullptr;

									WINHTTP_PROXY_INFO proxyInfo;
									memset(&proxyInfo, 0, sizeof(proxyInfo));

									if (::WinHttpGetProxyForUrl(this->m_SessionHandle, this->m_RequestURL.c_str(), &autoProxyOptions, &proxyInfo)) {
										if (::WinHttpSetOption(request, WINHTTP_OPTION_PROXY, &proxyInfo, sizeof(proxyInfo))) {
											if (::WinHttpSendRequest(request, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, NULL)) {
												bSendRequestSucceed = true;
											}
										}
										if (proxyInfo.lpszProxy != nullptr) {
											::GlobalFree(proxyInfo.lpszProxy);
										}
										if (proxyInfo.lpszProxyBypass != nullptr) {
											::GlobalFree(proxyInfo.lpszProxyBypass);
										}
									}
									else {
										this->m_LastError = ::GetLastError();
									}
								}
								else if (proxyConfig.lpszProxy != nullptr) {
									WINHTTP_PROXY_INFO proxyInfo;
									memset(&proxyInfo, 0, sizeof(proxyInfo));
									proxyInfo.dwAccessType = WINHTTP_ACCESS_TYPE_NAMED_PROXY;
									wchar_t szProxy[MAX_PATH] = L"";
									wcscpy_s(szProxy, MAX_PATH, proxyConfig.lpszProxy);
									proxyInfo.lpszProxy = szProxy;

									if (proxyConfig.lpszProxyBypass != nullptr) {
										wchar_t szProxyBypass[MAX_PATH] = L"";
										wcscpy_s(szProxyBypass, MAX_PATH, proxyConfig.lpszProxyBypass);
										proxyInfo.lpszProxyBypass = szProxyBypass;
									}

									if (!::WinHttpSetOption(request, WINHTTP_OPTION_PROXY, &proxyInfo, sizeof(proxyInfo))) {
										this->m_LastError = ::GetLastError();
									}
								}

								if (proxyConfig.lpszAutoConfigUrl != nullptr) {
									::GlobalFree(proxyConfig.lpszAutoConfigUrl);
								}

								if (proxyConfig.lpszProxy != nullptr) {
									::GlobalFree(proxyConfig.lpszProxy);
								}

								if (proxyConfig.lpszProxyBypass != nullptr) {
									::GlobalFree(proxyConfig.lpszProxyBypass);
								}
							}
							else {
								this->m_LastError = ::GetLastError();
							}
						}

						if (bSendRequestSucceed) {
							if (this->m_DataToSend != nullptr) {
								DWORD dwWritten = 0;
								if (!::WinHttpWriteData(request, this->m_DataToSend, this->m_DataToSendSize, &dwWritten)) {
									this->m_LastError = ::GetLastError();
								}
							}
							if (::WinHttpReceiveResponse(request, nullptr)) {
								DWORD dwSize = 0;
								BOOL bResult = FALSE;
								bResult = ::WinHttpQueryHeaders(request, WINHTTP_QUERY_RAW_HEADERS_CRLF, WINHTTP_HEADER_NAME_BY_INDEX, nullptr,
									&dwSize, WINHTTP_NO_HEADER_INDEX);
								if (bResult || (!bResult && (::GetLastError() == ERROR_INSUFFICIENT_BUFFER))) {
									wchar_t* szHeader = new wchar_t[dwSize];
									memset(szHeader, 0, dwSize * sizeof(wchar_t));
									if (::WinHttpQueryHeaders(request, WINHTTP_QUERY_RAW_HEADERS_CRLF, WINHTTP_HEADER_NAME_BY_INDEX, szHeader,
										&dwSize, WINHTTP_NO_HEADER_INDEX))
									{
										this->m_ResponseHeader.assign(szHeader);
										std::vector<std::wstring> result;
										std::wstring regExp = L"";
										if (!this->m_ForceCharset) {
											regExp = L"charset={[A-Za-z0-9\\-_]+}";
											if (ParseRegExp(regExp, false, 1, this->m_ResponseHeader, result) && result.size() > 0) {
												this->m_ResponseCharset = result[0];
											}
										}
										regExp = L"Content-Length: {[0-9]+}";
										if (ParseRegExp(regExp, false, 1, this->m_ResponseHeader, result) && result.size() > 0) {
											this->m_ResponseByteCount = static_cast<std::uint32_t>(_wtoi(result[0].c_str()));
										}
										regExp = L"Location: {[0-9]+}";
										if (ParseRegExp(regExp, false, 1, this->m_ResponseHeader, result) && result.size() > 0) {
											this->m_Location = result[0];
										}
										regExp = L"Set-Cookie:\\b*{.+?}\\n";
										if (ParseRegExp(regExp, false, 1, this->m_ResponseHeader, result) && result.size() > 0) {
											for (std::vector<std::wstring>::size_type i = 0; i < result.size(); i++) {
												this->m_ResponseCookies += result[i];
												if (i != result.size() - 1) {
													this->m_ResponseCookies += L"; ";
												}
											}
											this->m_ResponseCookies = Trim(this->m_ResponseCookies, L" ");
											if (this->m_ResponseCookies.size() > 0 && this->m_ResponseCookies[this->m_ResponseCookies.size() - 1] != L';') {
												this->m_ResponseCookies += L";";
											}
										}
									}
									delete[] szHeader;
								}

								dwSize = 0;
								bResult = ::WinHttpQueryHeaders(request, WINHTTP_QUERY_STATUS_CODE, WINHTTP_HEADER_NAME_BY_INDEX, nullptr,
									&dwSize, WINHTTP_NO_HEADER_INDEX);
								if (bResult || (!bResult && (::GetLastError() == ERROR_INSUFFICIENT_BUFFER))) {
									wchar_t* szStatusCode = new wchar_t[dwSize];
									memset(szStatusCode, 0, dwSize * sizeof(wchar_t));
									if (::WinHttpQueryHeaders(request, WINHTTP_QUERY_STATUS_CODE, WINHTTP_HEADER_NAME_BY_INDEX, szStatusCode,
										&dwSize, WINHTTP_NO_HEADER_INDEX))
									{
										this->m_StatusCode = szStatusCode;
									}
									delete[] szStatusCode;
								}

								std::uint32_t iMaxBufferSize = INT_BUFFERSIZE;
								std::uint32_t iCurrentBufferSize = 0;
								if (this->m_Response != nullptr) {
									delete[] this->m_Response;
									this->m_Response = nullptr;
								}
								this->m_Response = new std::uint8_t[iMaxBufferSize];
								if (this->m_Response == nullptr) {
									retVal = false;
									break;
								}
								memset(this->m_Response, 0, iMaxBufferSize);
								do {
									dwSize = 0;
									if (::WinHttpQueryDataAvailable(request, &dwSize)) {
										SetProgress(iCurrentBufferSize);
										std::uint8_t* pResponse = new std::uint8_t[dwSize + 1];
										memset(pResponse, 0, (dwSize + 1) * sizeof(std::uint8_t));
										DWORD dwRead = 0;
										if (::WinHttpReadData(request, pResponse, dwSize, &dwRead)) {
											if (dwRead + iCurrentBufferSize > iMaxBufferSize) {
												std::uint8_t* pOldBuffer = this->m_Response;
												this->m_Response = new std::uint8_t[iMaxBufferSize * 2];
												if (this->m_Response == nullptr) {
													this->m_Response = pOldBuffer;
													retVal = false;
													break;
												}
												iMaxBufferSize *= 2;
												memset(this->m_Response, 0, iMaxBufferSize);
												memcpy(this->m_Response, pOldBuffer, iCurrentBufferSize);
												delete[] pOldBuffer;
											}
											memcpy(this->m_Response + iCurrentBufferSize, pResponse, dwRead);
											iCurrentBufferSize += dwRead;
										}
										delete[] pResponse;
									}
									else {
										this->m_LastError = ::GetLastError();
									}
								} while (dwSize > 0);
								SetProgress(iCurrentBufferSize);
								this->m_ResponseByteCountReceived = iCurrentBufferSize;

								UINT codePage = CP_ACP;
								DWORD dwFlag = MB_PRECOMPOSED;
								if (_wcsnicmp(this->m_ResponseCharset.c_str(), L"utf-8", 5) == 0) {
									codePage = CP_UTF8;
									dwFlag = 0;
								}
								int iLength = ::MultiByteToWideChar(codePage, dwFlag, reinterpret_cast<LPCSTR>(this->m_Response),
									this->m_ResponseByteCountReceived + 1, nullptr, 0);
								if (iLength <= 0) {
									// Use CP_ACP if UTF-8 fail
									codePage = CP_ACP;
									dwFlag = MB_PRECOMPOSED;
									iLength = ::MultiByteToWideChar(codePage, dwFlag, reinterpret_cast<LPCSTR>(this->m_Response),
										this->m_ResponseByteCountReceived + 1, nullptr, 0);
								}
								if (iLength > 0) {
									wchar_t* wideChar = new wchar_t[iLength];
									memset(wideChar, 0, iLength * sizeof(wchar_t));
									iLength = ::MultiByteToWideChar(codePage, dwFlag, reinterpret_cast<LPCSTR>(this->m_Response),
										this->m_ResponseByteCountReceived + 1, wideChar, iLength);
									if (iLength > 0) {
										this->m_ResponseContent = wideChar;
									}
									delete[] wideChar;
								}
								getResponseSucceed = true;

								// If the resposne html web page size is less than 200, retry.
								if (verb == L"GET" && !disableAutoRedirect) {
									std::wstring regExp = L"{<html>}";
									std::vector<std::wstring> result;
									if (ParseRegExp(regExp, false, 1, this->m_ResponseContent, result) && result.size() > 0) {
										regExp = L"{</html>}";
										if (!ParseRegExp(regExp, false, 1, this->m_ResponseContent, result) || result.size() <= 0) {
											this->m_LastError = ERROR_INVALID_DATA;
											getResponseSucceed = false;
										}
									}
								}
							}
							else {
								this->m_LastError = ::GetLastError();
							}
						}
					} // while

					if (!getResponseSucceed) {
						retVal = false;
					}

					::WinHttpCloseHandle(request);
				}
				::WinHttpCloseHandle(connect);
			}
		}

		return retVal;
	}

	std::wstring WinHttpClient::GetResponseHeader() {
		return this->m_ResponseHeader;
	}

	std::wstring WinHttpClient::GetResponseContent() {
		return this->m_ResponseContent;
	}

	std::wstring WinHttpClient::GetResponseCharset() {
		return this->m_ResponseCharset;
	}

	std::wstring WinHttpClient::GetRequestHost() {
		return this->m_RequestHost;
	}

	bool WinHttpClient::SaveResponseToFile(const std::wstring& filePath) {
		if (this->m_Response == nullptr || this->m_ResponseByteCountReceived <= 0) {
			return false;
		}

		FILE* f = nullptr;
		int result = _wfopen_s(&f, filePath.c_str(), L"wb");
		if (result == 0 && f != nullptr) {
			fwrite(this->m_Response, this->m_ResponseByteCountReceived, 1, f);
			fclose(f);
			return true;
		}

		return false;
	}

	bool WinHttpClient::SetProgress(std::uint32_t byteCountReceived) {
		bool success = false;
		if (this->m_ProcessProc != nullptr && this->m_ResponseByteCount > 0) {
			double dProgress = (double)byteCountReceived * 100 / this->m_ResponseByteCount;
			this->m_ProcessProc(dProgress);
			success = true;
		}

		return success;
	}

	std::wstring WinHttpClient::GetResponseCookies() {
		return this->m_ResponseCookies;
	}

	bool WinHttpClient::SetAdditionalRequestCookies(const std::wstring& cookies) {
		this->m_AdditionalRequestCookies = cookies;
		return true;
	}

	bool WinHttpClient::SetAdditionalDataToSend(std::uint8_t* data, std::uint32_t dataSize) {
		if (data == nullptr || dataSize < 0) {
			return false;
		}

		if (this->m_DataToSend != nullptr) {
			delete[] this->m_DataToSend;
		}

		this->m_DataToSend = new std::uint8_t[dataSize];
		if (this->m_DataToSend != nullptr) {
			memcpy(this->m_DataToSend, data, dataSize);
			this->m_DataToSendSize = dataSize;
			return true;
		}

		return false;
	}

	// Reset additional data fields
	bool WinHttpClient::ResetAdditionalDataToSend() {
		if (this->m_DataToSend != nullptr) {
			delete[] this->m_DataToSend;
		}

		this->m_DataToSend = nullptr;
		this->m_DataToSendSize = 0;

		return true;
	}

	// Allow us to reset the url on subsequent requests
	bool WinHttpClient::UpdateUrl(const std::wstring& url) {
		this->m_RequestURL = url;
		ResetAdditionalDataToSend();

		return true;
	}

	bool WinHttpClient::SetAdditionalRequestHeaders(const std::wstring& additionalRequestHeaders) {
		this->m_AdditionalRequestHeaders = additionalRequestHeaders;
		return true;
	}

	bool WinHttpClient::SetProxy(const std::wstring& proxy) {
		this->m_Proxy = proxy;
		return true;
	}

	// If we don't require valid SSL Certs then accept any
	// certificate on an SSL connection
	bool WinHttpClient::SetRequireValidSslCertificates(bool require) {
		this->m_RequireValidSSL = require;
		return true;
	}

	const std::uint8_t* WinHttpClient::GetRawResponseContent() {
		return this->m_Response;
	}

	std::uint32_t WinHttpClient::GetRawResponseContentLength() {
		return this->m_ResponseByteCount;
	}

	std::uint32_t WinHttpClient::GetRawResponseReceivedContentLength() {
		return this->m_ResponseByteCountReceived;
	}

	DWORD WinHttpClient::GetLastError() {
		return this->m_LastError;
	}

	std::wstring WinHttpClient::GetResponseStatusCode() {
		return this->m_StatusCode;
	}

	bool WinHttpClient::SetUserAgent(const std::wstring& userAgent) {
		this->m_UserAgent = userAgent;
		return true;
	}

	bool WinHttpClient::SetForceCharset(const std::wstring& charset) {
		this->m_ResponseCharset = charset;
		return true;
	}

	bool WinHttpClient::SetProxyUsername(const std::wstring& username) {
		this->m_ProxyUsername = username;
		return true;
	}

	bool WinHttpClient::SetProxyPassword(const std::wstring& password) {
		this->m_ProxyPassword = password;
		return true;
	}

	std::wstring WinHttpClient::GetResponseLocation() {
		return this->m_Location;
	}

	bool WinHttpClient::SetTimeouts(std::uint32_t resolveTimeout, std::uint32_t connectTimeout, std::uint32_t sendTimeout, std::uint32_t receiveTimeout) {
		this->m_ResolveTimeout = resolveTimeout;
		this->m_ConnectTimeout = connectTimeout;
		this->m_SendTimeout = sendTimeout;
		this->m_ReceiveTimeout = receiveTimeout;
		return true;
	}
}
