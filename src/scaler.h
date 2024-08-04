#ifndef Arkanoid_scaler_h
#define Arkanoid_scaler_h

#include "SDL.h"

SDL_Surface* LoadScaledBitmap (const char*, int, int); // Main function for returning scaled and prepared bitmap for use
SDL_Surface* ScaleSurface (SDL_Surface*, int, int);    // scale bitmap function

SDL_Surface* LoadCroppedBitmap (const char*, const struct SDL_Rect&); // Main function for returning cropped and prepared bitmap for use
SDL_Surface* CropSurface (SDL_Surface*, const struct SDL_Rect&);      // crop bitmap function

Uint32 getpixel (SDL_Surface*, int, int);       // Helper function for getting pixel from SDL_Surface
void putpixel (SDL_Surface*, int, int, Uint32); // Helper function for placing pixel on SDL_Surface

#endif
