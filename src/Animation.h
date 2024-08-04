#ifndef __Arkanoid__Animation__
#define __Arkanoid__Animation__

#include "SDL.h"

class Animation
{
 public:
  Animation (const char*, int, int, int, int, int, int); /* Requires a valid path to an existing image */
  Animation (SDL_Surface*, int, int, int, int, int, int); /* Requires an already scaled bitmap, used in MapLoader */
  virtual ~Animation ();

  inline bool IsAutoAnimation () { return !animationDirection; } // helper for blocks that have animation based on health
  inline void SetFrame (int curFrame_in) { curFrame = curFrame_in; } // curFrame setter for special cases

  // LARGE effect
  inline SDL_Surface* GetImage () { return image; }
  inline void SetImage (SDL_Surface* image_in, int frameWidth_in) { image = image_in; frameWidth = frameWidth_in; clip->w = frameWidth_in; }

  inline int GetFrameWidth () const { return frameWidth; }
  inline int GetFrameHeight () const { return frameHeight; }
  void Animate ();                                            // Updating sprite
  void Draw (float, float) const;                             // Drawing animation at (x,y)

 private:
  int              maxFrame;
  int              curFrame;
  int              frameCount;
  int              frameDelay;
  int              frameWidth;
  int              frameHeight;
  int              animationColumns;
  int              animationDirection;
  struct SDL_Rect* clip;
  SDL_Surface*     image;
  bool             freeImage;
};

#endif /* defined(__Arkanoid__Animation__) */
