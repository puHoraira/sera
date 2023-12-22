#include <SDL2/SDL.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "global.h"
#include "starter.h"
#include "credit.h"

SDL_Window *gwindow = NULL;
SDL_Renderer *grenderer = NULL;
SDL_Texture *gtexturer = NULL;
char *z_font = "Monaco.ttf";
void StartScreen(SDL_Renderer *grenderer)
{
    /* Clear the screen with selected color */
    SDL_SetRenderDrawColor(grenderer, 55, 55, 55, 255);
    SDL_RenderClear(grenderer);
    char *singleButton = "Press 1 for CHESS";
    char *doubleButton = "Press 2 for TIC_TAC_TOE";
    char *credit = "Press 3 for CREDITS";
    char *gameName = "Please select one game.";
    TTF_Font *monaco = TTF_OpenFont(z_font, 24);
    if (monaco == NULL)
    {
        printf("Error opening font: %s\n", SDL_GetError());
        return;
    }
    SDL_Color blue = {0, 100, 255};
    SDL_Surface *singleButtonSurface = TTF_RenderText_Blended(monaco, singleButton, blue);
    SDL_Texture *singleButtonTexture = SDL_CreateTextureFromSurface(grenderer, singleButtonSurface);
    if (!singleButtonTexture)
    {
        printf("error creating texture: %s\n", SDL_GetError());
        SDL_FreeSurface(singleButtonSurface);
        TTF_CloseFont(monaco);
        return;
    }

    SDL_Surface *doubleButtonSurface = TTF_RenderText_Blended(monaco, doubleButton, blue);
    SDL_Texture *doubleButtonTexture = SDL_CreateTextureFromSurface(grenderer, doubleButtonSurface);
    if (!doubleButtonTexture)
    {
        printf("error creating texture: %s\n", SDL_GetError());
        SDL_FreeSurface(doubleButtonSurface);
        TTF_CloseFont(monaco);
        return;
    }

    SDL_Surface *bgSurface = TTF_RenderText_Blended(monaco, credit, blue);
    SDL_Texture *bgTexture = SDL_CreateTextureFromSurface(grenderer, bgSurface);
    if (!bgTexture)
    {
        printf("error creating texture: %s\n", SDL_GetError());
        SDL_FreeSurface(bgSurface);
        TTF_CloseFont(monaco);
        return;
    }

    SDL_Surface *gameNameSurface = TTF_RenderText_Blended(monaco, gameName, blue);
    SDL_Texture *gameNameTexture = SDL_CreateTextureFromSurface(grenderer, gameNameSurface);
    if (!gameNameTexture)
    {
        printf("error creating texture: %s\n", SDL_GetError());
        SDL_FreeSurface(gameNameSurface);
        TTF_CloseFont(monaco);
        return;
    }

    SDL_Rect single_button_rect;
    single_button_rect.x = (WINDOW_WIDTH - singleButtonSurface->w) / 2;
    single_button_rect.y = (WINDOW_HEIGHT - singleButtonSurface->h) / 2 + 5;
    single_button_rect.w = singleButtonSurface->w;
    single_button_rect.h = singleButtonSurface->h;

    SDL_Rect double_button_rect;
    double_button_rect.x = (WINDOW_WIDTH - doubleButtonSurface->w) / 2;
    double_button_rect.y = (WINDOW_HEIGHT - doubleButtonSurface->h) / 2 + 50;
    double_button_rect.w = doubleButtonSurface->w;
    double_button_rect.h = doubleButtonSurface->h;

    SDL_Rect game_name_rect;
    game_name_rect.x = (WINDOW_WIDTH - gameNameSurface->w) / 2;
    game_name_rect.y = (WINDOW_HEIGHT - gameNameSurface->h) / 2 - 100;
    game_name_rect.w = gameNameSurface->w;
    game_name_rect.h = gameNameSurface->h;
    SDL_Rect bg_rect;
    bg_rect.x = (WINDOW_WIDTH - gameNameSurface->w) / 2;
    bg_rect.y = (WINDOW_HEIGHT - gameNameSurface->h) / 2 + 100;
    bg_rect.w = gameNameSurface->w;
    bg_rect.h = gameNameSurface->h;

    SDL_RenderCopy(grenderer, singleButtonTexture, NULL, &single_button_rect);
    SDL_RenderCopy(grenderer, doubleButtonTexture, NULL, &double_button_rect);
    SDL_RenderCopy(grenderer, gameNameTexture, NULL, &game_name_rect);
    SDL_RenderCopy(grenderer, bgTexture, NULL, &bg_rect);

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

void initSDL()
{
    SDL_Init(SDL_INIT_VIDEO);
    gwindow = SDL_CreateWindow("Game Selector", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
    grenderer = SDL_CreateRenderer(gwindow, -1, SDL_RENDERER_ACCELERATED);
    TTF_Init();
}

int starter()
{
    initSDL();

    int quit = 0;
    SDL_Event e;
    StartScreen(grenderer);
    SDL_RenderPresent(grenderer);
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
                SDL_DestroyRenderer(grenderer);
                SDL_DestroyWindow(gwindow);
                SDL_Quit();
                TTF_Quit();
                return chess;
            }
            if (e.key.keysym.sym == SDLK_2)
            {
                SDL_DestroyRenderer(grenderer);
                SDL_DestroyWindow(gwindow);
                SDL_Quit();
                TTF_Quit();
                return tictactoe;
            }
            if (e.key.keysym.sym == SDLK_3)
            {
                SDL_DestroyRenderer(grenderer);
                SDL_DestroyWindow(gwindow);
                SDL_Quit();
                TTF_Quit();
                return credit;
            }
            StartScreen(grenderer);
            SDL_RenderPresent(grenderer);
        }
        StartScreen(grenderer);
        SDL_RenderPresent(grenderer);
    }

    SDL_DestroyRenderer(grenderer);
    SDL_DestroyWindow(gwindow);
    SDL_Quit();
    TTF_Quit();

    return 0;
}