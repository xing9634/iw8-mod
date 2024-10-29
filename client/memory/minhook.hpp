#pragma once
#include "common.hpp"

#include <MinHook.h>

namespace Client::Memory {
    template <typename P = void>
    class MinHook {
    public:
        enum {
            ENV_ALL,
            ENV_CLIENT,
            ENV_DEDICATED,
            ENV_ONLINE
        };

        MinHook(P* pointer = nullptr, int environment = ENV_ALL)
            : m_DetourPointer(pointer)
            , m_Environment(environment)
        {}

        bool Eligible() {
            if (this->m_Environment != ENV_ALL) {
                if (g_LaunchInfo.first == Common::GameLaunchType::DEDICATED && this->m_Environment != ENV_DEDICATED) {
                    return false;
                }
                if (g_LaunchInfo.first != Common::GameLaunchType::DEDICATED && this->m_Environment != ENV_CLIENT) {
                    return false;
                }
                if (g_LaunchInfo.first == Common::GameLaunchType::SP && this->m_Environment == ENV_ONLINE) {
                    return false;
                }
            }
            if (this->m_DetourPointer == nullptr) {
                return false;
            }
            return true;
        }

        template <typename T>
        void Hook() {
            if (!this->Eligible()) {
                return;
            }

            MH_CreateHook(this->m_DetourPointer, &T::hkCallback, (void**)(&T::m_Original));
            MH_EnableHook(this->m_DetourPointer);

            LOG("MinHook", DEBUG, "Hooked {}.", T::GetName());
        }

        template <typename T>
        void Hook(std::uint32_t idx) {
            if (!this->Eligible()) {
                return;
            }

            MH_CreateHook(VT_GET(this->m_DetourPointer, idx), &T::hkCallback, (void**)(&T::m_Original));
            MH_EnableHook(VT_GET(this->m_DetourPointer, idx));

            LOG("MinHook", DEBUG, "Hooked {}. (VT: {})", T::GetName(), idx);
        }

        void Unhook() {
            if (!this->Eligible()) {
                return;
            }

            MH_DisableHook(this->m_DetourPointer);
            MH_RemoveHook(this->m_DetourPointer);
        }

        void Unhook(std::uint32_t idx) {
            if (!this->Eligible()) {
                return;
            }

            MH_DisableHook(VT_GET(this->m_DetourPointer, idx));
            MH_RemoveHook(VT_GET(this->m_DetourPointer, idx));
        }
    private:
        LPVOID m_DetourPointer;
        DWORD m_VProtect;
        int m_Environment;
    };
}