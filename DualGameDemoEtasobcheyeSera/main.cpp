#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "global.h"
#include "sdlshape.h"
#include "sdlevent.h"
#include "sdlrender.h"
#include "starter.h"
#include "credit.h"
#include "copyofMain.h"

int main(int argc, char *argv[])
{
    // direct calling a function from copyofMain.c

    callFunction();
    return 0;
}
