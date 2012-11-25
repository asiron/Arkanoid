//
//  Effect.cpp
//  Arkanoid
//
//  Created by Maciej Żurad on 11/25/12.
//  Copyright (c) 2012 Maciej Żurad. All rights reserved.
//

#include "Effect.h"

Effect::Effect(const char* filename, int maxFrame, int frameDelay, int frameWidth,
               int frameHeight, int animationColumns, int animationDirection )
                : GameObject(filename, maxFrame, frameDelay, frameWidth, frameHeight, animationColumns, animationDirection)
{
    SetID(EFFECT);
}


void Effect::Destroy(){
    GameObject::Destroy();
}

void Effect::Init(int x, int y, int velY){
    GameObject::Init(x, y, 0, velY, 0, 1, animation->GetFrameWidth()/2.0, animation->GetFrameHeight()/2.0);
    SetAlive(true);
    
}

void Effect::Update(){
    if(isAlive()){
        GameObject::Update();
        if(animation) animation->Animate();
    }
}

void Effect::Render() {
    if(isAlive()){
        GameObject::Render();
        if(animation) animation->Draw(x-boundX, y-boundY);
    }
}

void Effect::Collided(int ObjectID, col_dir dir){
    if(ObjectID == PLAYER){
        //do something
    }
}