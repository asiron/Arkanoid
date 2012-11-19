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
    int lastMeasure;
    int currentMeasure;
    int FPS;
    int FPS_rate;
    
public:
    FpsCounter(int FPS_rate) : fps(0), lastMeasure(0), currentMeasure(0), FPS(0), FPS_rate(FPS_rate) {};
    int measureFPS();
    int getFPS();
};

#endif /* defined(__Arkanoid__FpsCounter__) */
