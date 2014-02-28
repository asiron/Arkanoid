//
//  main.cpp
//  Arkanoid
//
//  Created by Maciej Żurad on 11/18/12.
//  Copyright (c) 2012 Maciej Żurad. All rights reserved.
//

#include <iostream>
#include <SDL/SDL.h>
#ifdef __GNUC__
#include <SDL/SDL_main.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_image.h>
#else
#include "SDLMain.h"
#include <SDL_ttf/SDL_ttf.h>
#include <SDL_mixer/SDL_mixer.h>
#include <SDL_image/SDL_image.h>
#endif
#include "Game.h"

int main(int argc, char** argv)
{
    srand(SDL_GetTicks()); // Initializing random seed
    Game game(argc, argv); // Creating game
    return game.Loop();    // Starting game
}
