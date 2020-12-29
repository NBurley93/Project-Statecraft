#include <SDL.h>

class Window {
public:
    struct WindowProperties {
        uint32_t Width, Height;
        bool Fullscreen, VSync;

        WindowProperties(uint32_t width, uint32_t height, bool fullscreen = false, bool vsync = false) : Width(width), Height(height), Fullscreen(fullscreen), VSync(vsync) {}
    };

    Window(const WindowProperties& properties);
    ~Window();

    inline SDL_Renderer* GetContext() const { return mContext; }
    inline SDL_Window* GetNative() const { return mHandle; }

    inline bool IsFullscreen() const { return fullscreen; }
    inline bool IsVSync() const { return vsync; }

    inline uint32_t GetWidth() const { return width; }
    inline uint32_t GetHeight() const { return height; }

    inline bool IsInitialized() const { return initialized; }

private:
    SDL_Window* mHandle;
    SDL_Renderer* mContext;

    bool initialized = false;

    uint32_t width, height;
    bool fullscreen, vsync;
};