//
//  Ball.cpp
//  Arkanoid
//
//  Created by Maciej Żurad on 11/21/12.
//  Copyright (c) 2012 Maciej Żurad. All rights reserved.
//

#include "Ball.h"

Ball::Ball(const char* filename, int maxFrame, int frameDelay, int frameWidth,
           int frameHeight, int animationColumns, int animationDirection )
            : GameObject(filename, maxFrame, frameDelay, frameWidth, frameHeight, animationColumns, animationDirection)
{
    //setting ID and calling superclass constructor
    SetID(BALL);
}

void Ball::Destroy(){
    // object destructor calling superclass destructor
    GameObject::Destroy();
}

void Ball::Init(){
    int t_dirX = (rand() % 2 +1)*2 -3 ; //  picking random direction either left or right
    GameObject::Init(g_Game.GetScreen_W()/2, g_Game.GetScreen_H() - 50.0, rand()%7 + 3,  rand()%7 +  3, t_dirX, -1, animation->GetFrameWidth()/2, animation->GetFrameHeight()/2);
}

void Ball::Render(){
    
    //if object is alive we draw it 
    if(isAlive()){
        GameObject::Render();
        
        //If there is an animation, we draw it so that the centre of it is at (x,y)
        if(animation) animation->Draw(x-boundX,y-boundY);
    }
}


void Ball::Update(){
    //if object is alive we update it 
    if(isAlive()){
        GameObject::Update();
        
        // we do a boundry checking
        if( x >= g_Game.GetScreen_W() -boundX || x<= boundX)
            dirX *= -1;
        else if( y <= boundY)
            dirY *= -1;
        else if ( y >= g_Game.GetScreen_H())
            SetAlive(false);
        
        // we also update its animation if it exists
        if(animation) animation->Animate();
    }

}
void Ball::StartFlying(){
    // starting the ball, if it's not alive we dont allow for starting it again, until it dies
    if(!isAlive()){
        SetAlive(true);
        Init();
    }
}

void Ball::Collided(int ObjectID, col_dir dir){
    if(dir == NO_COLLISION)
        return;
    
    //checking for collision with PLAYER
    if(ObjectID == PLAYER){
        switch(dir){
            case LEFT:
                dirX = -1;
                break;
            case RIGHT:
                dirX = 1;
                break;
            case TOP:
                dirY = -1;
                break;
            case TLCOR:
                dirX = -1;
                dirY = -1;
                break;
            case TRCOR:
                dirX = 1;
                dirY = -1;
                break;
 
        }
        if(velX <0) --velX; else ++velX;
        if(velY <0) --velY; else ++velY;
    } else if(ObjectID == BLOCK){
        switch(dir){
            case LEFT:
                dirX = -1;
                break;
            case RIGHT:
                dirX = 1;
                break;
            case TOP:
                dirY = -1;
                break;
            case BOTTOM:
                dirY = 1;
                break;
            case TLCOR:
                dirX = -1;
                dirY = -1;
                break;
            case TRCOR:
                dirX = 1;
                dirY = -1;
                break;
            case BLCOR:
                dirX = -1;
                dirY = 1;
                break;
            case BRCOR:
                dirX = 1;
                dirY = 1;
                break;
        }
    }
}