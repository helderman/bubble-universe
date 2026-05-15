// bu.c - Ruud Helderman, May 2026 - MIT License
// Based on "Bubble Universe", written in Processing by yuruyurau
// https://x.com/yuruyurau/status/1226846058728177665

#ifdef _WIN32
#include <SDL.h>   // SDL2 releases for Windows do not come with SDL2 subfolder (SDL3 releases do)
#undef main        // prevent linker error - https://stackoverflow.com/a/6847847/2485966
#else
#include <SDL2/SDL.h>    // every sane OS
#endif

static float flabs(float a)    // so annoying fabs is double-only
{
    return a < 0 ? -a : a;
}

static void DrawPixel(SDL_Renderer *renderer, int red, int green, int blue, float x, float y, int xi, int yi)
{
    float a = (1.0f - flabs(x - (float)xi - 0.5f)) * (1.0f - flabs(y - (float)yi - 0.5f));
    SDL_SetRenderDrawColor(renderer, red, green, blue, (int)(a * 255));
    SDL_RenderDrawPoint(renderer, xi, yi);
}

static void DrawPoint(SDL_Renderer *renderer, int red, int green, int blue, float x, float y)
{
    int xi = (int)round(x);
    int yi = (int)round(y);
    DrawPixel(renderer, red, green, blue, x, y, xi  , yi);
    DrawPixel(renderer, red, green, blue, x, y, xi-1, yi);
    DrawPixel(renderer, red, green, blue, x, y, xi  , yi-1);
    DrawPixel(renderer, red, green, blue, x, y, xi-1, yi-1);
}

static void DrawFrame(SDL_Window *window, SDL_Renderer *renderer, float time)
{
    int width, height;
    float size, x0, y0;

    SDL_GetWindowSize(window, &width, &height);
    size = (width < height ? width : height) * 0.24f;
    x0 = 0.5f * (float)width;
    y0 = 0.5f * (float)height;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);     // black
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    for (int i = 0; i < 256; i++)
    {
        float x = 0, y = 0;
        for (int j = 0; j < 256; j++)
        {
            float a = x + i;
            float b = y + i * 0.031f - time;
            x = SDL_cosf(a) + SDL_cosf(b);
            y = SDL_sinf(a) + SDL_sinf(b);
            DrawPoint(renderer, i, j, 99, x0 - x * size, y0 - y * size);
        }
    }
    SDL_RenderPresent(renderer);      // show everything (double buffering)
}

static SDL_bool EventLoop(void)
{
    SDL_Event event;
    SDL_bool running = SDL_TRUE;

    while (SDL_PollEvent(&event))     // consume all pending events
    {
        switch (event.type)
        {
            case SDL_QUIT:            // triggered on window close
                running = SDL_FALSE;
                break;
        }
    }
    return running;
}

int main(int argc, char *argv[])
{
    int exitCode = 1;
    (void)argc, (void)argv;     // suppress compiler warning
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        SDL_Log("SDL_Init Error: %s\n", SDL_GetError());
    }
    else
    {
        SDL_Window *window = SDL_CreateWindow("Bubble Universe"
            , SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 400, 400, SDL_WINDOW_RESIZABLE);
        if (!window)
        {
            SDL_Log("SDL_CreateWindow Error: %s\n", SDL_GetError());
        }
        else
        {
            SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
            if (!renderer)
            {
                SDL_Log("SDL_CreateRenderer Error: %s\n", SDL_GetError());
            }
            else
            {
                exitCode = 0;           // initialization was successful
                while (EventLoop())
                {
                    DrawFrame(window, renderer, SDL_GetTicks() * 0.0001f);
                }
                SDL_DestroyRenderer(renderer);
            }
            SDL_DestroyWindow(window);
        }
    }
    SDL_Quit();
    return exitCode;
}
