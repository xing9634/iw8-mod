#pragma once
#include "memory/minhook.hpp"
#include <global/string_literal.hpp>

namespace Client::HookPlate {
    template <Common::StringLiteral Name, typename Type, typename ...Args>
    struct StdcallHook {
        using funcType = Type __stdcall(Args...);
        static funcType hkCallback;
        static inline funcType* m_Original;

        static inline const char* GetName() {
            return Name.m_Value;
        }
    };

    template <Common::StringLiteral Name, typename Type, typename ...Args>
    struct CdeclHook {
        using funcType = Type __cdecl(Args...);
        static funcType hkCallback;
        static inline funcType* m_Original;

        static inline const char* GetName() {
            return Name.m_Value;
        }
    };

    template <Common::StringLiteral Name, typename Type, typename ...Args>
    struct FastcallHook {
        using funcType = Type __fastcall(Args...);
        static funcType hkCallback;
        static inline funcType* m_Original;

        static inline const char* GetName() {
            return Name.m_Value;
        }
    };

    template <Common::StringLiteral Name, typename TypeDecl>
    struct FnHook {
        using funcType = TypeDecl;
        static funcType* hkCallback;
        static inline funcType* m_Original;

        static inline const char* GetName() {
            return Name.m_Value;
        }
    };
}