//
//  Ball.h
//  Arkanoid
//
//  Created by Maciej Żurad on 11/21/12.
//  Copyright (c) 2012 Maciej Żurad. All rights reserved.
//

#ifndef __Arkanoid__Ball__
#define __Arkanoid__Ball__

#include <iostream>
#include "GameObject.h"
#include "Game.h"

class Ball : public GameObject {
    
public:
    Ball();
    void Destroy();
    void Init(SDL_Surface* image = NULL);
    void Render();
    void Update();
    
    void StartFlying();
    //void KillBall();
    void Collided(int ObjectID);
    
    
};
#endif /* defined(__Arkanoid__Ball__) */
