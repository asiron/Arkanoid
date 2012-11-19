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
}

Game::Game(int argc, char** argv){
    if(initSystems() == -1)
        cerr << "Problem occured while initializing SDL systems" << endl;
    running = true ;
    
}

int Game::initSystems(){
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		cout << "Problem while initializing SDL" << endl;
		return -1;
	}
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_DEPTH, SDL_SWSURFACE);
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

int Game::loop(){
    
    while(running){
        cout << running << endl;
        running = false ;
    }
    return 0;
}