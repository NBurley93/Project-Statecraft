#include "GameEngine.hh"
#include <tinyxml2.h>
#include <pch.hh>
#include <Bitmap.hh>

using namespace tinyxml2;

GameEngine* GameEngine::CreateEngine(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        throw new std::exception("SDL failed to initialize");
    }

    SDL_version v;
    SDL_GetVersion(&v);
    GAME_INFO("SDL {}.{}.{}", v.major, v.minor, v.patch);

    // Load config
    GAME_INFO("Loading config.xml...");
    XMLDocument doc;
    doc.LoadFile("game/config.xml");

    int width, height;
    bool fullscreen, vsync;

    auto windowCfg = doc.FirstChildElement("window");
    auto cfgWndWidth = windowCfg->FirstChildElement("width");
    auto cfgWndHeight = windowCfg->FirstChildElement("height");
    auto cfgWndFulls = windowCfg->FirstChildElement("fullscreen");
    auto cfgWndVsync = windowCfg->FirstChildElement("vsync");

    cfgWndWidth->QueryIntText(&width);
    cfgWndHeight->QueryIntText(&height);
    cfgWndFulls->QueryBoolText(&fullscreen);
    cfgWndVsync->QueryBoolText(&vsync);

    GAME_INFO("Creating engine...");
    GameEngine *engine = new GameEngine();
    GAME_INFO("Creating Window and Renderer with params.");
    GAME_INFO("Width: {}, Height: {}, Fullscreen: {}, VSync: {}", width, height, fullscreen, vsync);
    engine->mWindow = new Window(Window::WindowProperties(width, height, fullscreen, vsync));
    return engine;
}


GameEngine::~GameEngine() {
    delete tex;
    delete mWindow;
    SDL_Quit();
}


void debug_save_texture(SDL_Renderer* ren, SDL_Texture* tex, const char* filename) {
    SDL_Texture *ren_tex;
    SDL_Surface *surf;
    int st;
    int w;
    int h;
    int format;
    void *pixels;

    pixels  = NULL;
    surf    = NULL;
    ren_tex = NULL;
    format  = SDL_PIXELFORMAT_RGBA32;

    /* Get information about texture we want to save */
    st = SDL_QueryTexture(tex, NULL, NULL, &w, &h);
    if (st != 0) {
        SDL_Log("Failed querying texture: %s\n", SDL_GetError());
        goto cleanup;
    }

    ren_tex = SDL_CreateTexture(ren, format, SDL_TEXTUREACCESS_TARGET, w, h);
    if (!ren_tex) {
        SDL_Log("Failed creating render texture: %s\n", SDL_GetError());
        goto cleanup;
    }

    /*
     * Initialize our canvas, then copy texture to a target whose pixel data we 
     * can access
     */
    st = SDL_SetRenderTarget(ren, ren_tex);
    if (st != 0) {
        SDL_Log("Failed setting render target: %s\n", SDL_GetError());
        goto cleanup;
    }

    SDL_SetRenderDrawColor(ren, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(ren);

    st = SDL_RenderCopy(ren, tex, NULL, NULL);
    if (st != 0) {
        SDL_Log("Failed copying texture data: %s\n", SDL_GetError());
        goto cleanup;
    }

    /* Create buffer to hold texture data and load it */
    pixels = malloc(w * h * SDL_BYTESPERPIXEL(format));
    if (!pixels) {
        SDL_Log("Failed allocating memory\n");
        goto cleanup;
    }

    st = SDL_RenderReadPixels(ren, NULL, format, pixels, w * SDL_BYTESPERPIXEL(format));
    if (st != 0) {
        SDL_Log("Failed reading pixel data: %s\n", SDL_GetError());
        goto cleanup;
    }

    /* Copy pixel data over to surface */
    surf = SDL_CreateRGBSurfaceWithFormatFrom(pixels, w, h, SDL_BITSPERPIXEL(format), w * SDL_BYTESPERPIXEL(format), format);
    if (!surf) {
        SDL_Log("Failed creating new surface: %s\n", SDL_GetError());
        goto cleanup;
    }

    /* Save result to an image */
    st = SDL_SaveBMP(surf, filename);
    if (st != 0) {
        SDL_Log("Failed saving image: %s\n", SDL_GetError());
        goto cleanup;
    }

    SDL_Log("Saved texture as BMP to \"%s\"\n", filename);

cleanup:
    SDL_FreeSurface(surf);
    free(pixels);
    SDL_DestroyTexture(ren_tex);
}


bool GameEngine::Init() {
    if (mWindow->IsInitialized()) {
        tex = new Texture();
        //tex->LoadImage(*mWindow->GetContext(), "game/textures/test.png");
        Bitmap* bm = LoadBitmap("game/textures/test.bitmap");
        SDL_Surface* bmSurface = bm->CreateSurface();
        if (bmSurface) {
            tex->LoadFromSurface(*mWindow->GetContext(), bmSurface);
            debug_save_texture(mWindow->GetContext(), tex->GetID(), "test.bmp");
        } else {
            return false;
        }
        SDL_FreeSurface(bmSurface);
        delete bm;

        mRunning = true;
        return true;
    } else {
        return false;
    }
}


void GameEngine::Run() {
    SDL_SetRenderDrawBlendMode(mWindow->GetContext(), SDL_BLENDMODE_BLEND);

    while (mRunning) {
        SDL_Event e;
        while(SDL_PollEvent(&e)) {
            switch(e.type) {
                case SDL_QUIT:
                mRunning = false;
                break;
            }
        }

        SDL_SetRenderDrawColor(mWindow->GetContext(), 255, 0, 255, 255);
        SDL_RenderClear(mWindow->GetContext());
        SDL_SetRenderDrawColor(mWindow->GetContext(), 255, 255, 255, 255);

        // Draw stuff
        SDL_Rect src;
        SDL_Rect dst;

        src.x = 0;
        src.y = 0;
        src.w = tex->Width();
        src.h = tex->Height();

        dst.x = 0;
        dst.y = 0;
        dst.w = tex->Width();
        dst.h = tex->Height();

        SDL_RenderCopy(
            mWindow->GetContext(),
            tex->GetID(),
            &src,
            &dst
        );

        SDL_RenderPresent(mWindow->GetContext());
    }
}

