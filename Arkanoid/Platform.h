//
//  Platform.h
//  Arkanoid
//
//  Created by Maciej Żurad on 11/18/12.
//  Copyright (c) 2012 Maciej Żurad. All rights reserved.
//

#ifndef __Arkanoid__Platform__
#define __Arkanoid__Platform__

#include <iostream>
#include "GameObject.h"
#include "Game.h"

class Platform : public GameObject {
    
private:
    int lives;
    int score;
    
public:
    Platform();
    void Destroy();
    
    void Init(SDL_Surface *image = NULL);
    void Update();
    void Render();
    
    void MoveLeft();
    void MoveRight();
    void StopMoving();
    
    
    int GetScore() {return score;}
    int GetLives() {return lives;}
    
    void LoseLife() { --lives;}
    void AddPoint() { ++score;}
    
    void Collided( int objectID);
};


#endif /* defined(__Arkanoid__Platform__) */
