#pragma once

#include <SDL.h>

class Texture {
public:
    Texture();
    ~Texture();

    bool LoadImage(SDL_Renderer& renderer, const std::string& filepath);

    inline const uint32_t Width() const { return mWidth; }
    inline const uint32_t Height() const { return mHeight; }
    inline SDL_Texture* GetID() const { return mID; }
private:
    SDL_Texture* mID;
    uint32_t mWidth, mHeight;
};