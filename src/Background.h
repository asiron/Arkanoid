#ifndef __Arkanoid__Background__
#define __Arkanoid__Background__

#include "SDL.h"

class Background
{
 public:
  Background (const char* filename, int width, int height);
  ~Background ();
  void RenderBackground ();
  void UpdateBackground (float x, float y);
  void InitBackground ();

 private:
  float x;
  float y;
  int width;
  int height;
  SDL_Surface* image;
  SDL_Rect* clip;
};

#endif /* defined(__Arkanoid__Background__) */
