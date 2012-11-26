//
//  Effect.h
//  Arkanoid
//
//  Created by Maciej Żurad on 11/25/12.
//  Copyright (c) 2012 Maciej Żurad. All rights reserved.
//

#ifndef __Arkanoid__Effect__
#define __Arkanoid__Effect__

#include <iostream>
#include "GameObject.h"
#include "Game.h"

class Effect : public GameObject {
    
private:
    
public:
    Effect(const char* filename = NULL, int maxFrame = 0, int frameDelay = 0, int frameWidth = 0,
           int frameHeight = 0, int animationColumns = 0, int animationDirection  = 0);

    void Destroy();
    
    void Init();
    void Init(int x, int y, int velY);
    void Render();
    void Update();

    void Collided(int ObjectID, col_dir dir);

};

#endif /* defined(__Arkanoid__Effect__) */
