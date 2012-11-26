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
#include "State.h"

class MenuState : public State {
    
public:
    MenuState();
    void Destroy();

    void InitState();
    void RenderState();
    void UpdateState();
    void HandleEvents(Uint8* keystates, SDL_Event event, int control_type);
};

#endif /* defined(__Arkanoid__MenuState__) */
