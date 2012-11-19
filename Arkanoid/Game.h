//
//  Game.h
//  Arkanoid
//
//  Created by Maciej Żurad on 11/18/12.
//  Copyright (c) 2012 Maciej Żurad. All rights reserved.
//

#ifndef __Arkanoid__Game__
#define __Arkanoid__Game__

#include <iostream>
#include <list>
#include <SDL/SDL.h>
#include <SDL_ttf/SDL_ttf.h>
#include <SDL_mixer/SDL_mixer.h>
#include <SDL_image/SDL_image.h>
#include "GameObject.h"
#include "Singleton.h"

#define g_Game Game::GetSingleton()

using namespace std;

class Game : public Singleton<Game> {
    
private:
    int initSystems();
    void closeSystems();
    
    bool running;
    bool paused;
    bool displayFPS;
    bool musicOn;
    bool sfxOn;
    
    int screen_w;
    int screen_h;
    
    list<GameObject*> gobjects;
    
    SDL_Surface *screen;
    Mix_Chunk *sound;
    Mix_Music *music;
    TTF_Font *font;
    
    
    
public:
    Game(int argc, char** argv);
    ~Game();
    int loop();
    
    void SetScreen_W(int screen_w) { Game::screen_w = screen_w; }
    int GetScreen_W() {return screen_w; }
    
    void SetScreen_H(int screen_h) { Game::screen_h = screen_h; }
    int GetScreen_H() {return screen_h;}
    
    
    bool isMusicOn() { return musicOn;}
    bool isSfxOn() {return sfxOn;}
    
    void switchMusic() { musicOn = !musicOn; }
    void switchSfx() { sfxOn = !sfxOn; }
    
    
};
#endif /* defined(__Arkanoid__Game__) */
