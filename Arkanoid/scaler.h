//
//  scaler.h
//  Arkanoid
//
//  Created by Maciej Żurad on 11/24/12.
//  Copyright (c) 2012 Maciej Żurad. All rights reserved.
//

#ifndef Arkanoid_scaler_h
#define Arkanoid_scaler_h

#include <SDL/SDL.h>
#include <SDL_image/SDL_image.h>
#include <iostream>

SDL_Surface* LoadScaledBitmap(const char* filename, int width, int height);
Uint32 getpixel(SDL_Surface *surface, int x, int y);
void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
SDL_Surface *ScaleSurface(SDL_Surface *Surface, Uint16 Width, Uint16 Height);

#endif
