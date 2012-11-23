//
//  Game.cpp
//  Arkanoid
//
//  Created by Maciej Żurad on 11/18/12.
//  Copyright (c) 2012 Maciej Żurad. All rights reserved.
//

#include "Game.h"
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 640
#define SCREEN_DEPTH 32


Game::~Game(){
    closeSystems();
    
    delete fps_counter;
    delete platform;
    delete ball;
}

Game::Game(int argc, char** argv){
    
    screen_w = atoi(argv[1]);
    screen_h = atoi(argv[2]);
    
    if(initSystems() == -1)
        cerr << "Problem occured while initializing SDL systems" << endl;
    running = true ;
    
    //just for now
    gameFPS = 60;
    
    platform = new Platform();
    platform->Init();
    
    ball = new Ball();
    ball->Init();
    
    gobjects.push_back(ball);
    gobjects.push_back(platform);
    
    
    fps_counter = new FpsCounter(gameFPS);
}

int Game::initSystems(){
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		cout << "Problem while initializing SDL" << endl;
		return -1;
	}
	screen = SDL_SetVideoMode(screen_w, screen_h, 32, SDL_HWSURFACE);
	if (TTF_Init() < 0) {
		cout << "Problem initializing SDL_ttf" << endl;
		return -1;
	}
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) < 0) {
		cout << "Problem initializing SDL_mixer" << endl;
		return -1;
	}
	return 0;
}

void Game::closeSystems(){
    if(!screen) SDL_FreeSurface(screen);
    if (!font) TTF_CloseFont(font);
    if (!sound) Mix_FreeChunk(sound);
    if (!music) Mix_FreeMusic(music);
    Mix_CloseAudio();
    TTF_Quit();
    SDL_Quit();
}

int Game::Loop(){
    while(running){
        if(fps_counter->measureFPS()){
            
            HandleEvents();
            SDL_FillRect(screen, NULL, 0);
            
            if(platform->detectCollision(ball))
                ball->Collided(platform->GetID());
            
            for(list<GameObject*>::iterator iter = gobjects.begin(); iter!=gobjects.end(); iter++){
                (*iter)->Update();
                (*iter)->Render();
            }
            
            
            SDL_Flip(screen);
        }
    }
    return 0;
}


void Game::HandleEvents(){
    
    bool mouse_motion_flag = false;
    
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        if(event.type == SDL_QUIT)
            ShutDown();
        if(event.type == SDL_MOUSEMOTION){
            
            mouse_motion_flag = true ;
            if(event.motion.x > platform->GetX())
                platform->MoveRight();
            else if (event.motion.x < platform->GetX())
                platform->MoveLeft();
            else
                platform->StopMoving();
        }
    }
    
    Uint8* keystates = SDL_GetKeyState(NULL);
    
    if(keystates[SDLK_q])
        ShutDown();
    
    if(!mouse_motion_flag){
        if(keystates[SDLK_LEFT])
            platform->MoveLeft();
        else if(keystates[SDLK_RIGHT])
            platform->MoveRight();
        else
            platform->StopMoving();
    }
    
    if(keystates[SDLK_SPACE])
        ball->StartFlying();
    
    

//
//        if(event.type == SDL_KEYDOWN){
//            if(event.key.keysym.sym == SDLK_LEFT)
//                platform->MoveLeft();
//            if(event.key.keysym.sym == SDLK_RIGHT)
//                platform->MoveRight();
//            if(event.key.keysym.sym == SDLK_SPACE)
//                ball->StartFlying();
//            if(event.key.keysym.sym == SDLK_q)
//                ShutDown();
//                
//        }else if (event.type == SDL_KEYUP){
//            if(event.key.keysym.sym == SDLK_LEFT)
//                platform->StopMoving();
//            if(event.key.keysym.sym == SDLK_RIGHT)
//                platform->StopMoving();                
//        }

    }




















