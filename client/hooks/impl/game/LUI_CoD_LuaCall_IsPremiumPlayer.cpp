#include "common.hpp"
#include "game/game.hpp"
#include "hooks/hook.hpp"

template <>
std::int64_t Client::Hook::Hooks::HK_LUI_CoD_LuaCall_IsPremiumPlayer::hkCallback(IW8::lua_State* luaVM) {
	g_Pointers->m_lua_pushboolean(luaVM, TRUE);
	return 1;
}
