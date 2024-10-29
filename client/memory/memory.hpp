#pragma once
#include "common.hpp"
#include "memory/scanned_result.hpp"
#include <common/utility/utility.hpp>

namespace Client::Memory {
    struct MaskedSignature {
        std::vector<std::uint8_t> data{};
        std::vector<std::uint8_t> mask{};
    };

    template <typename T = void*>
    inline std::enable_if_t<std::is_pointer_v<T>, ScannedResult<std::remove_pointer_t<T>>> MaskedSigScan(MaskedSignature sig, std::string peModule) {
        std::size_t patLen = sig.mask.size();
        MODULEINFO modInfo = Common::Utility::GetModuleInfo(peModule);
        std::size_t modBase = (std::size_t)(modInfo.lpBaseOfDll);
        std::size_t modLen = (std::size_t)(modInfo.SizeOfImage);

        for (std::size_t ix = 0; ix < modLen - patLen; ix++) {
            bool found = true;

            for (std::size_t jx = 0; jx < patLen; jx++) {
                if (sig.mask[jx] != '?' && sig.data[jx] != *(std::uint8_t*)(modBase + ix + jx)) {
                    found = false;
                    break;
                }
            }

            if (found) {
                return ScannedResult<std::remove_pointer_t<T>>((void*)(modBase + ix));
            }
        }

        return ScannedResult<std::remove_pointer_t<T>>(nullptr);
    }

    template <typename T = void*>
    inline std::enable_if_t<std::is_pointer_v<T>, ScannedResult<std::remove_pointer_t<T>>> SigScan(std::string pattern, std::string peModule, std::string name = "<unknown>") {
        MaskedSignature sig{};

        char* base = const_cast<char*>(pattern.c_str());

        for (char* cur = base; cur < base + pattern.length(); cur++) {
            if ('\?' != *cur) { // check is ?
                sig.data.push_back((std::uint8_t)(strtoul(cur, &cur, 16)));
                sig.mask.push_back('x');
                continue;
            }

            cur++;
            if ('\?' == *cur) { // check is double ?
                cur++;
            }

            sig.data.push_back((std::uint8_t)'\xFF');
            sig.mask.push_back('?');
        }

        ScannedResult<std::remove_pointer_t<T>> res = MaskedSigScan<T>(sig, peModule);
        if (res) {
            LOG("Scanner", DEBUG, "Found '{}' {}+0x{:X}", name, peModule, res.As<std::uintptr_t>());
        }
        else {
            LOG("Scanner", WARN, "Failed to find '{}' in {} ({})", name, peModule, pattern);
        }
        return res;
    }

    template <typename T = void*>
    inline std::enable_if_t<std::is_pointer_v<T>, ScannedResult<std::remove_pointer_t<T>>> GetExport(std::string exportName, std::string peModule) {
        HMODULE mod = GetModuleHandleA(peModule.c_str());
        if (!mod) {
            LOG("Scanner", WARN, "Failed to find module '{}' while trying to find '{}'", exportName, peModule);
            return ScannedResult<std::remove_pointer_t<T>>(nullptr);
        }

        ScannedResult<std::remove_pointer_t<T>> res = ScannedResult<std::remove_pointer_t<T>>(reinterpret_cast<void*>(GetProcAddress(mod, exportName.c_str())));
        if (res) {
            LOG("Scanner", DEBUG, "Found '{}' {}+0x{:X}", exportName, peModule, res.As<std::uintptr_t>());
        }
        else {
            LOG("Scanner", WARN, "Failed to find '{}' in {}", exportName, peModule);
        }
        return res;
    }
}
