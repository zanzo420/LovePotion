#include "common/runtime.h"
#include "modules/keyboard.h"

int Keyboard::Register(lua_State * L)
{
    luaL_Reg reg[] = 
    {
        { "showTextInput", ShowTextInput },
        { 0, 0 },
    };

    love_register_module(L, "love.keyboard", reg);
    
    return 1;
}
