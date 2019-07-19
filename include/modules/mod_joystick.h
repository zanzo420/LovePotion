#pragma once

namespace Joystick
{

    void Initialize(lua_State * L); //should set up joysticks

    #if defined(__SWITCH__)
        Gamepad * GetJoystickFromID(uint id);
        
        void AddJoystick(lua_State * L, uint idx);

        void RemoveJoystick(lua_State * L, uint idx);
    #endif

    //Löve2D Functions

    int GetJoysticks(lua_State * L);

    int GetJoystickCount(lua_State * L);

    //End Löve2D Functions

    int Register(lua_State * L);
}
