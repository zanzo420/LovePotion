#pragma once

namespace Window
{
    void Initialize();

    EGLDisplay GetDisplay();

    EGLSurface GetSurface();

    int SetMode(lua_State * L);

    int Register(lua_State * L);

    void Exit();
}