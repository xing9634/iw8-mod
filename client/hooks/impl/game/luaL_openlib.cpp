#include "common.hpp"
#include "hooks/hook.hpp"

#include <utility/nt.hpp>

#define LOG_LUA_FUNCTION_REGISTRY 0

template <>
void Client::Hook::Hooks::HK_luaL_openlib::hkCallback(IW8::lua_State* L, const char* libname, const IW8::luaL_Reg* l, std::uint32_t nup) {
	if (libname != nullptr) {
#		if LOG_LUA_FUNCTION_REGISTRY
			Common::Utility::NT::Library gameLib = Common::Utility::NT::Library();
			std::size_t gameBase = reinterpret_cast<std::size_t>(gameLib.GetPtr());
#		endif
		for (IW8::luaL_Reg* f = const_cast<IW8::luaL_Reg*>(l); f; ++f) {
			if (f->m_Name == nullptr) {
				break;
			}
#			if LOG_LUA_FUNCTION_REGISTRY
				std::size_t funcAddr = reinterpret_cast<std::size_t>(f->m_Func);
				LOG("Game/luaL_openlib", DEBUG, "Registering Lua function \"{}.{}\" at 0x{:016X} ({}+0x{:016X})", libname, f->m_Name, funcAddr,
					gameLib.GetName(), funcAddr - gameBase);
#			endif

			std::string fullName = std::format("{}.{}", libname, f->m_Name);
			HookPlate::LuaHookStore::HookData* hook = g_Hooks->m_LuaHookStore.FindHook(fullName.c_str());
			if (hook != nullptr) {
				hook->Hook(f);
			}
		}
	}

	return m_Original(L, libname, l, nup);
}
