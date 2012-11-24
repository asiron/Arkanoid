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
    Ball(const char* filename = NULL, int maxFrame = 0, int frameDelay = 0, int frameWidth = 0,
                    int frameHeight = 0, int animationColumns = 0, int animationDirection = 0 ) ;
    void Destroy();
    void Init();
    void Render();
    void Update();
    
    void StartFlying();
    void Collided(int ObjectID);
    
    
};
#endif /* defined(__Arkanoid__Ball__) */
