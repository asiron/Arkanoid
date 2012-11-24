//
//  Animation.cpp
//  Arkanoid
//
//  Created by Maciej Żurad on 11/22/12.
//  Copyright (c) 2012 Maciej Żurad. All rights reserved.
//

#include "Animation.h"
#include "Game.h"

Animation::Animation(const char* filename, int maxFrame, int frameDelay, int frameWidth, int frameHeight, int animationColumns, int animationDirection ){
    
    Animation::maxFrame = maxFrame;
    Animation::curFrame = 0;
    Animation::frameCount = 0;
    Animation::frameDelay = frameDelay;
    Animation::frameWidth = frameWidth;
    Animation::frameHeight = frameHeight;
    Animation::animationColumns = animationColumns;
    Animation::animationDirection = animationDirection;
    
    
    SDL_Surface *temp = IMG_Load(filename);
    if(!temp){
        cerr << "IMG_Load: " << IMG_GetError() << endl ;
        exit(1);
    }
    image = SDL_DisplayFormatAlpha(temp);
    SDL_FreeSurface(temp);
    
    if(image){
        clip = new SDL_Rect();
        clip->x = 0;
        clip->y = 0;
        clip->w = frameWidth;
        clip->h = frameHeight;
    }
};

Animation::~Animation(){
    if(image){
        SDL_FreeSurface(image);
        delete clip;
    }
}

void Animation::Animate(){
    if(++frameCount > frameDelay){
        curFrame += animationDirection;
        
        if(curFrame >= maxFrame){
            animationDirection *= -1;
            curFrame = maxFrame;    
        }
        if(curFrame <= 0 ){
            animationDirection *= -1;
            curFrame = 0;
        }
        frameCount = 0;
        
        clip->x = (curFrame % animationColumns) * clip->w;
        clip->y = (curFrame / animationColumns) * clip->h;
    }
}

void Animation::Draw(float x, float y){
    SDL_Rect offset = {(Sint16)x, (Sint16)y, 0, 0};
    SDL_BlitSurface(image, clip, g_Game.GetScreen(), &offset);
}