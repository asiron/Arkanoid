//
//  FpsCounter.cpp
//  Arkanoid
//
//  Created by Maciej Żurad on 11/18/12.
//  Copyright (c) 2012 Maciej Żurad. All rights reserved.
//

#include "FpsCounter.h"


int FpsCounter::measureFPS(){
    
    //Get current time
    currentMeasure = SDL_GetTicks();
    
    
    //Measring time
    if(currentMeasure - lastMeasure >= 1000){
        FPS = fps;
        fps = 0;
        lastMeasure = currentMeasure ;
    }
    
    if(currentMeasure - lastMeasure >= (1000/(double)FPS_rate) ){
        lastMeasure = currentMeasure ;
        fps++;
        
        //next frame thus we return 1 for Renderer
        return 1;
    }
    //no frame in this measure, we dont return 1 for Renderer
    return 0;
}


int FpsCounter::getFPS(){
    return FPS;
}