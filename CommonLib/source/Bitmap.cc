#include "../pch.hh"
#include "Bitmap.hh"
#include <zlib.h>


Bitmap* LoadBitmap(const std::string& filepath) {
    std::ifstream fs(filepath, std::ios::binary);
    Bitmap *bm = new Bitmap();
    

    if (!fs.fail()) {
        fs.read(&bm->FILEID[0], 6);
        fs.read(reinterpret_cast<char*>(&bm->fileVersion), 4);
        fs.read(reinterpret_cast<char*>(&bm->width), 4);
        fs.read(reinterpret_cast<char*>(&bm->height), 4);
        fs.read(reinterpret_cast<char*>(&bm->crc), 4);

        // Read zlib compressed pixel data
        auto cpos = fs.tellg();
        fs.seekg(0, fs.end);
        auto eof = fs.tellg();
        auto compressedSize = eof - cpos;
        fs.seekg(-compressedSize, fs.end);
        Bytef* compressedData = new Bytef[compressedSize];

        fs.read((char*)compressedData, compressedSize);

        std::uint32_t crc32_dst = 0U;

        uLongf uncompressedSize = (bm->width * bm->height) * sizeof(uint32_t);
        char* pixelData = new char[uncompressedSize];
        int success = uncompress((Bytef*)pixelData, &uncompressedSize, &compressedData[0], compressedSize);
        if (success != Z_OK) {
            switch (success) {
                case Z_MEM_ERROR:
                    GAME_WARN("ZLIB_MEM_ERROR");
                    break;
                case Z_BUF_ERROR:
                    GAME_WARN("ZLIB_BUF_ERROR");
                    break;
                case Z_DATA_ERROR:
                    GAME_WARN("ZLIB_DATA_ERROR");
                    break;
                default:
                    GAME_WARN("ZLIB_UNKNOWN_ERROR");
                    break;
            }
            delete[] compressedData;
        } else {
            crc32_dst = crc32(crc32_dst, (Bytef*)pixelData, (uInt)uncompressedSize);
            if (crc32_dst != bm->crc) {
                GAME_WARN("Checksum: MISMATCH");
                GAME_WARN("Ref Crc32: {}, Result Crc32: {}", bm->crc, crc32_dst);
            }
            delete[] compressedData;
            

            // Process decompressed pixel-data
            std::vector<unsigned char> buffer(pixelData, pixelData + uncompressedSize);
            for (size_t i = 0; i < buffer.size(); i += 4) {
                std::uint32_t px;
                memcpy(&px, &buffer[i], 4);
                bm->pixels.push_back(px);
            }

            delete[] pixelData;
        }

        GAME_INFO("Loaded bitmap data from {}", filepath);
        return bm;
    } else {
        delete bm;
        return nullptr;
    }
}

SDL_Surface* Bitmap::CreateSurface() {
    SDL_Surface *surf = SDL_CreateRGBSurfaceWithFormatFrom(
        (void*)this->pixels.data(),
        this->width,
        this->height,
        32,
        4 * this->width,
        SDL_PIXELFORMAT_RGBA8888);
    if (surf == NULL) {
        GAME_ERROR("Failed to convert bitmap to usable SDL_Surface! {}", SDL_GetError());
        return nullptr;
    } else {
        return surf;
    }
}
