//
//  Ball.cpp
//  Arkanoid
//
//  Created by Maciej Żurad on 11/21/12.
//  Copyright (c) 2012 Maciej Żurad. All rights reserved.
//

#include "Ball.h"

Ball::Ball(){
    SetID(BALL);
}

void Ball::Destroy(){
    GameObject::Destroy();
}

void Ball::Init(SDL_Surface *image){
    int t_dirX = (rand() % 2 +1)*2 -3 ;
    GameObject::Init(g_Game.GetScreen_W()/2, g_Game.GetScreen_H() - 50.0, rand()%7 +3, rand()%7 +3, t_dirX, -1, 15.0, 15.0);

}

void Ball::Render(){
    if(isAlive()){
        GameObject::Render();
        SDL_Rect rect = {(Sint16)(x-boundX),(Sint16)(y-boundY),(Uint16)(2*boundX),(Uint16)(2*boundY)};
        SDL_FillRect(g_Game.GetScreen(), &rect, 0xFFFFFF);
    }
}


void Ball::Update(){
    if(isAlive()){
        GameObject::Update();
        if( x >= g_Game.GetScreen_W() -boundX || x<= boundX)
            dirX *= -1;
        else if( y <= boundY)
            dirY *= -1;
        else if ( y >= g_Game.GetScreen_H())
            SetAlive(false);
    }
}
void Ball::StartFlying(){
    if(!isAlive()){
        SetAlive(true);
        Init();
    }
}

//void Ball::KillBall(){
//    SetAlive(false)
//}

void Ball::Collided(int ObjectID){
    if(ObjectID == PLAYER){
        dirY *= -1;
        if(velX <0) --velX; else ++velX;
        if(velY <0) --velY; else ++velY;
    }
        
        
}