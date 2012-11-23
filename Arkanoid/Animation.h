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
    SDL_Surface* image;
    
    
public:
    Animation(SDL_Surface *image) : maxFrame(0), curFrame(0), frameCount(0), frameDelay(0), frameWidth(0),
    frameHeight(0), animationColumns(0), animationDirection(0), image(NULL){};
    ~Animation();
    
    void Animate();
    void Draw(int x, int y);


};

#endif /* defined(__Arkanoid__Animation__) */
