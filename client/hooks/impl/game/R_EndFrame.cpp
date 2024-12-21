#include "common.hpp"
#include "engine/iw8/XUID.hpp"
#include "game/game.hpp"
#include "hooks/hook.hpp"
#include "memory/memory.hpp"

bool s_PatchedAuth = false;
int s_FramesPassed = 0;

template <>
void Client::Hook::Hooks::HK_R_EndFrame::hkCallback() {
	if (g_Pointers->m_R_AddCmdDrawText) {
		IW8::GfxFont* font = g_Pointers->m_sharedUiInfo_assets->m_SubtitleFont;
		float color[4] = { .666f, .666f, .666f, .666f };
		g_Pointers->m_R_AddCmdDrawText("iw8-mod: " GIT_DESCRIBE " ["
#			ifdef _SHIP
				"ship"
#			else
				"non-ship"
#			endif
			"]", 0x7FFFFFFF, font, font->m_PixelHeight,
			4.f, 4.f + static_cast<float>(font->m_PixelHeight), 1.f, 1.f, 0.f, color);
	}

	if (!s_PatchedAuth) {
		if (s_FramesPassed > 500) {
			IW8::XUID xuid;
			xuid.RandomXUID();
			std::uint64_t xuidMagic = 0x11CB1243B8D7C31E;
			std::uint64_t xuidId = xuid.m_ID * xuid.m_ID;

			LOG("Game/R_EndFrame", INFO, "Patching auth...");
			g_Pointers->m_Unk_BNetClass->m_FinishedAuth = true;

			*Memory::SigScan("48 8D 3D ? ? ? ? 0F 1F 44 00 ? 48 63 C3", g_GameModuleName, "XUID Check #1").Add(3).Rip()
				.As<std::uint64_t*>() = xuidMagic | xuidId;
			// the below patch (0x14F05ACE8) seems to be replay only, with this being xref 5 of 6, however only 5 exist
			// in ship, and this is the one excluded.
#			ifdef _REPLAY
				*Memory::SigScan("48 8D 0D ? ? ? ? E8 ? ? ? ? 4C 8D 0D ? ? ? ? 48 89 44 24", g_GameModuleName, "XUID Check #2").Add(3).Rip()
					.As<std::uint64_t*>() = xuidMagic | xuidId;
#			endif

			(*g_Pointers->m_s_presenceData)[0].m_Current.m_CrossTitlePresenceData.m_PlatformID = xuidMagic | xuidId / 6;

			*Memory::SigScan("80 3D ? ? ? ? ? 74 ? 48 89 7C 24", g_GameModuleName, "Auth Check #1").Add(2).Rip().Add(1)
				.As<int*>() = 1;
			g_Pointers->m_Unk_BNetClass->m_State = 2;
			g_Pointers->m_Unk_BNetClass->m_FinishedAuth = true;

			IW8::dvar_t* xp_dec_dc = g_Pointers->m_Dvar_FindVarByName("NTTRLOPQKS");
			if (xp_dec_dc != nullptr) {
				xp_dec_dc->m_Current.m_Enabled = false;
			}

			//utils::hook::set(0x14E5C0730_g, 2);
#			ifdef _REPLAY
				*Memory::SigScan("48 8D 2D ? ? ? ? 84 C0 74 ? 48 69 C7", g_GameModuleName, "Auth Check #2").Add(3).Rip()
					.As<int*>() = 2;
#			else
				*Memory::SigScan("83 3D ? ? ? ? ? 7E ? 33 C9", g_GameModuleName, "Auth Check #2").Add(2).Rip()
					.As<int*>() = 2;
#			endif
			/*
#			ifdef _REPLAY
				*Memory::SigScan("4C 8D 35 ? ? ? ? 48 8B D8", g_GameModuleName, "Auth Check #2").Add(3).Rip()
					.As<int*>() = 2;
#			else
				int* test4 = Memory::SigScan("83 3D ? ? ? ? ? 7E ? 33 C9", g_GameModuleName, "Auth Check #2 (test 4)").Add(2).Rip()
					.As<int*>();
				LOG("Game/R_EndFrame", INFO, "test 4 = 0x{:016X}", reinterpret_cast<std::size_t>(test4));
				*test4 = 2;
				int* test5 = Memory::SigScan("48 8D 2D ? ? ? ? 84 C0 74 ? 48 69 C7", g_GameModuleName, "Auth Check #2 (test 5)").Add(3).Rip()
					.As<int*>();
				LOG("Game/R_EndFrame", INFO, "test 5 = 0x{:016X}", reinterpret_cast<std::size_t>(test5));
				*test5 = 2;
#			endif
			*/

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
