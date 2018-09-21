class Drawable : public Object
{
    public:
        Drawable(char * type);
        Drawable() {};
        void Draw(const Viewport & viewport, double x, double y, double rotation, double scalarX, double scalarY, Color color);
        ~Drawable();

        int GetWidth();
        int GetHeight();

        Viewport GetViewport();

    protected:
        int width;
        int height;

        uint textureHandle;
        Viewport viewport;
        u32 * LoadPNG(const char * path, char * buffer, size_t memorySize);
};