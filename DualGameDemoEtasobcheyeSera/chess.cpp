#include <SDL2/SDL.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>


#include "chessmain.h"
// #include "global.h"
// #include "starter.h"
// #include "chess.h"
// #include "chessAI.h"
// #include "chess.h"
#include "chessAI.h"
// #include "chessmain.h"
#include "copyofMain.h"
int WINDOW_WIDTH3 = 640;
int WINDOW_HEIGHT3 = 640;

SDL_Window *xwindow3 = NULL;
SDL_Renderer *xrenderer3 = NULL;
SDL_Texture *xtexturer3 = NULL;
char *my_font3 = "Monaco.ttf";

void start(SDL_Renderer *xrenderer)
{
    /* Clear the screen with selected color */
    SDL_SetRenderDrawColor(xrenderer3, 55, 55, 100, 255);
    SDL_RenderClear(xrenderer3);
    char *singleButton = "Press 1 for Human vs Human";
    char *doubleButton = "Press 2 for Bot vs Human";
    char *menu = "Press 3 for Main menu";
    char *gameName = "Please select game type";
    TTF_Font *monaco = TTF_OpenFont(my_font3, 24);
    if (monaco == NULL)
    {
        printf("Error opening font: %s\n", SDL_GetError());
        return;
    }
    SDL_Color blue = {0, 100, 255};
    SDL_Surface *singleButtonSurface = TTF_RenderText_Blended(monaco, singleButton, blue);
    SDL_Texture *singleButtonTexture = SDL_CreateTextureFromSurface(xrenderer3, singleButtonSurface);
    if (!singleButtonTexture)
    {
        printf("error creating texture: %s\n", SDL_GetError());
        SDL_FreeSurface(singleButtonSurface);
        TTF_CloseFont(monaco);
        return;
    }

    SDL_Surface *doubleButtonSurface = TTF_RenderText_Blended(monaco, doubleButton, blue);
    SDL_Texture *doubleButtonTexture = SDL_CreateTextureFromSurface(xrenderer3, doubleButtonSurface);
    if (!doubleButtonTexture)
    {
        printf("error creating texture: %s\n", SDL_GetError());
        SDL_FreeSurface(doubleButtonSurface);
        TTF_CloseFont(monaco);
        return;
    }

    SDL_Surface *bgSurface = TTF_RenderText_Blended(monaco, menu, blue);
    SDL_Texture *bgTexture = SDL_CreateTextureFromSurface(xrenderer3, bgSurface);
    if (!bgTexture)
    {
        printf("error creating texture: %s\n", SDL_GetError());
        SDL_FreeSurface(bgSurface);
        TTF_CloseFont(monaco);
        return;
    }

    SDL_Surface *gameNameSurface = TTF_RenderText_Blended(monaco, gameName, blue);
    SDL_Texture *gameNameTexture = SDL_CreateTextureFromSurface(xrenderer3, gameNameSurface);
    if (!gameNameTexture)
    {
        printf("error creating texture: %s\n", SDL_GetError());
        SDL_FreeSurface(gameNameSurface);
        TTF_CloseFont(monaco);
        return;
    }

    SDL_Rect single_button_rect;
    single_button_rect.x = (WINDOW_WIDTH3 - singleButtonSurface->w) / 2;
    single_button_rect.y = (WINDOW_HEIGHT3 - singleButtonSurface->h) / 2 + 5;
    single_button_rect.w = singleButtonSurface->w;
    single_button_rect.h = singleButtonSurface->h;

    SDL_Rect double_button_rect;
    double_button_rect.x = (WINDOW_WIDTH3 - doubleButtonSurface->w) / 2;
    double_button_rect.y = (WINDOW_HEIGHT3 - doubleButtonSurface->h) / 2 + 50;
    double_button_rect.w = doubleButtonSurface->w;
    double_button_rect.h = doubleButtonSurface->h;

    SDL_Rect game_name_rect;
    game_name_rect.x = (WINDOW_WIDTH3 - gameNameSurface->w) / 2;
    game_name_rect.y = (WINDOW_HEIGHT3 - gameNameSurface->h) / 2 - 100;
    game_name_rect.w = gameNameSurface->w;
    game_name_rect.h = gameNameSurface->h;
    SDL_Rect bg_rect;
    bg_rect.x = (WINDOW_WIDTH3 - gameNameSurface->w) / 2;
    bg_rect.y = (WINDOW_HEIGHT3 - gameNameSurface->h) / 2 + 100;
    bg_rect.w = gameNameSurface->w;
    bg_rect.h = gameNameSurface->h;

    SDL_RenderCopy(xrenderer3, singleButtonTexture, NULL, &single_button_rect);
    SDL_RenderCopy(xrenderer3, doubleButtonTexture, NULL, &double_button_rect);
    SDL_RenderCopy(xrenderer3, gameNameTexture, NULL, &game_name_rect);
    SDL_RenderCopy(xrenderer3, bgTexture, NULL, &bg_rect);

    SDL_FreeSurface(singleButtonSurface);
    SDL_FreeSurface(doubleButtonSurface);
    SDL_FreeSurface(gameNameSurface);
    SDL_FreeSurface(bgSurface);
    SDL_DestroyTexture(singleButtonTexture);
    SDL_DestroyTexture(doubleButtonTexture);
    SDL_DestroyTexture(gameNameTexture);
    SDL_DestroyTexture(bgTexture);
    TTF_CloseFont(monaco);
}
// initial
void initSDLs()
{
    SDL_Init(SDL_INIT_VIDEO);
    xwindow3 = SDL_CreateWindow("CHESS", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
    xrenderer3 = SDL_CreateRenderer(xwindow3, -1, SDL_RENDERER_ACCELERATED);
    TTF_Init();
}
// chess game working
int startChess()
{
    initSDLs();

    int quit = 0;
    SDL_Event e;
    start(xrenderer3);
    SDL_RenderPresent(xrenderer3);
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
                startHumanVsHuman();
            }
            else if (e.key.keysym.sym == SDLK_2)
            {
                // SDL_DestroyRenderer(xrenderer);
                // SDL_DestroyWindow(xwindow);
                SDL_Quit();
                NotDone();
            }
            else if (e.key.keysym.sym == SDLK_3)
            {
                // SDL_DestroyRenderer(xrenderer);
                // SDL_DestroyWindow(xwindow);
                SDL_Quit();
                callFunction();
            }
            start(xrenderer3);
            SDL_RenderPresent(xrenderer3);
        }
        start(xrenderer3);
        SDL_RenderPresent(xrenderer3);
    }

    SDL_DestroyRenderer(xrenderer3);
    SDL_DestroyWindow(xwindow3);
    SDL_Quit();
    TTF_Quit();

    return 0;
}