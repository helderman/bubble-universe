// bu.c - Ruud Helderman, 2026-05-09 - MIT License
// Based on "Bubble Universe", written in Processing by yuruyurau
// https://x.com/yuruyurau/status/1226846058728177665

#include <SDL2/SDL.h>

static void DrawFrame(SDL_Window *window, SDL_Renderer *renderer, float time)
{
    float x = 0;
    float y = 0;

    int width, height, size;
    SDL_GetWindowSize(window, &width, &height);
    size = (width < height ? width : height) * 0.24f;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);     // black
    SDL_RenderClear(renderer);

    for (int i = 0; i < 255; i++)
    {
        for (int j = 0; j < 255; j++)
        {
            float a = x + i;
            float b = y + i * 0.031 + time;
            x = SDL_cosf(a) + SDL_cosf(b);
            y = SDL_sinf(a) + SDL_sinf(b);
            SDL_SetRenderDrawColor(renderer, i, j, 99, 255);
            SDL_RenderDrawPoint(renderer
                , x * size + 0.5f * width
                , y * size + 0.5f * height);
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

int main()
{
    int exitCode = 1;
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        SDL_Log("SDL_Init Error: %s\n", SDL_GetError());
    }
    else
    {
        SDL_Window *window = SDL_CreateWindow("Bubble Universe"
            , SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 400, 400, SDL_WINDOW_SHOWN);
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
                    DrawFrame(window, renderer, SDL_GetTicks() * 0.0002);
                }
                SDL_DestroyRenderer(renderer);
            }
            SDL_DestroyWindow(window);
        }
    }
    SDL_Quit();
    return exitCode;
}
