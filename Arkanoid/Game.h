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
#include "FpsCounter.h"
#include "Platform.h"
#include "Ball.h"

#define g_Game Game::GetSingleton()
#define BASE_SCREEN_X 960
#define BASE_SCREEN_Y 540

enum control_type{KEYBOARD, MOUSE};

using namespace std;

class Platform;
class Ball;

class Game : public Singleton<Game> {
    
private:
    int initSystems();
    void closeSystems();
    
    bool running;
    bool paused;
    bool displayFPS;
    bool musicOn;
    bool sfxOn;
    enum control_type control_type;
    
    int screen_w;
    int screen_h;
    int gameFPS;
    
    list<GameObject*> gobjects;
    
    Ball* ball;
    Platform* platform ;
    FpsCounter* fps_counter;
    
    SDL_Surface *screen;
    Mix_Chunk *sound;
    Mix_Music *music;
    TTF_Font *font;
    
    
    
public:
    Game(int argc, char** argv);
    ~Game();
    int Loop();
    void HandleEvents();
    
    void ShutDown(){running = false;}
    
    
    void SetScreen_W(int screen_w) { Game::screen_w = screen_w; }
    int GetScreen_W() {return screen_w; }
    
    void SetScreen_H(int screen_h) { Game::screen_h = screen_h; }
    int GetScreen_H() {return screen_h;}
    
    
    bool isMusicOn() { return musicOn;}
    bool isSfxOn() {return sfxOn;}
    
    void switchMusic() { musicOn = !musicOn; }
    void switchSfx() { sfxOn = !sfxOn; }
    
    SDL_Surface* GetScreen() {return screen;}
    
};
#endif /* defined(__Arkanoid__Game__) */
