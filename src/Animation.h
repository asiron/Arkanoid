#ifndef __Arkanoid__Animation__
#define __Arkanoid__Animation__

#include "SDL.h"

#include "scaler.h"

class Animation
{
 public:
  Animation (const char*, int, int, int,                       /* Requires a valid path to an existing image */
             int, int, int);
  Animation (SDL_Surface*, int, int, int,                      /* Requires an already scaled bitmap, used in MapLoader */
             int, int, int);
  virtual ~Animation ();

  bool IsAutoAnimation ();                                    // helper for blocks that have animation based on health
  void SetFrame (int curFrame_in) { curFrame = curFrame_in; } // curFrame setter for special cases

  int GetFrameWidth () const { return frameWidth; }
  int GetFrameHeight () const { return frameHeight; }
  void Animate ();                                            // Updating sprite
  void Draw (float, float) const;                             // Drawing animation at (x,y)

 private:
  int maxFrame;
  int curFrame;
  int frameCount;
  int frameDelay;
  int frameWidth;
  int frameHeight;
  int animationColumns;
  int animationDirection;
  SDL_Rect* clip;
  SDL_Surface* image;
  bool freeImage;
};

#endif /* defined(__Arkanoid__Animation__) */
