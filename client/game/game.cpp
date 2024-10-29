#include "common.hpp"
#include "game/game.hpp"
#include "memory/memory.hpp"
#include <utility/memory.hpp>
#include <utility/nt.hpp>

#define SETUP_POINTER(name) #name, (void**)&this->m_##name
#define SETUP_MOD(mod) [](Memory::ScannedResult<void> r) { return r.##mod##; }

namespace Client::Game {
	void Pointers::PointerList::Apply() {
		for (PointerCalculator ptr : this->m_Pointers) {
			Memory::ScannedResult<void> res = Memory::SigScan(ptr.m_Calculator.m_Signature, this->m_Module, ptr.m_Name);
			if (res) {
				ptr.m_Calculator.m_Mod(res).Apply(ptr.m_Pointer);
			}
		}
	}

	Pointers::PointerList Pointers::GetPointerList() {
		return {
			g_LaunchInfo.second, {
				// unk_AuthManager
				{
					{ "4C 8B 35 ? ? ? ? 4C 89 BC 24 ? ? ? ? 49 BF", SETUP_MOD(Add(3).Rip()) },
					SETUP_POINTER(unk_AuthManager)
				}
			}
		};
	}

	Pointers::Pointers() {
		this->GetPointerList().Apply();

		/*Memory::GetExport("LoadImageA", "user32.dll")
			.Apply(&this->m_LoadImageA);
		Memory::GetExport("SetCursorPos", "user32.dll")
			.Apply(&this->m_SetCursorPos);
		Memory::GetExport("closesocket", "wsock32.dll")
			.Apply(&this->m_WS_CloseSocket);
		Memory::GetExport("connect", "wsock32.dll")
			.Apply(&this->m_WS_Connect);
		Memory::GetExport("gethostbyname", "wsock32.dll")
			.Apply(&this->m_WS_GetHostByName);
		Memory::GetExport("ioctlsocket", "wsock32.dll")
			.Apply(&this->m_WS_IoctlSocket);
		Memory::GetExport("recv", "wsock32.dll")
			.Apply(&this->m_WS_Recv);
		Memory::GetExport("recvfrom", "wsock32.dll")
			.Apply(&this->m_WS_RecvFrom);
		Memory::GetExport("select", "wsock32.dll")
			.Apply(&this->m_WS_Select);
		Memory::GetExport("send", "wsock32.dll")
			.Apply(&this->m_WS_Send);
		Memory::GetExport("sendto", "wsock32.dll")
			.Apply(&this->m_WS_SendTo);*/

		// patch auth :)
		/*if (this->m_unk_AuthManager) {
			const auto authMgr = *this->m_unk_AuthManager;
			for (int i = 0; i < *reinterpret_cast<int*>(authMgr + 0x8); i++) {
				uintptr_t temp = *reinterpret_cast<uintptr_t*>(authMgr + 0x10);
				if (temp) {
					*reinterpret_cast<int*>(temp + (0xD0 * i) + 0x14) = 0;
					*reinterpret_cast<int*>(temp + (0xD0 * i) + 0x48) = 2;
					*reinterpret_cast<int*>(temp + (0xD0 * i) + 0x4C) = 3;
				}
			}
			LOG("Pointers", INFO, "Patched authentication.");
		}
		else {
			LOG("Pointers", ERROR, "Failed to patch Call of Duty: Black Ops Cold War authentication because the pointer is null.");
		}*/
	}
}