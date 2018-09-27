#include "common/runtime.h"

#include "objects/file/file.h"

#include "common/drawable.h"
#include "objects/image/image.h"
#include "modules/window.h"

#include "warn_png.h"
#include "plus_png.h"

#include "dog_png.h"
#include "head_png.h"
#include "dogshadow_png.h"
#include "shadow_png.h"
#include "tail_png.h"
#include "cloud_png.h"
#include "tongue_png.h"

Image::Image(const char * path, bool memory) : Drawable("Image")
{
    uint texture;
    u32 * buffer = nullptr;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    if (!memory)
        buffer = this->LoadPNG(path, nullptr, 0);
    else
        buffer = this->GetMemoryImage(path);

    if (buffer != nullptr)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
        glGenerateMipmap(GL_TEXTURE_2D);

        delete[] buffer;
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    this->textureHandle = texture;

    this->viewport = {0, 0, 1, 1, this->width, this->height};
}

char * Image::GetMemoryImage(const char * path)
{
    string name = path;
    name = name.substr(name.find(":") + 1);

    if (name == "dog")
        return this->LoadPNG(nullptr, dog_png, dog_png_size);
    else if (name == "head")
        return this->LoadPNG(nullptr, head_png, head_png_size);
    else if (name == "dogshadow")
        return this->LoadPNG(nullptr, dogshadow_png, dogshadow_png_size);
    else if (name == "shadow")
        return this->LoadPNG(nullptr, shadow_png, shadow_png_size);
    else if (name == "tail")
        return this->LoadPNG(nullptr, tail_png, tail_png_size);
    else if (name == "tongue")
        return this->LoadPNG(nullptr, tongue_png, tongue_png_size);
    else if (name == "cloud")
        return this->LoadPNG(nullptr, cloud_png, cloud_png_size);
    else if (name == "warn")
        return this->LoadPNG(nullptr, warn_png, warn_png_size);
    else
        return this->LoadPNG(nullptr, plus_png, plus_png_size);

    return nullptr;
}