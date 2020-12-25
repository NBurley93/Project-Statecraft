#include "Window.hh"


Window::Window(const WindowProperties& properties) {
    width = properties.Width;
    height = properties.Height;
    fullscreen = properties.Fullscreen;
    vsync = properties.VSync;

    Uint32 windowFlags = SDL_WINDOW_SHOWN;
    if (properties.Fullscreen) {
        windowFlags |= SDL_WINDOW_FULLSCREEN;
    }

    Uint32 rendererFlags = SDL_RENDERER_ACCELERATED;
    if (properties.VSync) {
        rendererFlags |= SDL_RENDERER_PRESENTVSYNC;
    }

    mHandle = SDL_CreateWindow("Project Statecraft", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, properties.Width, properties.Height, windowFlags);
    if (mHandle != nullptr) {
        mContext = SDL_CreateRenderer(mHandle, -1, rendererFlags);
    }

    if (mHandle != nullptr && mContext != nullptr) {
        initialized = true;
    }
}

Window::~Window() {
    if (mContext != nullptr) {
        SDL_DestroyRenderer(mContext);
    }
    if (mHandle != nullptr) {
        SDL_DestroyWindow(mHandle);
    }
    initialized = false;
}
