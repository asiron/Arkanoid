//
//  main.cpp
//  Arkanoid
//
//  Created by Maciej Żurad on 11/18/12.
//  Copyright (c) 2012 Maciej Żurad. All rights reserved.
//

#include <iostream>
#include "SDLMain.h"
#include <SDL/SDL.h>
#include <SDL_ttf/SDL_ttf.h>
#include <SDL_mixer/SDL_mixer.h>
#include <SDL_image/SDL_image.h>
#include "Game.h"

int main(int argc, char** argv)
{
    srand(SDL_GetTicks());
    Game game(argc, argv);
    return game.Loop();
}
