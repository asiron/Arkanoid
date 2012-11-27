//
//  PlayingState.cpp
//  Arkanoid
//
//  Created by Maciej Żurad on 11/26/12.
//  Copyright (c) 2012 Maciej Żurad. All rights reserved.
//

#include "PlayingState.h"

PlayingState::PlayingState(){
    
    
    platform = new Platform("../../Arkanoid/data/graphics/platform.png", 0, 1, 64, 16, 1, 1);
    ball = new Ball("../../Arkanoid/data/graphics/ball.png", 0, 1, 16, 16, 1, 1);
    effect = new Effect("../../Arkanoid/data/graphics/effect.png", 23, 4, 16, 14, 24, 1);
    effect->Init(400, 300, 1);
    effect2 = new Effect("../../Arkanoid/data/graphics/effect2.png", 10, 7, 34, 29, 11, 1);
    effect2->Init(300, 300, 1);
    
    gobjects.push_back(effect2);
    gobjects.push_back(effect);
    gobjects.push_back(platform);
    gobjects.push_back(ball);
    
    map_loader = new MapLoader("../../Arkanoid/data/config");
    
    gobjects.splice(gobjects.end(), map_loader->LoadMap("../../Arkanoid/data/map.cfg"));
}

PlayingState::~PlayingState(){
    
    delete map_loader;
    for(list<GameObject*>::iterator iter = gobjects.begin(); iter != gobjects.end(); iter++)
        (*iter)->Destroy();
}

void PlayingState::RenderState(){
    for(list<GameObject*>::iterator iter = gobjects.begin(); iter!=gobjects.end(); iter++)
        (*iter)->Render();
}

void PlayingState::UpdateState(){
    for(list<GameObject*>::iterator iter = gobjects.begin(); iter!=gobjects.end(); iter++)
        for(list<GameObject*>::iterator iter2 = gobjects.begin(); iter2!=gobjects.end(); iter2++)
            (*iter)->Collided((*iter2)->GetID(), (*iter)->detectCollision(*iter2));
    
    for(list<GameObject*>::iterator iter = gobjects.begin(); iter!=gobjects.end(); iter++)
        (*iter)->Update();
}

void PlayingState::HandleEvents(Uint8* keystates, SDL_Event event, int control_type){
    
    if(keystates[SDLK_ESCAPE]){
        ChangeState();
        return;             // we get the hell out of here
    }
    
    if(keystates[SDLK_SPACE])
        ball->StartFlying();
    
    // Movement controls with keyboard
    if(control_type == KEYBOARD){
        if(keystates[SDLK_LEFT])
            platform->MoveLeft();
        else if(keystates[SDLK_RIGHT])
            platform->MoveRight();
        else
            platform->StopMoving();
        
    } else if(control_type == MOUSE){
        int x; // mouse x coordinate position
        SDL_GetMouseState(&x, NULL);
        if(x - 10 > platform->GetX())
            platform->MoveRight();
        else if (x + 10 < platform->GetX())
            platform->MoveLeft();
        else
            platform->StopMoving();
    }
}

void PlayingState::InitState(){
    for(list<GameObject*>::iterator iter = gobjects.begin(); iter!=gobjects.end(); iter++)
        (*iter)->Init();
}

