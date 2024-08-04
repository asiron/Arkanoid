#include "stdafx.h"

#include "Background.h"

#include "defines.h"
#include "scaler.h"
#include "Game.h"

Background::Background (const char* filename, int width_in, int height_in)
{
  float stretch_x = g_Game.GetScreen_W () / static_cast<float> (BASE_SCREEN_X);
  float stretch_y = g_Game.GetScreen_H () / static_cast<float> (BASE_SCREEN_Y);

  int new_width  = static_cast<int> (width_in * stretch_x);
  int new_height = static_cast<int> (height_in * stretch_y);

  image = LoadScaledBitmap (filename, new_width, new_height);

  width = new_width;
  height = new_height;

  x = static_cast<float> (g_GamePtr->GetScreen_W ()) / 2.0f;
  y = static_cast<float> (g_GamePtr->GetScreen_H ()) / 2.0f;

  if (image)
  {
    clip = new struct SDL_Rect ();
    clip->x = 0;
    clip->y = 0;
    clip->w = width;
    clip->h = height;
  }
}

Background::~Background ()
{
  delete clip;
  SDL_FreeSurface (image);
}

void
Background::RenderBackground ()
{
  struct SDL_Rect offset = {(Sint16)x, (Sint16)y, 0, 0};
  SDL_BlitSurface (image, clip, g_Game.GetScreen (), &offset);
}

void
Background::UpdateBackground (float x_in, float y_in)
{
  x = x_in - (x_in * width  / static_cast<float> (g_GamePtr->GetScreen_W ()));
  y = y_in - (y_in * height / static_cast<float> (g_GamePtr->GetScreen_H ()));
}
