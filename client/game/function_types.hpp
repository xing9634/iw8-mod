#pragma once
#include "common.hpp"

namespace Client::Game::Functions {
	using LoadImageAT = HANDLE(__stdcall)(HINSTANCE hInst, LPCSTR name, UINT type, int cx, int cy, UINT fuLoad);
	using SetCursorPosT = BOOL(int x, int y);
	using WS_CloseSocketT = int(__stdcall)(SOCKET s);
	using WS_ConnectT = int(__stdcall)(SOCKET s, const sockaddr* name, int namelen);
	using WS_GetHostByNameT = hostent * (__stdcall)(const char* name);
	using WS_IoctlSocketT = int(__stdcall)(SOCKET s, int cmd, u_long* argp);
	using WS_RecvT = int(__stdcall)(SOCKET s, char* buf, int len, int flags);
	using WS_RecvFromT = int(__stdcall)(SOCKET s, char* buf, int len, int flags, sockaddr* from, int* fromlen);
	using WS_SelectT = int(__stdcall)(int nfds, fd_set* readfds, fd_set* writefds, fd_set* exceptfds, timeval* timeout);
	using WS_SendT = int(__stdcall)(SOCKET s, const char* buf, int len, int flags);
	using WS_SendToT = int(__stdcall)(SOCKET s, const char* buf, int len, int flags, const sockaddr* to, int tolen);
}
