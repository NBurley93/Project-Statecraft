#include <pch.hh>
#include "Texture.hh"

#define STB_IMAGE_IMPLEMENTATION
#include "../Lib/stb_image.h"


bool Texture::LoadImage(SDL_Renderer& renderer, const std::string& filepath) {
    int width, height, bpp;
    unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &bpp, STBI_rgb_alpha);
    if (data == NULL) {
        GAME_ERROR("Failed to load image: {}", stbi_failure_reason());
        return false;
    }

    int depth, pitch;
    Uint32 pixel_format;
    if (bpp == STBI_rgb) {
        depth = 24;
        pitch = 3*width;
        pixel_format = SDL_PIXELFORMAT_RGB24;
    } else {
        depth = 32;
        pitch = 4*width;
        pixel_format = SDL_PIXELFORMAT_RGBA32;
    }

    SDL_Surface* surf = SDL_CreateRGBSurfaceWithFormatFrom(
        (void*)data,
        width,
        height,
        depth,
        pitch,
        pixel_format
    );
    
    if (surf == NULL) {
        GAME_ERROR("Failed to create SDL_Surface: {}", SDL_GetError());
        stbi_image_free(data);
        return false;
    }

    mWidth = width;
    mHeight = height;
    mID = SDL_CreateTextureFromSurface(&renderer, surf);
    if (mID == NULL) {
        GAME_ERROR("Failed to convert surface to texture: {}", SDL_GetError());
        SDL_FreeSurface(surf);
        stbi_image_free(data);
        return false;
    }

    GAME_INFO("Loaded image from {}", filepath);
    SDL_FreeSurface(surf);
    stbi_image_free(data);
    return true;
}

bool Texture::LoadFromSurface(SDL_Renderer& renderer, SDL_Surface* surface) {
    mWidth = surface->w;
    mHeight = surface->h;
    mID = SDL_CreateTextureFromSurface(&renderer, surface);
    if (mID == NULL) {
        GAME_ERROR("Failed to convert surface to texture: {}", SDL_GetError());
        return false;
    }

    GAME_INFO("Loaded image from surface");
    return true;
}


Texture::Texture() {}


Texture::~Texture() {
    if (mID != NULL) {
        SDL_DestroyTexture(mID);
    }
}
