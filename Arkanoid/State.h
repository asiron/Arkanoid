//
//  State.h
//  Arkanoid
//
//  Created by Maciej Żurad on 11/26/12.
//  Copyright (c) 2012 Maciej Żurad. All rights reserved.
//

#ifndef __Arkanoid__State__
#define __Arkanoid__State__

#include <iostream>
#include <SDL/SDL.h>

class State {
public:
    
    State() {};
    virtual ~State() {};
    
    void virtual HandleEvents(Uint8* keystates, SDL_Event event, int control_type) {};
    void virtual RenderState() {};
    void virtual UpdateState() {};
    void virtual InitState() {};
    
};


#endif /* defined(__Arkanoid__State__) */
