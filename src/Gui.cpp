#include "stdafx.h"

#include "Gui.h"

#include "ace/OS.h"
#include "ace/Log_Msg.h"

#include "defines.h"
#include "Game.h"
#include "Platform.h"
#include "PlayingState.h"

Gui::Gui ()
 : x (0)
 , y (0)
 , score (0)
 , lives (0)
 , score_msg (NULL)
 , lives_msg (NULL)
 , font (NULL)
{
  color = {0x0c, 0xac, 0xd0, 0};

  char buffer[MAX_PATH];
  ACE_OS::getcwd (buffer, sizeof (buffer));
  std::string path_base = buffer;
  path_base += ACE_DIRECTORY_SEPARATOR_STR;
  path_base += RESOURCE_DIRECTORY;
  std::string file = path_base;
  file += ACE_DIRECTORY_SEPARATOR_STR;
  file += ACE_TEXT_ALWAYS_CHAR ("font.ttf");
  font = TTF_OpenFont (file.c_str (), SMALL_FONT_SIZE);
  if (!font)
  {
    ACE_DEBUG ((LM_ERROR,
                ACE_TEXT ("failed to TTF_OpenFont: \"%s\", aborting\n"),
                ACE_TEXT (TTF_GetError ())));
    exit (1);
  }
};

Gui::~Gui ()
{
  SDL_FreeSurface (score_msg);
  SDL_FreeSurface (lives_msg);
  if (font) TTF_CloseFont (font);
}

void
Gui::Init (int x_in, int y_in)
{
  x = x_in;
  y = y_in;
}

void Gui::Update ()
{
  Platform* platform = dynamic_cast<PlayingState*> (g_GamePtr->GetState ())->GetPlatform ();
  score = platform->GetScore ();
  lives = platform->GetLives ();
}

void
Gui::Render ()
{
  score_msg = TTF_RenderText_Solid (font, ("Score: " + IntToStr (score)).c_str (), color);
  if (!score_msg)
  {
    std::cerr << "failed to render score " << TTF_GetError << std::endl;
    exit (1);
  }
  lives_msg = TTF_RenderText_Solid (font, ("Lives: " + IntToStr (lives)).c_str (), color);
  if (!score_msg)
  {
    std::cerr << "failed to render lives " << TTF_GetError << std::endl;
    exit (1);
  }

  Game::Draw (g_GamePtr->GetScreen (), score_msg, x, y);
  Game::Draw (g_GamePtr->GetScreen (), lives_msg, x, y+score_msg->h);
}
