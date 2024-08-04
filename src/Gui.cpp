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
 , level (-1)
 , score (-1)
 , lives (-1)
 , level_msg (NULL)
 , score_msg (NULL)
 , lives_msg (NULL)
 , font (NULL)
{
  color = {0x0c, 0xac, 0xd0, 0};

  char buffer_a[PATH_MAX];
  ACE_OS::getcwd (buffer_a, sizeof (char[PATH_MAX]));
  std::string path_base = buffer_a;
  path_base += ACE_DIRECTORY_SEPARATOR_STR_A;
  path_base += RESOURCE_DIRECTORY;
  std::string file = path_base;
  file += ACE_DIRECTORY_SEPARATOR_STR_A;
  file += ACE_TEXT_ALWAYS_CHAR ("font.ttf");
  font = TTF_OpenFont (file.c_str (), SMALL_FONT_SIZE);
  if (!font)
  {
    ACE_DEBUG ((LM_ERROR,
                ACE_TEXT ("failed to TTF_OpenFont: \"%s\", aborting\n"),
                ACE_TEXT (TTF_GetError ())));
    ACE_OS::exit (1);
  }
};

Gui::~Gui ()
{
  SDL_FreeSurface (level_msg);
  SDL_FreeSurface (score_msg);
  SDL_FreeSurface (lives_msg);
  if (font)
    TTF_CloseFont (font);
}

void
Gui::Init (int x_in, int y_in)
{
  x = x_in;
  y = y_in;
}

void Gui::Update ()
{
  int prev_level = level, prev_score = score, prev_lives = lives;
  PlayingState* playing_state_p = static_cast<PlayingState*> (g_GamePtr->GetState ());
  level = static_cast<int> (playing_state_p->CurrentLevel ());
  if (!level)
    level++;
  Platform* platform = playing_state_p->GetPlatform ();
  score = platform->GetScore ();
  lives = platform->GetLives ();

  if (level != prev_level)
  {
    level_msg = TTF_RenderText_Solid (font, (ACE_TEXT_ALWAYS_CHAR ("Level: ") + IntToStr (level)).c_str (), color);
    if (!level_msg)
    {
      std::cerr << ACE_TEXT_ALWAYS_CHAR ("failed to render level: ") << TTF_GetError () << std::endl;
      ACE_OS::exit (1);
    } // end IF
  } // end IF

  if (score != prev_score)
  {
    score_msg = TTF_RenderText_Solid (font, (ACE_TEXT_ALWAYS_CHAR ("Score: ") + IntToStr (score)).c_str (), color);
    if (!score_msg)
    {
      std::cerr << ACE_TEXT_ALWAYS_CHAR ("failed to render score: ") << TTF_GetError () << std::endl;
      ACE_OS::exit (1);
    } // end IF
  } // end IF

  if (lives != prev_lives)
  {
    lives_msg = TTF_RenderText_Solid (font, (ACE_TEXT_ALWAYS_CHAR ("Lives: ") + IntToStr (lives)).c_str (), color);
    if (!lives_msg)
    {
      std::cerr << ACE_TEXT_ALWAYS_CHAR ("failed to render lives: ") << TTF_GetError () << std::endl;
      ACE_OS::exit (1);
    } // end IF
  } // end IF
}

void
Gui::Render ()
{ ACE_ASSERT (level_msg && score_msg && lives_msg);
  Game::Draw (g_GamePtr->GetScreen (), level_msg, x, y);
  Game::Draw (g_GamePtr->GetScreen (), score_msg, x, y + level_msg->h);
  Game::Draw (g_GamePtr->GetScreen (), lives_msg, x, y + level_msg->h + score_msg->h);
}
