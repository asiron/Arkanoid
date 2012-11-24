//
//  Platform.cpp
//  Arkanoid
//
//  Created by Maciej Żurad on 11/18/12.
//  Copyright (c) 2012 Maciej Żurad. All rights reserved.
//

#include "Platform.h"


Platform::Platform(const char* filename, int maxFrame, int frameDelay, int frameWidth,
           int frameHeight, int animationColumns, int animationDirection )
            : GameObject(filename, maxFrame, frameDelay, frameWidth, frameHeight, animationColumns, animationDirection)
{
    SetID(PLAYER);
}

void Platform::Destroy(){
    GameObject::Destroy();
}

void Platform::Init() {
    
    GameObject::Init(g_Game.GetScreen_W()/2.0, g_Game.GetScreen_H() - 20 , 10, 0, 0, 0, 50.0, 15.0);
    SetAlive(true);
    
    lives = 3;
    score = 0;
    
}

void Platform::Update(){
    GameObject::Update();
    if( x > g_Game.GetScreen_W() - boundX)
        x = g_Game.GetScreen_W() - boundX;
    else if ( x < boundX)
        x = boundX ;
    
    if(animation) animation->Animate();
}
void Platform::Render(){
    GameObject::Render();

//    SDL_Rect rect = {(Sint16)(x-boundX),(Sint16)(y-boundY),(Uint16)(2*boundX),(Uint16)(2*boundY)};
//    SDL_FillRect(g_Game.GetScreen(), &rect, 0xFFFFFF);
    if(animation) animation->Draw(x, y);
    
}

void Platform::MoveLeft(){
    dirX = -1;
}
void Platform::MoveRight(){
    dirX = 1;
}

void Platform::StopMoving(){
    dirX = 0;
}


void Platform::Collided( int objectID){}