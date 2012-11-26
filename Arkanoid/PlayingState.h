//
//  PlayingState.h
//  Arkanoid
//
//  Created by Maciej Żurad on 11/26/12.
//  Copyright (c) 2012 Maciej Żurad. All rights reserved.
//

#ifndef __Arkanoid__PlayingState__
#define __Arkanoid__PlayingState__

#include <iostream>
#include "Game.h"
#include "State.h"
#include "GameObject.h"
#include "Platform.h"
#include "Ball.h"
#include "Block.h"
#include "Effect.h"
#include "Projectile.h"
#include "MapLoader.h"



class Platform;
class Ball;
class Block;
class Effect;
class Projectile;

class MapLoader;

using namespace std;

class PlayingState : public State {

private:
    list<GameObject*> gobjects;
    MapLoader* map_loader;
    
    Ball* ball;
    Platform* platform;
    Effect* effect;
    Effect* effect2;
    Projectile* projectile;
    
public:
    PlayingState();
    ~PlayingState();
    
    void InitState();
    void RenderState();
    void UpdateState();
    void HandleEvents(Uint8* keystates, SDL_Event event, int control_type);
    
    Platform* GetPlatform() {return platform;}
};
#endif /* defined(__Arkanoid__PlayingState__) */
