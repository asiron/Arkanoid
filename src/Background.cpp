#include "stdafx.h"

#include "Background.h"

#include "defines.h"
#include "scaler.h"
#include "Game.h"

Background::Background (const char* filename, int width, int height)
{
  float stretch_x = g_Game.GetScreen_W () / (float)BASE_SCREEN_X;
  float stretch_y = g_Game.GetScreen_H () / (float)BASE_SCREEN_Y;

  int new_width  = static_cast<int> (width *stretch_x);
  int new_height = static_cast<int> (height*stretch_y);

  image = LoadScaledBitmap (filename, new_width, new_height);

  Background::width = new_width;
  Background::height = new_height;

  Background::x = static_cast<float>(g_GamePtr->GetScreen_W ()) / 2.0F;
  Background::y = static_cast<float>(g_GamePtr->GetScreen_H ()) / 2.0F;

  if (image)
  {
    clip = new SDL_Rect ();
    clip->x = 0;
    clip->y = 0;
    clip->w = Background::width;
    clip->h = Background::height;
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
  SDL_Rect offset = {(Sint16)x, (Sint16)y, 0, 0};
  SDL_BlitSurface (image, clip, g_Game.GetScreen (), &offset);
}

void
Background::InitBackground ()
{

}

void
Background::UpdateBackground (float x, float y)
{
  Background::x = x - (x * width  / static_cast<float> (g_GamePtr->GetScreen_W ()));
  Background::y = y - (y * height / static_cast<float> (g_GamePtr->GetScreen_H ()));
}
