#include "common/runtime.h"

#include <unistd.h>
#include <stdarg.h>

#include "common/version.h"

#include "objects/gamepad/gamepad.h"
#include "objects/gamepad/wrap_gamepad.h"

#include "objects/channel/channel.h"

#include "modules/audio.h"
#include "modules/event.h"
#include "modules/filesystem.h"
#include "modules/graphics.h"
#include "modules/joystick.h"
#include "modules/keyboard.h"
#include "modules/mod_image.h"
#include "modules/mod_math.h"
#include "modules/mod_thread.h"
#include "modules/system.h"
#include "modules/timer.h"
#include "modules/touch.h"
#include "modules/window.h"
#include "modules/display.h"

#include "objects/canvas/wrap_canvas.h"
#include "objects/channel/wrap_channel.h"
#include "objects/file/wrap_file.h"
#include "objects/font/wrap_font.h"
#include "objects/image/wrap_image.h"
#include "objects/imagedata/wrap_imagedata.h"
#include "objects/quad/wrap_quad.h"
#include "objects/source/wrap_source.h"
#include "objects/text/wrap_text.h"
#include "objects/thread/wrap_thread.h"
#include "objects/randomgenerator/wrap_randomgenerator.h"

#include "nogame_lua.h"

love_modules modules[14] =
{
    { "love.audio",      Audio::Register,       Audio::Exit      },
    { "love.event",      LoveEvent::Register,   NULL             },
    { "love.filesystem", Filesystem::Register,  Filesystem::Exit },
    { "love.graphics",   Graphics::Register,    NULL             },
    { "love.image",      ImageModule::Register, NULL             },
    { "love.joystick",   Joystick::Register,    NULL             },
    { "love.keyboard",   Keyboard::Register,    NULL             },
    { "love.math",       Math::Register,        NULL             },
    { "love.system",     System::Register,      System::Exit     },
    { "love.thread",     LoveThread::Register,  LoveThread::Exit },
    { "love.timer",      Timer::Register,       NULL             },
    { "love.touch",      Touch::Register,       NULL             },
    { "love.window",     Window::Register,      NULL             },
    { 0 }
};

int (*classes[])(lua_State *L) =
{
    initCanvasClass,
    initChannelClass,
    initFileClass,
    initFontClass,
    initGamepadClass,
    initImageDataClass,
    initImageClass,
    initQuadClass,
    initSourceClass,
    initLuaThread,
    initTextClass,
    initRandomGeneratorClass,
    NULL
};

//love.run
int Love::Run(lua_State * L)
{
    LoveEvent::Pump(L);

    luaL_dostring(L, LOVE_TIMER_STEP);

    if (luaL_dostring(L, LOVE_UPDATE))
        luaL_error(L, "%s", lua_tostring(L, -1));

    Graphics::Clear(L);

    if (luaL_dostring(L, LOVE_DRAW))
        luaL_error(L, "%s", lua_tostring(L, -1));

    Graphics::Present(L);

    Timer::Tick();

    //run for at least one frame then check
    if (appletGetAppletType() != AppletType_Application)
        luaL_error(L, "%s", "Please run Löve Potion under Atmosphère title takeover.");

    return 0;
}
