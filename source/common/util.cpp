extern "C" 
{
    #include <lua.h>
    #include <lualib.h>
    #include <lauxlib.h>

    #include <compat-5.3.h>
    #include <luaobj.h>
}

#include <map>
#include <vector>
#include <stdarg.h>

#if defined (_3DS)
    #include <3ds.h>
#elif defined (__SWITCH__)
    #include <switch.h>
#endif

#include "common/types.h"
#include "objects/object/object.h"
#include "common/util.h"

void love_getfield(lua_State * L, const char * field)
{
    lua_getfield(L, LUA_GLOBALSINDEX, "love");
    lua_getfield(L, -1, field);
    lua_remove(L, -2);
}

int love_preload(lua_State * L, lua_CFunction function, const char * name)
{
    lua_getglobal(L, "package");
    lua_getfield(L, -1, "preload");
    lua_pushcfunction(L, function);
    lua_setfield(L, -2, name);
    lua_pop(L, 2);

    return 0;
}

void love_register(lua_State * L, int index, void * object)
{
    love_get_registry(L, OBJECTS);

    lua_pushlightuserdata(L, object);   //light userdata key
    lua_pushvalue(L, index);            //push the userdata value to the key
    lua_settable(L, -3);                //set the taaable (╯°□°）╯︵ ┻━┻

    lua_setfield(L, LUA_REGISTRYINDEX, "_loveobjects");
}

void love_unregister(lua_State * L, void * object)
{
    love_get_registry(L, OBJECTS);

    lua_pushlightuserdata(L, object); // light userdata key
    lua_pushnil(L);                // push nil to the value
    lua_settable(L, -3);           // set the table back to normal

    lua_setfield(L, LUA_REGISTRYINDEX, "_loveobjects");
}

void love_push_userdata(lua_State * L, void * object)
{
    if (!object)
        return;

    love_get_registry(L, OBJECTS);
    lua_pushlightuserdata(L, object);
    lua_gettable(L, -2);
    lua_remove(L, -2);
}

bool love_is_registered(lua_State * L, void * object)
{
    bool valid = true;

    if (!object)
        return false;

    love_get_registry(L, OBJECTS);
    lua_pushlightuserdata(L, object);
    lua_gettable(L, -2);
    lua_remove(L, -2);

    if (lua_isnil(L, 2))
        valid = false;

    lua_pop(L, 1);

    return valid;
}

int love_get_registry(lua_State * L, REGISTRY registry)
{
    switch(registry)
    {
        case OBJECTS:
            lua_getfield(L, LUA_REGISTRYINDEX, "_loveobjects");
            return 1;
        default:
            return luaL_error(L, "Attempted to use invalid registry."); 
    }
}

int luax_traceback(lua_State *L)
{
    if (!lua_isstring(L, 1))  // 'message' not a string?
        return 1; // keep it intact

    lua_getglobal(L, "debug");
    if (!lua_istable(L, -1))
    {
        lua_pop(L, 1);
        return 1;
    }

    lua_getfield(L, -1, "traceback");
    if (!lua_isfunction(L, -1))
    {
        lua_pop(L, 2);
        return 1;
    }

    lua_pushvalue(L, 1); // pass error message
    lua_pushinteger(L, 2); // skip this function and traceback
    lua_call(L, 2, 1); // call debug.traceback

    return 1;
}

double clamp(double low, double value, double high)
{
    return std::min(high, std::max(low, value));
}

int NextPO2(unsigned int in)
{
    in--;
    in |= in >> 1;
    in |= in >> 2;
    in |= in >> 4;
    in |= in >> 8;
    in |= in >> 16;
    in++;

    return std::min(std::max(in, 8U), 1024U); // clamp size to keep gpu from locking
}