#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <fstream>
#include <SDL.h>


struct Bitmap {
    char FILEID[6];
    std::uint32_t fileVersion;
    char bitmapID[40];
    std::uint32_t width;
    std::uint32_t height;
    std::uint32_t crc;
    std::vector<std::uint32_t> pixels;

    SDL_Surface* CreateSurface();
};


Bitmap* LoadBitmap(const std::string& filepath);