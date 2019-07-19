#include "common/runtime.h"
#include "modules/event.h"

#include "objects/gamepad/gamepad.h"
#include "modules/mod_joystick.h"
#include "modules/timer.h"

SDL_Event event;
queue<map<string, vector<Variant>>> poll_queue;

int LoveEvent::Pump(lua_State * L)
{
    //update gamepad rumble
    for (Gamepad * joycon : controllers)
        joycon->Update(Timer::GetDelta());

    while (SDL_PollEvent(&event))
    {
        map<string, vector<Variant>> args;
        
        switch (event.type)
        {
            case SDL_JOYBUTTONDOWN:
            case SDL_JOYBUTTONUP:
            {
                Gamepad * controller = Joystick::GetJoystickFromID(event.jbutton.which);

                love_getfield(L, (event.type == SDL_JOYBUTTONDOWN) ? "joystickpressed" : "joystickreleased");

                if (!lua_isnil(L, -1))
                {
                    love_push_userdata(L, controller);
                    lua_pushnumber(L, event.jbutton.button + 1); // buttons are 1 indexed in LOVE

                    lua_call(L, 2, 0);
                }
                break;
            }
            case SDL_JOYAXISMOTION:
            {
                Gamepad * controller = Joystick::GetJoystickFromID(event.jaxis.which);

                love_getfield(L, "joystickaxis");

                if (!lua_isnil(L, -1))
                {
                    love_push_userdata(L, controller);

                    lua_pushnumber(L, event.jaxis.axis);

                    float value = (float)event.jaxis.value / JOYSTICK_MAX;
                    controller->ClampAxis(value);

                    lua_pushnumber(L, value);
                    lua_call(L, 3, 0);
                }
                break;
            }
            case SDL_JOYHATMOTION:
            {
                Gamepad * controller = Joystick::GetJoystickFromID(event.jaxis.which);

                love_getfield(L, "joystickhat");

                if (!lua_isnil(L, -1))
                {
                    love_push_userdata(L, controller);
                    lua_pushnumber(L, event.jhat.hat)
                    
                    string val;

                    switch (event.jhat.value)
                    {
                        case SDL_HAT_CENTERED:
                            val = "c";
                            break;
                        case SDL_HAT_DOWN:
                            val = "d";
                            break;
                        case SDL_HAT_LEFT:
                            val = "l";
                            break;
                        case SDL_HAT_LEFTDOWN:
                            val = "ld";
                            break;
                        case SDL_HAT_LEFTUP:
                            val = "lu";
                            break;
                        case SDL_HAT_RIGHT:
                            val = "r";
                            break;
                        case SDL_HAT_RIGHTDOWN:
                            val = "rd";
                            break;
                        case SDL_HAT_RIGHTUP:
                            val = "ru";
                            break;
                        case SDL_HAT_UP:
                            val = "u";
                            break;
                    }

                    lua_pushstring(L, val.c_str());

                    lua_call(L, 3, 0);
                }

                break;
            }
            case SDL_JOYDEVICEADDED:
            {
                // add joystick to list with jdevice.which
                //Joystick::Add(event.jdevice.which);
                // then grab that joystick out of the list and push it
                Gamepad * controller = Joystick::GetJoystickFromID(event.jdevice.which);

                love_getfield(L, "joystickadded");

                if (!lua_isnil(L, -1))
                {
                    love_push_userdata(L, controller);

                    lua_call(L, 1, 0);
                }
                break;
            }
            case SDL_JOYDEVICEREMOVED:
            {
                Gamepad * controller = Joystick::GetJoystickFromID(event.jdevice.which);

                love_getfield(L, "joystickremoved");

                if (!lua_isnil(L, -1))
                {
                    love_push_userdata(L, controller);

                    lua_call(L, 1, 0);
                }

                Joystick::RemoveJoystick(event.jdevice.which);

                break;
            }
            case SDL_CONTROLLERBUTTONDOWN:
            case SDL_CONTROLLERBUTTONUP:
            {
                Gamepad * controller = Joystick::GetJoystickFromID(event.jbutton.which);

                love_getfield(L, (event.type == SDL_CONTROLLERBUTTONDOWN) ? "gamepadpressed" : "gamepadreleased");

                if (!lua_isnil(L, -1))
                {
                    love_push_userdata(L, controller);
                    lua_pushstring(L, KEYS[event.cbutton.button].c_str());

                    lua_call(L, 2, 0);
                }
                break;
            }
            case SDL_CONTROLLERAXISMOTION:
            {
                Gamepad * controller = Joystick::GetJoystickFromID(event.jbutton.which);

                love_getfield(L, "gamepadaxis");

                if (!lua_isnil(L, -1))
                {
                    love_push_userdata(L, controller);
                    lua_pushstring(L, GAMEPAD_AXES[event.caxis.axis].c_str());
                    
                    float value = (float)event.caxis.value / JOYSTICK_MAX;
                    controller->ClampAxis(value);

                    lua_pushnumber(L, value);
                    lua_call(L, 3, 0);
                }
                break;
            }
            case SDL_FINGERDOWN:
            case SDL_FINGERUP:
            {
                love_getfield(L, (event.type == SDL_FINGERDOWN) ? "touchpressed" : "touchreleased");

                if (!lua_isnil(L, -1))
                {
                    lua_pushlightuserdata(L, (void *)event.tfinger.touchId);
                    lua_pushnumber(L, event.tfinger.x * 1280);
                    lua_pushnumber(L, event.tfinger.y * 720);
                    lua_pushnumber(L, 0);
                    lua_pushnumber(L, 0);
                    lua_pushnumber(L, event.tfinger.pressure);

                    lua_call(L, 6, 0);
                }
                break;
            }
            case SDL_FINGERMOTION:
            {
                love_getfield(L, "touchmoved");

                if (!lua_isnil(L, -1))
                {
                    lua_pushlightuserdata(L, (void *)event.tfinger.touchId);
                    lua_pushnumber(L, event.tfinger.x * 1280);
                    lua_pushnumber(L, event.tfinger.y * 720);
                    lua_pushnumber(L, event.tfinger.dx * 1280);
                    lua_pushnumber(L, event.tfinger.dy * 720);
                    lua_pushnumber(L, event.tfinger.pressure);

                    lua_call(L, 6, 0);
                }
                break;
            }
            default:
                break;
        }
    }

    return 0;
}

int LoveEvent::Poll(lua_State * L)
{
    if (poll_queue.empty())
    {
        lua_pushnil(L);

        return 1;
    }
    else
    {
        map<string, vector<Variant>> info = poll_queue.front();
        size_t size = 0;

        for (auto it = info.begin(); it != info.end(); ++it)
        {
            string event = it->first;
            size = it->second.size();

            lua_pushstring(L, event.c_str());
            
            for (uint index = 0; index < it->second.size(); index++)
            {
                Variant value = it->second[index];
                value.ToLua(L);
            }
        }

        //poll_queue.pop();

        return size + 1; //add event name
    }

    return 0;
}

//Löve2D Functions