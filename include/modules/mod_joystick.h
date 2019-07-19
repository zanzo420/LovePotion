/*
** modules/joystick.h
** @brief    : Handles (basic) joystick stuff.
** @ref        : See wrap_gamepad.cpp for advanced use.
*/

#pragma once

namespace Joystick
{
    // std::array<Gamepad *, 8> controllers;

    void Initialize(lua_State * L); //should set up joysticks

    Gamepad * GetJoystickFromID(uint id);
    
    void RemoveJoystick(uint id);

    //Löve2D Functions

    int GetJoysticks(lua_State * L);

    int GetJoystickCount(lua_State * L);

    int SetLayout(lua_State * L);

    //End Löve2D Functions

    int Register(lua_State * L);
}

extern std::vector<Gamepad *> controllers;
