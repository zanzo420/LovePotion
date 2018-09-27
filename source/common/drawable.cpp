#include "common/runtime.h"
#include "common/drawable.h"
#include "modules/graphics.h"

Drawable::Drawable(char * type) : Object(type) { }

Drawable::~Drawable()
{
    if (this->textureHandle != 0)
    {
        glDeleteTextures(1, &this->textureHandle);
        this->textureHandle = 0;
    }

    this->width = 0;
    this->height = 0;
}

u32 * Drawable::LoadPNG(const char * path, char * buffer, size_t memorySize)
{
    u32 * out = nullptr;
    FILE * input;

    if (buffer == nullptr || memorySize == 0)
        input = fopen(path, "rb");
    else
        input = fmemopen(buffer, memorySize, "rb");

    if (!input)
        return nullptr;

    constexpr size_t sigLength = 8;
    u8 sig[sigLength] = {0};

    size_t size = fread(sig, sizeof(u8), sigLength, input);
    
    fseek(input, 0, SEEK_SET);

    if (size < sigLength || png_sig_cmp(sig, 0, sigLength))
    {
        fclose(input);
        return nullptr;
    }

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    png_infop info = png_create_info_struct(png);

    if (setjmp(png_jmpbuf(png)))
    {
        png_destroy_read_struct(&png, &info, NULL);
        fclose(input);
        return nullptr;
    }

    png_init_io(png, input);
    png_read_info(png, info);

    this->width = png_get_image_width(png, info);
    this->height = png_get_image_height(png, info);

    png_byte color_type = png_get_color_type(png, info);
    png_byte bit_depth  = png_get_bit_depth(png, info);

    // Read any color_type into 8bit depth, ABGR format.
    // See http://www.libpng.org/pub/png/libpng-manual.txt
    if(bit_depth == 16)
        png_set_strip_16(png);

    if(color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png);

    // PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
    if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        png_set_expand_gray_1_2_4_to_8(png);

    if(png_get_valid(png, info, PNG_INFO_tRNS) != 0)
        png_set_tRNS_to_alpha(png);

    // These color_type don't have an alpha channel then fill it with 0xff.
    if(color_type == PNG_COLOR_TYPE_RGB ||
        color_type == PNG_COLOR_TYPE_GRAY ||
        color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

    if(color_type == PNG_COLOR_TYPE_GRAY ||
        color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(png);

    png_read_update_info(png, info);

    png_bytep * row_pointers = new (std::nothrow) png_bytep[this->height];
    if (row_pointers == nullptr)
    {
        png_destroy_read_struct(&png, &info, NULL);
        return nullptr;
    }

    for (int y = 0; y < this->height; y++)
        row_pointers[y] = (png_byte *)malloc(png_get_rowbytes(png, info));

    png_read_image(png, row_pointers);

    fclose(input);
    png_destroy_read_struct(&png, &info, NULL);

    //out = (u32 *)linearAlloc(this->width * this->height);
    out = new (std::nothrow) u32[this->width * this->height];

    if (out == NULL)
    {
        for (int j = 0; j < this->height; j++)
            free(row_pointers[j]);

        delete[] row_pointers;
        return nullptr;
    }

    for(int j = this->height - 1; j >= 0; j--)
    {
        png_bytep row = row_pointers[j];
        for(int i = 0; i < this->width; i++)
        {
            png_bytep px = &(row[i * 4]);
            memcpy(&out[(((this->height - 1) - j) * this->width) + i], px, sizeof(u32));
        }
        free(row_pointers[j]); // free the completed row, to avoid having to iterate over the whole thing again
    }
    delete[] row_pointers;

    return out;
}

void Drawable::Draw(const Viewport & view, double x, double y, double rotation, double scalarX, double scalarY, Color color)
{
    GLint textureLoc = glGetUniformLocation(Graphics::GetShader(), "hasTexture");
    glUniform1i(textureLoc, GL_TRUE);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->textureHandle);
    
    /*
    float left   = viewport.x; //0
    float top    = viewport.y; //0

    float right  = viewport.width; //1
    float bottom = viewport.height; //1

    Graphics::AppendVertex(x, y, color, {left, bottom});
    Graphics::AppendVertex(x, y + height, color, {left, top});
    Graphics::AppendVertex(x + width, y + height, color, {right, top});

    Graphics::AppendVertex(x + width, y + height, color, {right, top});
    Graphics::AppendVertex(x + width, y, color, {right, bottom});
    Graphics::AppendVertex(x, y, color, {left, bottom});
    */

    Graphics::AppendVertex(x, y, color, {0, 1});
    Graphics::AppendVertex(x, y + height, color, {0, 0});
    Graphics::AppendVertex(x + width, y + height, color, {1, 0});

    Graphics::AppendVertex(x + width, y + height, color, {1, 0});
    Graphics::AppendVertex(x + width, y, color, {1, 1});
    Graphics::AppendVertex(x, y, color, {0, 1});

    Graphics::Flush();

    glUniform1i(textureLoc, GL_FALSE);
    glBindTexture(GL_TEXTURE_2D, 0);
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