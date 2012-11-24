//
//  Animation.h
//  Arkanoid
//
//  Created by Maciej Żurad on 11/22/12.
//  Copyright (c) 2012 Maciej Żurad. All rights reserved.
//

#ifndef __Arkanoid__Animation__
#define __Arkanoid__Animation__

#include <iostream>
#include <SDL/SDL.h>
#include "scaler.h"

class Animation {
    
private:
    int maxFrame;
    int curFrame;
    int frameCount;
    int frameDelay;
    int frameWidth;
    int frameHeight;
    int animationColumns;
    int animationDirection;
    SDL_Rect* clip;
    SDL_Surface* image;
    
    
public:
    Animation(const char* filename, int maxFrame, int frameDelay, int frameWidth,
              int frameHeight, int animationColumns, int animationDirection );
    ~Animation();

    int GetFrameWidth(){ return frameWidth; }
    int GetFrameHeight(){ return frameHeight; }
    void Animate();
    void Draw(float x, float y);

    
};

#endif /* defined(__Arkanoid__Animation__) */
