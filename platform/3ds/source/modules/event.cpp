#include "common/runtime.h"

#include "objects/gamepad/gamepad.h"
#include "modules/joystick.h"
#include "modules/event.h"

int lastTouch[2];

int LoveEvent::Pump(lua_State * L)
{
    hidScanInput();

    //love.gamepadpressed
    u32 buttonDown = hidKeysDown();
    Gamepad * controller = Joystick::GetJoystickFromID(0);
    string downKey = controller->GetInput(buttonDown);

    touchPosition touch;
    hidTouchRead(&touch);

    if (downKey != "nil")
    {
        if (downKey != "touch")
        {
            love_getfield(L, "gamepadpressed");
            if (!lua_isnil(L, -1))
            {
                love_push_userdata(L, controller);
                lua_pushstring(L, downKey.c_str());

                lua_call(L, 2, 0);
            }
        }
        else
        {
            love_getfield(L, "touchpressed");
            if (!lua_isnil(L, -1))
            {
                lua_pushlightuserdata(L, (void *)1);
                lua_pushnumber(L, touch.px + 1);
                lua_pushnumber(L, touch.py + 1);
                lua_pushinteger(L, 0);
                lua_pushinteger(L, 0);
                lua_pushinteger(L, 1);

                lua_call(L, 6, 0);

            }
            touchDown = true;
        }
    }

    u32 buttonHeld = hidKeysHeld();
    string heldKey = controller->GetInput(buttonHeld);

    if (heldKey != "nil" && heldKey == "touch")
    {
        lastTouch[0] = touch.px + 1;
        lastTouch[1] = touch.py + 1;

        love_getfield(L, "touchmoved");
        if (!lua_isnil(L, -1))
        {
            lua_pushlightuserdata(L, (void *)1);
            lua_pushnumber(L, lastTouch[0] + 1);
            lua_pushnumber(L, lastTouch[1] + 1);
            lua_pushinteger(L, 0);
            lua_pushinteger(L, 0);
            lua_pushinteger(L, 1);

            lua_call(L, 6, 0);
        }
    }

    //love.gamepadreleased
    u32 buttonUp = hidKeysUp();
    string upKey = controller->GetInput(buttonUp);

    if (upKey != "nil")
    {
        if (upKey != "touch")
        {
            love_getfield(L, "gamepadreleased");
            if (!lua_isnil(L, -1))
            {
                love_push_userdata(L, controller);
                lua_pushstring(L, upKey.c_str());

                lua_call(L, 2, 0);
            }
        }
        else
        {
            love_getfield(L, "touchreleased");
            if (!lua_isnil(L, -1))
            {
                lua_pushlightuserdata(L, (void *)1);
                lua_pushnumber(L, lastTouch[0]);
                lua_pushnumber(L, lastTouch[1]);
                lua_pushinteger(L, 0);
                lua_pushinteger(L, 0);
                lua_pushinteger(L, 1);

                lua_call(L, 6, 0);
            }
            touchDown = false;
        }
    }

    for (uint i = 0; i < 4; i++)
    {
        love_getfield(L, "gamepadaxis");
        if (!lua_isnil(L, -1))
        {
            love_push_userdata(L, controller);
            lua_pushstring(L, GAMEPAD_AXES[i].c_str());
            lua_pushnumber(L, controller->GetAxis(i + 1));

            lua_call(L, 3, 0);
        }
    }

    for (uint i = 4; i < 11; i++)
    {
        love_getfield(L, "joystickaxis");
        if (!lua_isnil(L, -1))
        {
            love_push_userdata(L, controller);
            lua_pushinteger(L, i + 1);
            lua_pushnumber(L, controller->GetAxis(i + 1));

            lua_call(L, 3, 0);
        }
    }

    return 0;
}

int LoveEvent::Poll(lua_State * L)
{
    return 0;
}
