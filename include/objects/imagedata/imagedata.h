#pragma once

class ImageData
{
    public:
        ImageData(const char * path);
        ImageData(char * buffer);
        ImageData(int width, int height);
        ImageData(int width, int height, char * buffer);

        Color GetPixel(int x, int y);
};