//
//  FpsCounter.h
//  Arkanoid
//
//  Created by Maciej Żurad on 11/18/12.
//  Copyright (c) 2012 Maciej Żurad. All rights reserved.
//

#ifndef __Arkanoid__FpsCounter__
#define __Arkanoid__FpsCounter__

#include <iostream>
#include <SDL/SDL.h>
#ifdef __GNUC__
#include <SDL/SDL_ttf.h>
#else
#include <SDL_ttf/SDL_ttf.h>
#endif
#include "Game.h"

class Game;

class FpsCounter {
    
private:
    int fps;
    int lastTime;
    int lastFrame;
    int FPS;
    int frameDelay;
    
    
public:
    FpsCounter(int FPS_rate) : fps(0), lastTime(0), lastFrame(0), FPS(0) {
        frameDelay = 1000/(double)FPS_rate ;
    };
    int measureFPS();
    int getFPS();
};

#endif /* defined(__Arkanoid__FpsCounter__) */
