#include "stdafx.h"

#include "MenuState.h"

#include "ace/OS.h"
#include "ace/Log_Msg.h"

#include "defines.h"
#include "Game.h"
#include "Music.h"

#define RenderTextColor(_text, _color) TTF_RenderText_Solid (font, _text, _color) // helpful macro for rendering text with color
// macro for placing 3 SDL_Surfaces in different colours defined as below
#define RenderText(_text) TTF_RenderText_Solid(font, _text, text),   \
                          TTF_RenderText_Solid(font, _text, shade),  \
                          TTF_RenderText_Solid(font, _text, highlight)

MenuState::MenuState ()
 : inherited ()
 , bgs ()
 , menu_main ()
 , menu_highscores ()
 , menu_options ()
 , mouse_pos_x (-1)
 , mouse_pos_y (-1)
 , offsetX ((g_GamePtr->GetScreen_W () / 2) - 250)
 , offsetY ((g_GamePtr->GetScreen_H () / 2) - 50)
 , distance_between_msg (50)
 , distance_between_opt (300)
 , curMenu (MAIN_MENU)
 , text ()
 , shade ()
 , highlight ()
 , font (NULL)
 , startgame (NULL)
 , options (NULL)
 , highscores (NULL)
 , quit (NULL)
 , mainmenu (NULL)
 , switchcontrol (NULL)
 , musicon (NULL)
 , soundon (NULL)
 , showfps (NULL)
{
  char buffer_a[PATH_MAX];
  ACE_OS::getcwd (buffer_a, sizeof (char[PATH_MAX]));
  std::string path_base = buffer_a;
  path_base += ACE_DIRECTORY_SEPARATOR_STR_A;
  path_base += RESOURCE_DIRECTORY;
  std::string file = path_base;
  file += ACE_DIRECTORY_SEPARATOR_STR_A;
  file += ACE_TEXT_ALWAYS_CHAR (GRAPHICS_DIRECTORY);
  file += ACE_DIRECTORY_SEPARATOR_STR_A;
  file += ACE_TEXT_ALWAYS_CHAR ("background2.jpg");
  bgs.push_back (new Background (file.c_str (), 1366, 768));
  //file = path_base;
  //file += ACE_DIRECTORY_SEPARATOR_STR;
  //file += ACE_TEXT_ALWAYS_CHAR (GRAPHICS_DIRECTORY);
  //file += ACE_DIRECTORY_SEPARATOR_STR;
  //file += ACE_TEXT_ALWAYS_CHAR ("starsback.png");
  //bgs.push_back (new Background (file.c_str (), 1700, 900));
  //file = path_base;
  //file += ACE_DIRECTORY_SEPARATOR_STR;
  //file += ACE_TEXT_ALWAYS_CHAR (GRAPHICS_DIRECTORY);
  //file += ACE_DIRECTORY_SEPARATOR_STR;
  //file += ACE_TEXT_ALWAYS_CHAR ("starsmidground.png");
  //bgs.push_back (new Background (file.c_str (), 1800, 1050));
  file = path_base;
  file += ACE_DIRECTORY_SEPARATOR_STR_A;
  file += ACE_TEXT_ALWAYS_CHAR (GRAPHICS_DIRECTORY);
  file += ACE_DIRECTORY_SEPARATOR_STR_A;
  file += ACE_TEXT_ALWAYS_CHAR ("starsforeground.png");
  bgs.push_back (new Background (file.c_str (), 1550, 950));
  file = path_base;
  file += ACE_DIRECTORY_SEPARATOR_STR_A;
  file += ACE_TEXT_ALWAYS_CHAR (GRAPHICS_DIRECTORY);
  file += ACE_DIRECTORY_SEPARATOR_STR_A;
  file += ACE_TEXT_ALWAYS_CHAR ("starsforeforeground.png");
  bgs.push_back (new Background (file.c_str (), 1800, 1050));

  file = path_base;
  file += ACE_DIRECTORY_SEPARATOR_STR_A;
  file += ACE_TEXT_ALWAYS_CHAR ("font.ttf");
  font = TTF_OpenFont (file.c_str (), MEDIUM_FONT_SIZE); // loading menu font
  if (!font)
  {
    ACE_DEBUG ((LM_ERROR,
                ACE_TEXT ("failed to TTF_OpenFont: \"%s\", aborting\n"),
                ACE_TEXT (TTF_GetError ())));
    ACE_OS::exit (1);
  }

  //Function pointers
  startgame = ChangeState;       // pointer to function in Game.h to change between states
  quit = ShutDown;               // pointer to function in Game.h that performs an application shutdown
  options = GotoOptions;         // pointer in to function that changes curMenu to OPTIONS
  highscores = GotoHighscores;   // pointer in to function that changes curMenu to HIGHSCORES
  mainmenu  = GotoMainMenu;      // pointer in to function that changes curMenu to MAINMENU
  switchcontrol = SwitchControl; // pointer to function in Game.h that switches control_type between Keyboard and Mouse
  musicon = SwitchMusic;         // pointer to function in Game.h that switches musicON flag on or off
  soundon = SwitchSfx;           // pointer to function in Game.h that switches sfxON flag on or off
  showfps = SwitchFPSVisibility; // pointer to function in Game.h that switches displayFPS flag on or off

  // message colors
  text = {0xFF, 0, 0, 0};
  shade = {0xc8, 0xc8, 0xc8, 0} ;
  highlight = {0, 0xFF, 0, 0};

  // creating main_menu objects
  menu_main.push_back (MainMenuText (std::make_tuple (RenderText("Start Game")), false, startgame));
  menu_main.push_back (MainMenuText (std::make_tuple (RenderText("Options")), false, options));
  menu_main.push_back (MainMenuText (std::make_tuple (RenderText("Highscores")), false, highscores));
  menu_main.push_back (MainMenuText (std::make_tuple (RenderText("Quit")), false, quit));

  // creating highscore objects
  for (std::list<std::pair<std::string, int> >::iterator iter = highsco_list.begin (); iter != highsco_list.end (); iter++)
    menu_highscores.push_back (Highscore_Text (std::make_tuple (RenderText((iter->first).c_str ())), false, std::make_tuple (RenderText(IntToStr (iter->second).c_str ()))));

  // creating menu option objects
  menu_options.push_back (OptionsText (std::make_tuple (RenderText("Control")), false, switchcontrol, std::make_tuple (RenderText ("KEYBOARD")), std::make_tuple (RenderText ("MOUSE")), g_GamePtr->GetControl () == KEYBOARD, CONTROL));
  menu_options.push_back (OptionsText (std::make_tuple (RenderText("Music")), false, musicon, std::make_tuple (RenderText ("ON")), std::make_tuple (RenderText ("OFF")), g_GamePtr->GetMusic ()->isMusicOn (), MUSICON));
  menu_options.push_back (OptionsText (std::make_tuple (RenderText("Sounds")), false, soundon, std::make_tuple (RenderText ("ON")), std::make_tuple (RenderText ("OFF")), g_GamePtr->isSfxOn (), SOUNDON));
  menu_options.push_back (OptionsText (std::make_tuple (RenderText("Show FPS")), false, showfps, std::make_tuple (RenderText ("ON")), std::make_tuple (RenderText ("OFF")), g_GamePtr->isFPSVisible (), SHOWFPS));
}

void
MenuState::Destroy ()
{
  for (std::list<Background*>::iterator iter = bgs.begin (); iter != bgs.end (); iter++)
    delete *iter;

  for (std::list<MainMenuText>::iterator iter = menu_main.begin (); iter != menu_main.end (); iter++)
  {
    SDL_FreeSurface (std::get<0> (std::get<0> (*iter)));
    SDL_FreeSurface (std::get<1> (std::get<0> (*iter)));
    SDL_FreeSurface (std::get<2> (std::get<0> (*iter)));
  }

  for (std::list<Highscore_Text>::iterator iter = menu_highscores.begin (); iter != menu_highscores.end (); iter++)
  {
    SDL_FreeSurface (std::get<0> (std::get<0> (*iter)));
    SDL_FreeSurface (std::get<1> (std::get<0> (*iter)));
    SDL_FreeSurface (std::get<2> (std::get<0> (*iter)));
  }

  for (std::list<OptionsText>::iterator iter = menu_options.begin (); iter != menu_options.end (); iter++)
  {
    SDL_FreeSurface (std::get<0> (std::get<0> (*iter)));
    SDL_FreeSurface (std::get<1> (std::get<0> (*iter)));
    SDL_FreeSurface (std::get<2> (std::get<0> (*iter)));

    SDL_FreeSurface (std::get<0> (std::get<3> (*iter)));
    SDL_FreeSurface (std::get<1> (std::get<3> (*iter)));
    SDL_FreeSurface (std::get<2> (std::get<3> (*iter)));

    SDL_FreeSurface (std::get<0> (std::get<4> (*iter)));
    SDL_FreeSurface (std::get<1> (std::get<4> (*iter)));
    SDL_FreeSurface (std::get<2> (std::get<4> (*iter)));
  }

  TTF_CloseFont (font);
}

void
MenuState::UpdateInfo (int ID)
{
  for (std::list<OptionsText>::iterator iter = menu_options.begin (); iter != menu_options.end (); iter++)
  {
    if (ID == std::get<6> (*iter))
      std::get<5> (*iter) = !std::get<5> (*iter);
  }
}

void
MenuState::RenderState ()
{
  for (std::list<Background*>::iterator iter = bgs.begin (); iter != bgs.end (); iter++)
    (*iter)->RenderBackground ();

  int y = offsetY;
  switch (curMenu)
  {
    case MAIN_MENU:
      for (std::list<MainMenuText>::iterator iter = menu_main.begin (); iter != menu_main.end (); iter++)
      {
        Draw (std::get<1> (std::get<0> (*iter)), offsetX + 1, y + 1);
        if (std::get<1> (*iter))
          Draw (std::get<2> (std::get<0> (*iter)), offsetX, y);
        else
          Draw (std::get<0> (std::get<0> (*iter)), offsetX, y);
        y += distance_between_msg;
      }
      break;
    case OPTIONS:
      for (std::list<OptionsText>::iterator iter = menu_options.begin (); iter != menu_options.end (); iter++)
      {
        Draw (std::get<1> (std::get<0> (*iter)), offsetX + 1, y + 1);
        if (std::get<1> (*iter))
          Draw (std::get<2> (std::get<0> (*iter)), offsetX, y);
        else
          Draw (std::get<0> (std::get<0> (*iter)), offsetX, y);

        if (std::get<5> (*iter))
        {
          Draw (std::get<1> (std::get<3> (*iter)), offsetX + distance_between_opt + 1, y + 1);
          if (std::get<1> (*iter))
            Draw (std::get<2> (std::get<3> (*iter)), offsetX + distance_between_opt, y);
            else
              Draw (std::get<0> (std::get<3> (*iter)), offsetX + distance_between_opt, y);
        }
        else
        {
          Draw (std::get<1> (std::get<4> (*iter)), offsetX + distance_between_opt + 1, y + 1);
          if (std::get<1> (*iter))
            Draw (std::get<2> (std::get<4> (*iter)), offsetX + distance_between_opt, y);
          else
            Draw (std::get<0> (std::get<4> (*iter)), offsetX + distance_between_opt, y);
        }
        y += distance_between_msg;
      }
      break;
    case HIGHSCORES:
      int counter = 0 ; // we have to allow only print 5 highscores
      for (std::list<Highscore_Text>::iterator iter = menu_highscores.begin (); iter != menu_highscores.end () && counter<5; iter++)
      {
        Draw (std::get<1> (std::get<0> (*iter)), offsetX + 1, y + 1);
        if (std::get<1> (*iter))
          Draw (std::get<2> (std::get<0> (*iter)), offsetX, y);
          else
            Draw (std::get<0> (std::get<0> (*iter)), offsetX, y);

        Draw (std::get<1> (std::get<2> (*iter)), offsetX + distance_between_opt + 1, y + 1);
        if (std::get<1> (*iter))
          Draw (std::get<2> (std::get<2> (*iter)), offsetX + distance_between_opt, y);
        else
          Draw (std::get<0> (std::get<2> (*iter)), offsetX + distance_between_opt, y);

        y += distance_between_msg;
        counter++;
      }
      break;
  }
}

void
MenuState::UpdateState ()
{
  for (std::list<Background*>::iterator iter = bgs.begin (); iter != bgs.end (); iter++)
    (*iter)->UpdateBackground (static_cast<float> (g_GamePtr->GetScreen_W ()) - mouse_pos_x,
                               static_cast<float> (g_GamePtr->GetScreen_H ()) - mouse_pos_y);

  switch (curMenu)
  {
    case MAIN_MENU:
      UpdateList (menu_main);
      break;
    case OPTIONS:
      UpdateList (menu_options);
      break;
    case HIGHSCORES:
      UpdateList (menu_highscores);
      break;
    default:
      break;
  }
}

void
MenuState::HandleEvents (Uint8* keystates, const SDL_Event& event, int control_type)
{
  SDL_GetMouseState (&mouse_pos_x, &mouse_pos_y);
  bool is_right_mouse_down_b = ((event.type == SDL_MOUSEBUTTONDOWN) &&
                                (event.button.type == SDL_MOUSEBUTTONDOWN) &&
                                (event.button.button == SDL_BUTTON_RIGHT));

  switch (curMenu)
  {
    case MAIN_MENU:
    {
#if defined (SDL1_USE)
      if (keystates[SDLK_ESCAPE] ||
#elif defined (SDL2_USE)
      if (keystates[SDL_SCANCODE_ESCAPE] ||
#endif // SDL1_USE || SDL2_USE
          is_right_mouse_down_b)
      {
        quit ();
        return;
      }
      break;
    }
    case OPTIONS:
    {
#if defined (SDL1_USE)
      if (keystates[SDLK_ESCAPE] ||
#elif defined (SDL2_USE)
      if (keystates[SDL_SCANCODE_ESCAPE] ||
#endif // SDL1_USE || SDL2_USE
          is_right_mouse_down_b)
      {
        GotoMainMenu ();
        return;
      }
      break;
    }
    case HIGHSCORES:
    {
#if defined (SDL1_USE)
      if (keystates[SDLK_ESCAPE] ||
#elif defined (SDL2_USE)
      if (keystates[SDL_SCANCODE_ESCAPE] ||
#endif // SDL1_USE || SDL2_USE
          is_right_mouse_down_b)
      {
        GotoMainMenu ();
        return;
      }
      break;
    }
    default:
      break;
  }

  if (event.type == SDL_MOUSEBUTTONDOWN)
    if (event.button.type == SDL_MOUSEBUTTONDOWN)
    {
      if (event.button.button == SDL_BUTTON_LEFT)
      {
        switch (curMenu)
        {
          case MAIN_MENU:
            RunCommand (menu_main);
            break;
          case OPTIONS:
            RunCommand (menu_options);
            break;
          default:
            break;
        } // end SWITCH
      } // end IF
    } // end IF
}

inline
void
MenuState::Draw (SDL_Surface* image, int x, int y) const
{
  SDL_Rect offset = {(Sint16)x, (Sint16)y, 0, 0};
  SDL_Rect clip = {0, 0, (Uint16)image->w, (Uint16)image->h};
  SDL_BlitSurface (image, &clip, g_Game.GetScreen(), &offset);
}

template <class T>
void
MenuState::UpdateList (std::list<T>& menu_list)
{
  int counter = 0;

  for (typename std::list<T>::iterator iter = menu_list.begin (); iter != menu_list.end (); iter++)
  {
    if ((mouse_pos_x >= offsetX)                                          &&
        (mouse_pos_x <= offsetX + (std::get<0> (std::get<0> (*iter)))->w) &&
        (mouse_pos_y >= offsetY + counter)                                &&
        (mouse_pos_y <= offsetY + counter + std::get<0> (std::get<0> (*iter))->h))
      std::get<1> (*iter) = true;
    else
      std::get<1> (*iter) = false;

    counter += distance_between_msg;
  }
}

template <class T>
void
MenuState::RunCommand (std::list<T>& menu_list)
{
  for (typename std::list<T>::iterator iter = menu_list.begin ();
       iter != menu_list.end ();
       iter++)
    if (std::get<1> (*iter))
    {
      std::get<2> (*iter) ();
      break;
    }
}

void
GotoOptions ()
{
  static_cast<MenuState*> (g_GamePtr->GetState ())->curMenu = OPTIONS;
}

void
GotoHighscores ()
{
  static_cast<MenuState*> (g_GamePtr->GetState ())->curMenu = HIGHSCORES;
}

void
GotoMainMenu ()
{
  static_cast<MenuState*> (g_GamePtr->GetState ())->curMenu = MAIN_MENU;
}
