#include "common/runtime.h"
#include "modules/event.h"

//Löve2D Functions

//love.event.quit
int LoveEvent::Quit(lua_State * L)
{
    Love::Quit(L);

    return 0;
}

//End Löve2D Functions

bool LoveEvent::IsTouchDown()
{
    return touchDown;
}

int LoveEvent::Register(lua_State * L)
{
    luaL_Reg reg[] =
    {
        { "poll", Poll },
        { "pump", Pump },
        { "quit", Quit },
        { 0, 0 }
    };

    love_register_module(L, "love.event", reg);

    return 1;
}
