#include "common.hpp"
#include "hooks/hook.hpp"
#include "hooks/util/hook_util.hpp"

template <>
int Client::Hook::Hooks::HK_CL_GetLocalClientSignInState::hkCallback(int controllerIndex) {
	if (Hook::Util::g_ForceSignInState) {
		static Common::Utility::NT::Library game{};

		std::uintptr_t addrRel = 0;
		int arrElemSize = 0;

		if (GameVersionIsAny(GameVersion::v1_38_3_9489393)) {
			addrRel = 0x16AB3220;
			arrElemSize = 0x46;
		}
		else if (GameVersionIsAny(GameVersion::v1_44_0_10435696)) {
			addrRel = 0x17AF98A0;
			arrElemSize = 0x46;
		}

		if (addrRel != 0) {
			std::uintptr_t addr = reinterpret_cast<std::uintptr_t>(game.GetPtr()) + addrRel;
			int* arr = reinterpret_cast<int*>(addr);

			arr[arrElemSize * controllerIndex] = 2;
		}

		return 2;
	}

	return m_Original(controllerIndex);
}
