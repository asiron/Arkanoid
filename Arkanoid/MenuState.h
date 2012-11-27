//
//  MenuState.h
//  Arkanoid
//
//  Created by Maciej Żurad on 11/26/12.
//  Copyright (c) 2012 Maciej Żurad. All rights reserved.
//

#ifndef __Arkanoid__MenuState__
#define __Arkanoid__MenuState__

#include <iostream>
#include <sstream>
#include <tuple>
#include "Game.h"
#include "State.h"
#include "Background.h" 

using namespace std;

enum MENU_STATE{MAIN_MENU, OPTIONS, HIGHSCORE};

//helper function for conversion
const char* IntToStr(int n);

// Function pointer to function that is run when object is clicked
typedef void (*Clicked)(void);

typedef tuple<SDL_Surface*, SDL_Surface*, SDL_Surface*> Text;

typedef tuple<Text, bool> Highscore_Text;
typedef tuple<Text, bool, Clicked> MainMenuText;
typedef tuple<Text, bool, Clicked, Text, Text, bool> OptionsText;

class Background;

class MenuState : public State {
    
    
private:
    list<Background*> bgs;
    
    list<tuple<Text, bool, Clicked>> menu_main;
    list<tuple<Text, bool>> menu_highscores;
    list<tuple<Text, bool, Clicked, Text, Text, bool>> menu_options;

    
    list<int> highsco_list;
    
    int mouse_pos_x;
    int mouse_pos_y;
    
    int offsetX;    // offset from top to first menu message
    int offsetY;    // offset from left to menu messages
    
    int distance_between_msg;       //vertical distance between consecutive MSGs
    int distance_between_opt;       //horizontal distance between Option and value
    
    int curMenu;
    
    SDL_Color text;
    SDL_Color shade;
    SDL_Color highlight;
    
    TTF_Font* font;
    void Draw(SDL_Surface* image, int x, int y) const;
    
    //function pointers to menu event listener
    Clicked startgame;
    Clicked options;
    Clicked highscores;
    Clicked quit;
    Clicked showfps;
    Clicked musicon;
    Clicked soundon;
    
    template <typename T> void UpdateList(list<T> &menu_list);
    template <typename T> void RunCommand(list<T> menu_list);
    
    
public:
    MenuState();
    void Destroy();

    void InitState();
    void RenderState();
    void UpdateState();
    void HandleEvents(Uint8* keystates, SDL_Event event, int control_type);
};

#endif /* defined(__Arkanoid__MenuState__) */
