#pragma once

class Image : public Drawable
{
    public:
        Image() {};
        Image(const char * path, bool memory);
        ~Image();

    private:
        uint GetMemoryImage(const char * path);
};
