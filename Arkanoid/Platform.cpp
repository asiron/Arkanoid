//
//  Platform.cpp
//  Arkanoid
//
//  Created by Maciej Żurad on 11/18/12.
//  Copyright (c) 2012 Maciej Żurad. All rights reserved.
//

#include "Platform.h"


Platform::Platform(){
}

void Platform::Destroy(){
    GameObject::Destroy();
}

void Platform::Init(SDL_Surface *image) {
    
    GameObject::Init(g_Game.GetScreen_W()/2.0, g_Game.GetScreen_H() - 20 , 10, 0, 0, 0, 50.0, 15.0);
        
    SetAlive(true);
    SetID(PLAYER);
    
    lives = 3;
    score = 0;
    
    ///Graphics here
}

void Platform::Update(){
    GameObject::Update();
    if( x > g_Game.GetScreen_W() - boundX)
        x = g_Game.GetScreen_W() - boundX;
    else if ( x < boundX)
        x = boundX ;
}
void Platform::Render(){
    GameObject::Render();

    SDL_Rect rect = {(Sint16)(x-boundX),(Sint16)(y-boundY),(Uint16)(2*boundX),(Uint16)(2*boundY)};
    SDL_FillRect(g_Game.GetScreen(), &rect, 0xFFFFFF);
    
    
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