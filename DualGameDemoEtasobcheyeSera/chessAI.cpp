#include <SDL2/SDL.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

// #include "global.h"
// #include "starter.h"
// #include "credit.h"
#include "chess.h"
#include "chessAI.h"
// #include "chess.h"
int WINDOW_WIDTH2 = 640;
int WINDOW_HEIGHT2 = 640;

SDL_Window *xwindow2 = NULL;
SDL_Renderer *xrenderer2 = NULL;
SDL_Texture *xtexturer2 = NULL;
char *x_font2 = "Monaco.ttf";

void start2(SDL_Renderer *xrenderer)
{
    /* Clear the screen with selected color */
    SDL_SetRenderDrawColor(xrenderer, 200, 55, 250, 255);
    SDL_RenderClear(xrenderer);
    char *workingButton = "SORRY,Still Working,press 1 to go back.";
    TTF_Font *monaco = TTF_OpenFont(x_font2, 24);
    if (monaco == NULL)
    {
        printf("Error opening font: %s\n", SDL_GetError());
        return;
    }
    SDL_Color blue = {0, 100, 255};
    SDL_Surface *workingSurface = TTF_RenderText_Blended(monaco, workingButton, blue);
    SDL_Texture *workingTexture = SDL_CreateTextureFromSurface(xrenderer, workingSurface);
    if (!workingTexture)
    {
        printf("error creating texture: %s\n", SDL_GetError());
        SDL_FreeSurface(workingSurface);
        TTF_CloseFont(monaco);
        return;
    }
    SDL_Rect working_rect;
    working_rect.x = (WINDOW_WIDTH2 - workingSurface->w) / 2;
    working_rect.y = (WINDOW_HEIGHT2 - workingSurface->h) / 2 + 5;
    working_rect.w = workingSurface->w;
    working_rect.h = workingSurface->h;

    SDL_RenderCopy(xrenderer, workingTexture, NULL, &working_rect);

    SDL_FreeSurface(workingSurface);
    SDL_DestroyTexture(workingTexture);
    TTF_CloseFont(monaco);
}
// initial
void initSDLs2()
{
    SDL_Init(SDL_INIT_VIDEO);
    xwindow2 = SDL_CreateWindow("CHESS", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
    xrenderer2 = SDL_CreateRenderer(xwindow2, -1, SDL_RENDERER_ACCELERATED);
    TTF_Init();
}
// chess game working
int NotDone()
{
    initSDLs2();

    int quit = 0;
    SDL_Event e;
    start2(xrenderer2);
    SDL_RenderPresent(xrenderer2);
    while (!quit)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quit = 1;
            }
            if (e.key.keysym.sym == SDLK_1)
            {
                // SDL_DestroyRenderer(xrenderer);
                // SDL_DestroyWindow(xwindow);
                SDL_Quit();
                startChess();
            }
            start2(xrenderer2);
            SDL_RenderPresent(xrenderer2);
        }
        start2(xrenderer2);
        SDL_RenderPresent(xrenderer2);
    }

    SDL_DestroyRenderer(xrenderer2);
    SDL_DestroyWindow(xwindow2);
    SDL_Quit();
    TTF_Quit();

    return 0;
}