#include "stdafx.h"

#include "FpsCounter.h"

#include "SDL.h"

#include "Game.h"

FpsCounter::FpsCounter (int FPS_rate)
 : fps (0)
 , lastTime (0)
 , lastFrame (0)
 , FPS (0)
 , frameDelay (static_cast<int> (1000.0 / static_cast<double> (FPS_rate)))
{

};

bool
FpsCounter::measureFPS ()
{
  //Get current time
  int currentTime = SDL_GetTicks ();

  //Measuring time
  if (currentTime - lastTime >= 1000)
  {
    FPS = fps;
    fps = 0;
    lastTime = currentTime;
  }

  if (currentTime - lastFrame >= frameDelay)
  {
    lastFrame = currentTime;
    fps++;

    //next frame thus we return true for Renderer
    return true;
  }

  //no frame in this measure, we return false for Renderer
  return false;
}

// Too slow 
void
FpsCounter::RenderFPS (int x, int y)
{
  static struct SDL_Color col = {0xcc , 0xcc, 0xcc, 0};
  SDL_Surface* fps_message = TTF_RenderText_Solid (g_GamePtr->GetMainFont (), IntToStr (FPS).c_str (), col);

  Game::Draw (g_GamePtr->GetScreen (), fps_message, x, y);

  SDL_FreeSurface (fps_message);
}
