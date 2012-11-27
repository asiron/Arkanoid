//
//  Game.cpp
//  Arkanoid
//
//  Created by Maciej Żurad on 11/18/12.
//  Copyright (c) 2012 Maciej Żurad. All rights reserved.
//

#include "Game.h"



Game::~Game(){
    
    delete fps_counter;
    delete game_state;
    
    closeSystems();
}

Game::Game(int argc, char** argv){
    
    SDL_WM_SetCaption("ARKANOID", NULL);
    
    screen_w = atoi(argv[1]);
    screen_h = atoi(argv[2]);
    
    if(initSystems() == -1)
        cerr << "Problem occured while initializing SDL systems" << endl;
    
    running = true ;
    
    //just for now
    gameFPS = 60;
    control_type = KEYBOARD;
    current_state = MENU ;
    
    
    fps_counter = new FpsCounter(gameFPS);
    
    
    game_state = new MenuState();
    game_state->InitState();
    
}

int Game::initSystems(){
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		cerr << "Problem while initializing SDL" << endl;
		return -1;
	}
	screen = SDL_SetVideoMode(screen_w, screen_h, 32, SDL_HWSURFACE);
	if (TTF_Init() < 0) {
		cerr << "Problem initializing SDL_ttf" << endl;
		return -1;
	}
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) < 0) {
		cerr << "Problem initializing SDL_mixer" << endl;
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
            
            game_state->UpdateState();
            game_state->RenderState();
            
            SDL_Flip(screen);
        }
    }
    return 0;
}


void Game::HandleEvents(){

    SDL_Event event;
    while(SDL_PollEvent(&event))
        if(event.type == SDL_QUIT)
            ShutDown();

    Uint8* keystates = SDL_GetKeyState(NULL);
    
    if(keystates[SDLK_q])
        ::ShutDown();
    
    game_state->HandleEvents(keystates, event, control_type);
    
}


void ChangeState(){
    Game* game = g_GamePtr;
    if(game->current_state == PLAYING){
        delete game->game_state;
        game->game_state = new MenuState();
        game->game_state->InitState();
        
    } else if (game->current_state == MENU){
        delete game->game_state;
        game->game_state = new PlayingState();
        game->game_state->InitState();
    }
}

void ShutDown(){
    g_GamePtr->running = false;
}

void SwitchFPSVisibility(){
    g_GamePtr->displayFPS = !g_GamePtr->displayFPS;
}
void SwitchMusic(){
    g_GamePtr->musicOn = !g_GamePtr->musicOn ;
}
void SwitchSfx(){
    g_GamePtr->sfxOn = !g_GamePtr->sfxOn;
}














