
//
//  Block.cpp
//  Arkanoid
//
//  Created by Maciej Żurad on 11/18/12.
//  Copyright (c) 2012 Maciej Żurad. All rights reserved.
//

#include "Block.h"


Block::Block(const char* filename, int maxFrame, int frameDelay, int frameWidth,
                   int frameHeight, int animationColumns, int animationDirection )
: GameObject(filename, maxFrame, frameDelay, frameWidth, frameHeight, animationColumns, animationDirection)
{
    //setting ID to PLAYER and calling superclass constructor
    SetID(BLOCK);
}

void Block::Destroy(){
    //calling superclass constructor
    GameObject::Destroy();
}

void Block::Init() {
    
    // Initializing block
    GameObject::Init(g_Game.GetScreen_W()/2.0, g_Game.GetScreen_H()/2.0 , 10, 0, 1, 0, animation->GetFrameWidth()/2, animation->GetFrameHeight()/2);
    
    SetAlive(true);
    health = 2;

}

void Block::Update(){
    if(isAlive()){
        GameObject::Update();
        //Performing boundry checking
        if( x > g_Game.GetScreen_W() - boundX || x < boundX)
            dirX *= -1;
        //Updating of animation exists
        if(animation) animation->Animate();
    }
}
void Block::Render(){
    if(isAlive()){
        GameObject::Render();
        //Rendering if animation exists
        if(animation) animation->Draw(x-boundX, y-boundY);
    }
}


void Block::Collided( int objectID)
{
    if(objectID == BALL){
        if(!--health)
            SetAlive(false);
    }
}