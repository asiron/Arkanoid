#include "stdafx.h"

#include "Animation.h"

#include "defines.h"
#include "Game.h"

Animation::Animation (const char* filename_in,
                      int maxFrame_in,
                      int frameDelay_in,
                      int frameWidth_in,
                      int frameHeight_in,
                      int animationColumns_in,
                      int animationDirection_in)
 : curFrame (0)
 , frameCount (0)
 , maxFrame (maxFrame_in)
 , frameDelay (frameDelay_in)
 , animationColumns (animationColumns_in)
 , animationDirection (animationDirection_in)
 , image (NULL)
 , clip (NULL)
 , freeImage (true)
{
  frameWidth = static_cast<int>  (g_Game.GetScreen_W () / static_cast<float> (BASE_SCREEN_X) * frameWidth_in);
  frameHeight = static_cast<int> (g_Game.GetScreen_H () / static_cast<float> (BASE_SCREEN_Y) * frameHeight_in);

  // formjula for checking if the last row is full in spritesheet if so then we have to adjust height of a loading image
  int loading_image_w = frameWidth * animationColumns;
  int loading_image_h;
  if ((maxFrame+1)%animationColumns)
    loading_image_h = frameHeight*((maxFrame+1)/animationColumns+1);
  else
    loading_image_h = frameHeight*(((maxFrame+1)/animationColumns));

  //loading image to SDL_Surface* with new width and height
  image = LoadScaledBitmap (filename_in, loading_image_w, loading_image_h);
  if (image)
  {
    clip = new SDL_Rect ();
    clip->x = 0;
    clip->y = 0;
    clip->w = frameWidth;
    clip->h = frameHeight;
  }
}

Animation::Animation (SDL_Surface* image_in,
                      int maxFrame_in,
                      int frameDelay_in,
                      int frameWidth_in,
                      int frameHeight_in,
                      int animationColumns_in,
                      int animationDirection_in)
 : curFrame (0)
 , frameCount (0)
 , maxFrame (maxFrame_in)
 , frameDelay (frameDelay_in)
 , animationColumns (animationColumns_in)
 , animationDirection (animationDirection_in)
 , image (NULL)
 , clip (NULL)
 , freeImage (false)
{
  frameWidth  = static_cast<int> (g_Game.GetScreen_W () / static_cast<float> (BASE_SCREEN_X) * frameWidth_in);
  frameHeight = static_cast<int> (g_Game.GetScreen_H () / static_cast<float> (BASE_SCREEN_Y) * frameHeight_in);

  image = image_in;
  if (image)
  {
    clip = new SDL_Rect ();
    clip->x = 0;
    clip->y = 0;
    clip->w = frameWidth;
    clip->h = frameHeight;
  }
}

Animation::~Animation ()
{
  if (image && freeImage)
    SDL_FreeSurface (image);
  if (clip)
    delete clip;
}

void
Animation::Animate ()
{
  if (++frameCount > frameDelay)
  {
    curFrame += animationDirection;
    if (curFrame > maxFrame)
    {
      curFrame = 0;
    }
    if (curFrame < 0)
    {
      curFrame = maxFrame;
    }
    frameCount = 0;

    clip->x = (curFrame % animationColumns) * clip->w;
    clip->y = (curFrame / animationColumns) * clip->h;
  }
}

void
Animation::Draw (float x, float y) const
{
  SDL_Rect offset = {(Sint16)x, (Sint16)y, 0, 0};
  SDL_BlitSurface (image, clip, g_Game.GetScreen (), &offset);
}

bool
Animation::IsAutoAnimation ()
{
  return !animationDirection;
};
