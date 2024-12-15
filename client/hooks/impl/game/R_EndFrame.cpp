#include "common.hpp"
#include "engine/iw8/XUID.hpp"
#include "game/game.hpp"
#include "hooks/hook.hpp"
#include "memory/memory.hpp"

bool s_PatchedAuth = false;
int s_FramesPassed = 0;

template <>
void Client::Hook::Hooks::HK_R_EndFrame::hkCallback() {
	if (!s_PatchedAuth) {
		if (s_FramesPassed > 500) {
			IW8::XUID xuid;
			xuid.RandomXUID();
			std::uint64_t xuidMagic = 0x11CB1243B8D7C31E;
			std::uint64_t xuidId = xuid.m_ID * xuid.m_ID;

			LOG("Game/R_EndFrame", INFO, "Patching auth...");
			g_Pointers->m_Unk_BNetClass->m_FinishedAuth = true;

			*Memory::SigScan("48 8D 3D ? ? ? ? 0F 1F 44 00 ? 48 63 C3", g_GameModuleName, "XUID Check #1").Add(3).Rip() // verified
				.As<std::uint64_t*>() = xuidMagic | xuidId;
			//utils::hook::set<uintptr_t>(0x14F05ACE8_g, xuidMagic | xuidId);
#			ifdef _REPLAY
				*Memory::SigScan("48 8D 0D ? ? ? ? E8 ? ? ? ? 4C 8D 0D ? ? ? ? 48 89 44 24", g_GameModuleName, "XUID Check #2").Add(3).Rip()
					.As<std::uint64_t*>() = xuidMagic | xuidId;
#			endif

			(*g_Pointers->m_s_presenceData)[0].m_Current.m_CrossTitlePresenceData.m_PlatformID = xuidMagic | xuidId / 6;

			//utils::hook::set<int>(0x14E371231_g, 1); // presumably is authed?
			*Memory::SigScan("80 3D ? ? ? ? ? 74 ? 48 89 7C 24", g_GameModuleName, "Auth Check #1").Add(2).Rip().Add(1) // verified
				.As<int*>() = 1;
			g_Pointers->m_Unk_BNetClass->m_State = 2;
			g_Pointers->m_Unk_BNetClass->m_FinishedAuth = true;

			//IW8::dvar_t* DVARBOOL_xp_dec_dc = reinterpret_cast<IW8::dvar_t*>(0x14EE560B0_g);
#			ifdef _REPLAY
				IW8::dvar_t* DVARBOOL_xp_dec_dc = Memory::SigScan("48 8B 05 ? ? ? ? 80 78 ? ? 74 ? 8B CE", g_GameModuleName, "DVARBOOL_xp_dec_dc").Add(3).Rip()
					.As<IW8::dvar_t*>();
				DVARBOOL_xp_dec_dc->m_Current.m_Enabled = false;
#			endif

			//utils::hook::set(0x14E5C0730_g, 2);
#			ifdef _REPLAY
				*Memory::SigScan("4C 8D 35 ? ? ? ? 48 8B D8", g_GameModuleName, "Auth Check #2").Add(3).Rip()
					.As<int*>() = 2;
#			endif

			g_Pointers->m_Unk_BNetClass->m_Var3 = 0x795230F0;
			g_Pointers->m_Unk_BNetClass->m_Var4 = 0x1F;
			g_Pointers->m_Unk_BNetClass->m_Var5 = 0x00000000;

			LOG("Game/R_EndFrame", INFO, "Patched auth.");
			s_PatchedAuth = true;
		}
		else {
			s_FramesPassed++;
		}
	}
	return m_Original();
}
