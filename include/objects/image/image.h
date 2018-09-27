#pragma once

class Image : public Drawable
{
    public:
        Image() {};
        Image(const char * path, bool memory);
        ~Image();

    private:
        char * GetMemoryImage(const char * path);
};
