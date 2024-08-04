#ifndef __Arkanoid__Gui__
#define __Arkanoid__Gui__

#include "SDL.h"
#include "SDL_ttf.h"

class Gui
{
 public:
  Gui ();
  ~Gui ();

  void Init (int x, int y);
  void Render ();
  void Update ();

 private:
  struct SDL_Color color;

  int              x;
  int              y;

  int              level;
  int              score;
  int              lives;

  SDL_Surface*     level_msg;
  SDL_Surface*     score_msg;
  SDL_Surface*     lives_msg;
  TTF_Font*        font;
};

#endif /* defined(__Arkanoid__Gui__) */
