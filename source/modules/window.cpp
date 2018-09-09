#include "common/runtime.h"
#include "modules/window.h"

static EGLDisplay DISPLAY;
static EGLContext CONTEXT;
static EGLSurface SURFACE;

void Window::Initialize()
{
    // Connect to default display
    DISPLAY = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    if (!DISPLAY)
        Love::RaiseError("Could not connect to display! error: %d", eglGetError());

    eglInitialize(DISPLAY, nullptr, nullptr);

    if (eglBindAPI(EGL_OPENGL_API) == EGL_FALSE)
        Love::RaiseError("Could not set API! error: %d", eglGetError());

    EGLConfig config;
    EGLint numConfigs;
    static const EGLint attributeList[] =
    {
        EGL_RED_SIZE, 1,
        EGL_GREEN_SIZE, 1,
        EGL_BLUE_SIZE, 1,
        EGL_NONE
    };

    eglChooseConfig(DISPLAY, attributeList, &config, 1, &numConfigs);

    if (numConfigs == 0)
        Love::RaiseError("No config found! error: %d", eglGetError());

    // Create surface
    SURFACE = eglCreateWindowSurface(DISPLAY, config, (char*)"", nullptr);
    if (!SURFACE)
        Love::RaiseError("Surface creation failed! error: %d", eglGetError());

    CONTEXT = eglCreateContext(DISPLAY, config, EGL_NO_CONTEXT, nullptr);
    if (!CONTEXT)
        Love::RaiseError("Context creation failed! error: %d", eglGetError());
    
    eglMakeCurrent(DISPLAY, SURFACE, SURFACE, CONTEXT);
}

EGLDisplay Window::GetDisplay()
{
    return DISPLAY;
}

EGLSurface Window::GetSurface()
{
    return SURFACE;
}

void Window::Exit()
{
    eglDestroyContext(DISPLAY, CONTEXT);
    CONTEXT = nullptr;

    eglDestroySurface(DISPLAY, SURFACE);
    SURFACE = nullptr;

    eglTerminate(DISPLAY);
    DISPLAY = nullptr;
}

//Löve2D Functions
int Window::SetMode(lua_State * L)
{
    return 0;
}

//End Löve2D Functions

int Window::Register(lua_State * L)
{
    luaL_Reg reg[] = 
    {
        { "setMode", SetMode },
        { 0, 0 }
    };

    luaL_newlib(L, reg);
    
    return 1;
}