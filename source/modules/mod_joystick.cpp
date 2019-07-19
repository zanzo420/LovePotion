#include "common/runtime.h"

#include "objects/gamepad/gamepad.h"
#include "objects/gamepad/wrap_gamepad.h"
#include "modules/mod_joystick.h"


#if defined(__SWITCH__)
    std::vector<Gamepad *> controllers;
#elif defined(_3DS)
    Gamepad * controller3DS;
#endif


void Joystick::Initialize(lua_State * L)
{
    #if defined (__SWITCH__)
        SDL_InitSubSystem(SDL_INIT_JOYSTICK);
    #elif defined (_3DS)
        gamepadNew(L);
    #endif
}

#if defined(__SWITCH__)
Gamepad * Joystick::GetJoystickFromID(uint id)
{
    for (auto & controller : controllers)
    {
        if (controller->GetID() == id)
            return controller;
    }

    return nullptr;
}

void Joystick::AddJoystick(lua_State * L, uint idx)
{
    Gamepad * newjoystick = gamepadNew(L, idx);
    controllers.insert(controllers.begin() + idx, newjoystick);
}

void Joystick::RemoveJoystick(lua_State * L, uint idx)
{
    delete controllers[idx];
    controllers.erase(controllers.begin() + idx);
}
#endif

//Löve2D Functions

//Super hacky workaround for now..
//love.joystick.getJoysticks
int Joystick::GetJoysticks(lua_State * L)
{
    lua_newtable(L);

    #if defined(__SWITCH__)
        for (uint i = 0; i < controllers.size(); i++)
        {
            lua_pushnumber(L, i + 1);
            love_push_userdata(L, controllers[i]);

            lua_settable(L, -3);
        }
    #elif defined(_3DS)
        love_push_userdata(L, controller3DS);

        lua_settable(L, -3);
    #endif

    return 1;
}


//love.joystick.getJoystickCount
int Joystick::GetJoystickCount(lua_State * L)
{
    #if defined(__SWITCH__)
        lua_pushnumber(L, controllers.size());
    #elif defined(_3DS)
        lua_pushnumber(L, 1);
    #endif

    return 1;
}

//End Löve2D Functions

int Joystick::Register(lua_State * L)
{
    luaL_Reg reg[] = 
    {
        { "getJoystickCount", GetJoystickCount },
        { "getJoysticks",     GetJoysticks     },
        { 0, 0 },
    };

    luaL_newlib(L, reg);
    
    return 1;
}
