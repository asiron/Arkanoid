#ifndef __Arkanoid__Background__
#define __Arkanoid__Background__

#include "SDL.h"

class Background
{
 public:
  Background (const char*, int, int);
  ~Background ();
  void RenderBackground ();
  void UpdateBackground (float x, float y);
  inline void InitBackground () {}

 private:
  float            x;
  float            y;
  int              width;
  int              height;
  SDL_Surface*     image;
  struct SDL_Rect* clip;
};

#endif /* defined(__Arkanoid__Background__) */
