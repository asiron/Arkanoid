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
    
    GameObject::Init(g_Game.GetScreen_W()/2.0, g_Game.GetScreen_H() - 20 , 5, 0, 0, 0, 25, 10);
    
    cout << g_Game.GetScreen_W() << g_Game.GetScreen_H() << endl ;
    
    SetAlive(true);
    SetID(PLAYER);
    
    lives = 3;
    score = 0;
    
    ///Graphics here
}

void Platform::Update(){
    GameObject::Update();
    if( x > g_Game.GetScreen_H())
        x = g_Game.GetScreen_W();
    else if ( x < 0 )
        x = 0 ;
}
void Platform::Render(){
    GameObject::Render();
    
    SDL_Rect rect = {(short)x,(short)y,50,20};
    SDL_FillRect(g_Game.GetScreen(), &rect, 0xFFFFFF);
    
    
}

void Platform::MoveLeft(){
    dirX = -1;
}
void Platform::MoveRight(){
    dirX = 1;
}


void Platform::Collided( int objectID){}