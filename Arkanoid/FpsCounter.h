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

class FpsCounter {
    
private:
    int fps;
    int lastTime;
    int lastFrame;
    int FPS;
    int FPS_rate;
    
public:
    FpsCounter(int _FPS_rate) : fps(0), lastTime(0), lastFrame(0), FPS(0), FPS_rate(_FPS_rate) {};
    int measureFPS();
    int getFPS();
};

#endif /* defined(__Arkanoid__FpsCounter__) */
