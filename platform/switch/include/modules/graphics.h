/*
** modules/graphics.h
** @brief : Used for rendering graphics (primitives, images, text)
*/

#pragma once

class Graphics
{
    public:
        Graphics() = delete;

        static void Initialize();

        static int Clear(lua_State * L);
        static int Present(lua_State * L);

        static int Register(lua_State * L);

        static void Exit();

    private:
        static inline SDL_Color backgroundColor = { 0, 0, 0, 255 };
        static inline SDL_Color drawColor = { 255, 255, 255, 255 };

        static inline float lineWidth = 2.0;

        //Löve2D Functions

        static int GetDimensions(lua_State * L);
        static int GetWidth(lua_State * L);
        static int GetHeight(lua_State * L);

        static int Draw(lua_State * L);

        static int SetColor(lua_State * L);
        static int GetColor(lua_State * L);

        static int SetBackgroundColor(lua_State * L);
        static int GetBackgroundColor(lua_State * L);

        static int SetLineWidth(lua_State * L);
        static int GetLineWidth(lua_State * L);

        static int Push(lua_State * L);
        static int Pop(lua_State * L);
        static int Translate(lua_State * L);
        static int Scale(lua_State * L);
        static int Rotate(lua_State * L);
        static int Shear(lua_State * L);
        static int Origin(lua_State * L);

        static int SetCanvas(lua_State * L);

        static int Rectangle(lua_State * L);
        static int Polygon(lua_State * L);
        static int Line(lua_State * L);
        static int Circle(lua_State * L);
        static int Arc(lua_State * L);
        static int Points(lua_State * L);

        static int SetScissor(lua_State * L);

        static int GetRendererInfo(lua_State * L);
        static int SetDefaultFilter(lua_State * L);

        static int GetFont(lua_State * L);
        static int SetFont(lua_State * L);
        static int SetNewFont(lua_State * L);

        static int Printf(lua_State * L);
        static int Print(lua_State * L);

        //End Löve2D Functions
};
