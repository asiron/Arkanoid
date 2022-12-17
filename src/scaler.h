#ifndef Arkanoid_scaler_h
#define Arkanoid_scaler_h

#include <iostream>

#include "SDL.h"
#ifdef __GNUC__
#include "SDL/SDL_image.h"
#else
#include "SDL_image.h"
#endif

SDL_Surface* LoadScaledBitmap (const char* filename, int width, int height);             // Main function for returning scaled and prepared bitmap for use
Uint32 getpixel (SDL_Surface* surface, int x, int y);                                    // Helper function for getting pixel from SDL_Surface
void putpixel (SDL_Surface* surface, int x, int y, Uint32 pixel);                        // Helper function for placing pixel on SDL_Surface
SDL_Surface* ScaleSurface (SDL_Surface* Surface, Uint16 Width, Uint16 Height);           // scale bitmap function

#endif
