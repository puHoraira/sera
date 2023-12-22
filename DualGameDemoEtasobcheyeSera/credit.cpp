#include <SDL2/SDL.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "global.h"
#include "starter.h"
#include "credit.h"

SDL_Window *cwindow = NULL;
SDL_Renderer *crenderer = NULL;
SDL_Texture *ctexturer = NULL;
char *c_font = "Monaco.ttf";
void creditpagestarts(SDL_Renderer *crenderer)
{
    /* Clear the screen with selected color */
    SDL_SetRenderDrawColor(crenderer, 181, 101, 167, 255);
    SDL_RenderClear(crenderer);
    char *horaira = "MD. Abu Horaira";
    char *adib = "Md. Adib Ahsan";
    char *avi = "Shraban Karmoker Avi";
    char *final = "Press 1 to go back";
    TTF_Font *monaco = TTF_OpenFont(c_font, 28);
    if (monaco == NULL)
    {
        printf("Error opening font: %s\n", SDL_GetError());
        return;
    }
    SDL_Color blue = {223, 207, 190};
    SDL_Surface *horairaSurface = TTF_RenderText_Blended(monaco, horaira, blue);
    SDL_Texture *singleButtonTexture = SDL_CreateTextureFromSurface(crenderer, horairaSurface);
    if (!singleButtonTexture)
    {
        printf("error creating texture: %s\n", SDL_GetError());
        SDL_FreeSurface(horairaSurface);
        TTF_CloseFont(monaco);
        return;
    }

    SDL_Surface *adibSurface = TTF_RenderText_Blended(monaco, adib, blue);
    SDL_Texture *doubleButtonTexture = SDL_CreateTextureFromSurface(crenderer, adibSurface);
    if (!doubleButtonTexture)
    {
        printf("error creating texture: %s\n", SDL_GetError());
        SDL_FreeSurface(adibSurface);
        TTF_CloseFont(monaco);
        return;
    }

    SDL_Surface *aviSurface = TTF_RenderText_Blended(monaco, avi, blue);
    SDL_Texture *bgTexture = SDL_CreateTextureFromSurface(crenderer, aviSurface);
    if (!bgTexture)
    {
        printf("error creating texture: %s\n", SDL_GetError());
        SDL_FreeSurface(aviSurface);
        TTF_CloseFont(monaco);
        return;
    }

    SDL_Surface *finalSurface = TTF_RenderText_Blended(monaco, final, blue);
    SDL_Texture *gameNameTexture = SDL_CreateTextureFromSurface(crenderer, finalSurface);
    if (!gameNameTexture)
    {
        printf("error creating texture: %s\n", SDL_GetError());
        SDL_FreeSurface(finalSurface);
        TTF_CloseFont(monaco);
        return;
    }

    SDL_Rect single_button_rect;
    single_button_rect.x = (WINDOW_WIDTH - horairaSurface->w) / 2;
    single_button_rect.y = (WINDOW_HEIGHT - horairaSurface->h) / 2 + 50;
    single_button_rect.w = horairaSurface->w;
    single_button_rect.h = horairaSurface->h;

    SDL_Rect double_button_rect;
    double_button_rect.x = (WINDOW_WIDTH - adibSurface->w) / 2;
    double_button_rect.y = (WINDOW_HEIGHT - adibSurface->h) / 2 + 100;
    double_button_rect.w = adibSurface->w;
    double_button_rect.h = adibSurface->h;

    SDL_Rect bg_rect;
    bg_rect.x = (WINDOW_WIDTH - aviSurface->w) / 2;
    bg_rect.y = (WINDOW_HEIGHT - aviSurface->h) / 2 + 160;
    bg_rect.w = aviSurface->w;
    bg_rect.h = aviSurface->h;

    SDL_Rect game_name_rect;
    game_name_rect.x = (WINDOW_WIDTH - finalSurface->w) / 2;
    game_name_rect.y = (WINDOW_HEIGHT - finalSurface->h) / 2 - 100;
    game_name_rect.w = finalSurface->w;
    game_name_rect.h = finalSurface->h;

    SDL_RenderCopy(crenderer, singleButtonTexture, NULL, &single_button_rect);
    SDL_RenderCopy(crenderer, doubleButtonTexture, NULL, &double_button_rect);
    SDL_RenderCopy(crenderer, gameNameTexture, NULL, &game_name_rect);
    SDL_RenderCopy(crenderer, bgTexture, NULL, &bg_rect);

    SDL_FreeSurface(horairaSurface);
    SDL_FreeSurface(adibSurface);
    SDL_FreeSurface(aviSurface);
    SDL_FreeSurface(finalSurface);
    SDL_DestroyTexture(singleButtonTexture);
    SDL_DestroyTexture(doubleButtonTexture);
    SDL_DestroyTexture(gameNameTexture);
    SDL_DestroyTexture(bgTexture);
    TTF_CloseFont(monaco);
}

void initial()
{
    SDL_Init(SDL_INIT_VIDEO);
    cwindow = SDL_CreateWindow("Credit", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
    crenderer = SDL_CreateRenderer(cwindow, -1, SDL_RENDERER_ACCELERATED);
    TTF_Init();
}

int creditpagesec()
{
    initial();

    int quit = 0;
    SDL_Event e;
    creditpagestarts(crenderer);
    SDL_RenderPresent(crenderer);
    while (!quit)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quit = 1;
            }
            else if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym == SDLK_1)
                {

                    SDL_DestroyRenderer(crenderer);
                    SDL_DestroyWindow(cwindow);
                    SDL_Quit();
                    TTF_Quit();
                    return 6;
                }
            }
        }
        creditpagestarts(crenderer);
        SDL_RenderPresent(crenderer);
    }

    SDL_DestroyRenderer(crenderer);
    SDL_DestroyWindow(cwindow);
    SDL_Quit();
    TTF_Quit();
    return 0;
}