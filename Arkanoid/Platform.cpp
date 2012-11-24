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
    //setting ID to PLAYER and calling superclass constructor
    SetID(PLAYER);
}

void Platform::Destroy(){
    //calling superclass constructor
    GameObject::Destroy();
}

void Platform::Init() {
    
    // Initializing ball
    GameObject::Init(g_Game.GetScreen_W()/2.0, g_Game.GetScreen_H() - 70 , 10, 0, 0, 0, animation->GetFrameWidth()/2, animation->GetFrameHeight()/2);
    
    SetAlive(true);
    
    lives = 3;
    score = 0;
    
}

void Platform::Update(){
    if(isAlive()){
        GameObject::Update();
        //Performing boundry checking
        if( x > g_Game.GetScreen_W() - boundX)
            x = g_Game.GetScreen_W() - boundX;
        else if ( x < boundX)
            x = boundX ;
        //Updating of animation exists
        if(animation) animation->Animate();
    }
}
void Platform::Render(){
    if(isAlive()){
        GameObject::Render();
        //Rendering if animation exists
        if(animation) animation->Draw(x-boundX, y-boundY);
    }
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


void Platform::Collided( int objectID, col_dir dir){}