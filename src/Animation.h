#ifndef __Arkanoid__Animation__
#define __Arkanoid__Animation__

#include "SDL.h"

#include "scaler.h"

class Animation
{
 public:
  Animation (const char* filename, int maxFrame, int frameDelay, int frameWidth,     /* Requires a valid path to an existing image */
             int frameHeight, int animationColumns, int animationDirection);
  Animation (SDL_Surface *image, int maxFrame, int frameDelay, int frameWidth,       /* Requires an already scaled bitmap, used in MapLoader */
             int frameHeight, int animationColumns, int animationDirection);
  virtual ~Animation ();

  bool IsAutoAnimation ();                                                           // helper for blocks that have animation based on health
  void SetFrame (int curFrame) { Animation::curFrame = curFrame; }                   // curFrame setter for special cases

  int GetFrameWidth () const { return frameWidth; }
  int GetFrameHeight () const { return frameHeight; }
  void Animate ();                                                                   // Updating sprite
  void Draw (float x, float y) const;                                               // Drawing animation at (x,y)

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
};

#endif /* defined(__Arkanoid__Animation__) */
