#include "common.hpp"
#include "hooks/hook.hpp"

#include <utility/flags.hpp>
#include <utility/nt.hpp>

#define LOG_LUA_FUNCTION_REGISTRY FALSE

std::map<std::string, std::string> s_NamedLUIFunctions{
	{ "", "" }
};

void WriteToLuiDumpFile(const std::string& content) {
	static bool writingToFile = false;
	std::ofstream file("luidump.txt", writingToFile ? std::ios::app : std::ios::trunc);

	if (file.is_open()) {
		file << content;
		file.close();
	}
	writingToFile = true;
}

template <>
void Client::Hook::Hooks::HK_luaL_openlib::hkCallback(IW8::lua_State* L, const char* libname, const IW8::luaL_Reg* l, std::uint32_t nup) {
	static bool dumpLuiFuncs = Common::Utility::Flags::HasFlag("luidump");
	if (libname != nullptr) {
#		if LOG_LUA_FUNCTION_REGISTRY
			Common::Utility::NT::Library gameLib = Common::Utility::NT::Library();
			std::size_t gameBase = reinterpret_cast<std::size_t>(gameLib.GetPtr());
#		endif
		for (IW8::luaL_Reg* f = const_cast<IW8::luaL_Reg*>(l); f; ++f) {
			if (f->m_Name == nullptr) {
				break;
			}
			
			std::string fullName = std::format("{}.{}", libname, f->m_Name);
			std::size_t funcAddr = reinterpret_cast<std::size_t>(f->m_Func);

			if (dumpLuiFuncs) {
				WriteToLuiDumpFile(std::format("{} = {}\n", fullName, AndRel(funcAddr)));
			}

#			if LOG_LUA_FUNCTION_REGISTRY
				LOG("Game/luaL_openlib", DEBUG, "Registering Lua function \"{}.{}\" at 0x{:016X} ({}+0x{:016X})", libname, f->m_Name, funcAddr,
					gameLib.GetName(), funcAddr - gameBase);
#			endif

			HookPlate::LuaHookStore::HookData* hook = g_Hooks->m_LuaHookStore.FindHook(fullName.c_str());
			if (hook != nullptr) {
				hook->Hook(f);
			}
		}
	}

	return m_Original(L, libname, l, nup);
}
