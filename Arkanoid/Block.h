//
//  Block.h
//  Arkanoid
//
//  Created by Maciej Żurad on 11/24/12.
//  Copyright (c) 2012 Maciej Żurad. All rights reserved.
//

#ifndef __Arkanoid__Block__
#define __Arkanoid__Block__

#include <iostream>
#include "GameObject.h"
#include "Game.h"

class Block : public GameObject {

private:
    int health;
    
public:
    Block(const char* filename = NULL, int maxFrame = 0, int frameDelay = 0, int frameWidth = 0,
         int frameHeight = 0, int animationColumns = 0, int animationDirection = 0 ) ;
    
    void Destroy();
    void Init();
    void Render();
    void Update();
    
    //respond to collision function
    void Collided(int ObjectID);

};

#endif /* defined(__Arkanoid__Block__) */
