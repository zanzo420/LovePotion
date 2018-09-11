#include "common/runtime.h"
#include "common/drawable.h"
#include "modules/window.h"

Drawable::Drawable(char * type) : Object(type)
{
    this->texture = NULL;
    this->surface = NULL;

    this->horScale = 0.0;
}

void Drawable::Draw(const Viewport & view, double x, double y, double rotation, double scalarX, double scalarY, Color color)
{
    SDL_Rect quad = {view.x, view.y, view.subWidth * scalarX, view.subHeight * scalarY};
    SDL_Rect position = {x, y, view.subWidth * scalarX, view.subHeight * scalarY};

    if (this->texture != NULL)
    {
        SDL_RendererFlip flip = SDL_FLIP_NONE;
        if (scalarX < 0.0)
            flip = SDL_FLIP_HORIZONTAL;

        /*SDL_SetTextureColorMod(this->texture, color.r, color.g, color.b);
        SDL_SetTextureAlphaMod(this->texture, color.a);

        SDL_RenderCopyEx(Window::GetRenderer(), this->texture, &quad, &position, rotation, NULL, flip);*/
    }
}

Viewport Drawable::GetViewport()
{
    return this->viewport;
}

int Drawable::GetWidth()
{
    return this->width;
}

int Drawable::GetHeight()
{
    return this->height;
}

Drawable::~Drawable()
{
    //SDL_DestroyTexture(this->texture);
}