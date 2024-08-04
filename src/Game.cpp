#include "stdafx.h"

#include "Game.h"

#include <iostream>
#include <sstream>

#include "ace/Log_Msg.h"
#include "ace/OS.h"

#include "defines.h"
#include "FpsCounter.h"
#include "MenuState.h"
#include "Music.h"
#include "PlayingState.h"
#include "State.h"

Game::Game (int argc, char* argv[], u_int seed_in)
 : randomSeed (seed_in)
 , running (true)
 , paused (false)
 , displayFPS (false)
 , sfxOn (true)
 , control_type (DEFAULT_CONTROL_TYPE)
 , current_state (MENU)
 , screen_w (ACE_OS::atoi (argv[1]))
 , screen_h (ACE_OS::atoi (argv[2]))
 , gameFPS (BASE_GAME_FPS)
 , game_state (NULL)
 , fps_counter (NULL)
#if defined (SDL2_USE)
 , window (NULL)
 //, renderer (NULL)
 //, texture (NULL)
#endif // SDL2_USE
 , screen (NULL)
 , music (NULL)
 , collision (NULL)
 , laser (NULL)
 , powerup (NULL)
 , font (NULL)
{
  if (initSystems () == -1)
  {
    ACE_DEBUG ((LM_ERROR,
                ACE_TEXT ("failed to initSystems, aborting\n")));
    ACE_OS::exit (1);
  } // end IF

  char buffer_a[PATH_MAX];
  ACE_OS::getcwd (buffer_a, sizeof (char[PATH_MAX]));
  std::string path_base = buffer_a;
  path_base += ACE_DIRECTORY_SEPARATOR_STR_A;
  path_base += RESOURCE_DIRECTORY;
  std::string file = path_base;
  file += ACE_DIRECTORY_SEPARATOR_STR_A;
  file += SOUNDS_DIRECTORY;
  file += ACE_DIRECTORY_SEPARATOR_STR_A;
  file += ACE_TEXT_ALWAYS_CHAR ("collision.wav");
  collision = Mix_LoadWAV (file.c_str ());
  if (!collision)
  {
    ACE_DEBUG ((LM_ERROR,
                ACE_TEXT ("failed to Mix_LoadWAV: \"%s\", aborting\n"),
                ACE_TEXT (Mix_GetError ())));
    ACE_OS::exit (1);
  } // end IF
  file = path_base;
  file += ACE_DIRECTORY_SEPARATOR_STR_A;
  file += SOUNDS_DIRECTORY;
  file += ACE_DIRECTORY_SEPARATOR_STR_A;
  file += ACE_TEXT_ALWAYS_CHAR ("laser.wav");
  laser = Mix_LoadWAV (file.c_str ());
  if (!laser)
  {
    ACE_DEBUG ((LM_ERROR,
                ACE_TEXT ("failed to Mix_LoadWAV: \"%s\", aborting\n"),
                ACE_TEXT (Mix_GetError ())));
    ACE_OS::exit (1);
  } // end IF
  file = path_base;
  file += ACE_DIRECTORY_SEPARATOR_STR_A;
  file += SOUNDS_DIRECTORY;
  file += ACE_DIRECTORY_SEPARATOR_STR_A;
  file += ACE_TEXT_ALWAYS_CHAR ("powerup.wav");
  powerup = Mix_LoadWAV (file.c_str ());
  if (!powerup)
  {
    ACE_DEBUG ((LM_ERROR,
                ACE_TEXT ("failed to Mix_LoadWAV: \"%s\", aborting\n"),
                ACE_TEXT (Mix_GetError ())));
    ACE_OS::exit (1);
  } // end IF

#if defined (SDL1_USE)
  SDL_WM_SetCaption (ACE_TEXT_ALWAYS_CHAR (WINDOW_CAPTION), NULL);
#endif // SDL1_USE

  file = path_base;
  file += ACE_DIRECTORY_SEPARATOR_STR_A;
  file += ACE_TEXT_ALWAYS_CHAR ("mainfont.ttf");
  font = TTF_OpenFont (file.c_str (), LARGE_FONT_SIZE);
  if (!font)
  {
    ACE_DEBUG ((LM_ERROR,
                ACE_TEXT ("failed to TTF_OpenFont: \"%s\", aborting\n"),
                ACE_TEXT (TTF_GetError ())));
    ACE_OS::exit (1);
  } // end IF

  music = new Music (path_base);

  fps_counter = new FpsCounter (gameFPS);

  game_state = new MenuState ();
  game_state->InitState ();

  if (!music->isMusicOn ())
    SwitchMusic ();
}

Game::~Game ()
{
  delete music;
  delete fps_counter;
  delete game_state;

  closeSystems ();
}

int
Game::initSystems ()
{
  if (SDL_Init (SDL_INIT_EVERYTHING) < 0)
  {
    ACE_DEBUG ((LM_ERROR,
                ACE_TEXT ("failed to SDL_Init: \"%s\", aborting\n"),
                ACE_TEXT (SDL_GetError ())));
    return -1;
  } // end IF
#if defined (SDL1_USE)
  Uint32 flags_i = SDL_DOUBLEBUF;
  flags_i |= SDL_SWSURFACE;
  screen = SDL_SetVideoMode (screen_w, screen_h, 32, flags_i);
#elif defined (SDL2_USE)
  window = SDL_CreateWindow (ACE_TEXT_ALWAYS_CHAR (WINDOW_CAPTION),
                             SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED,
                             screen_w, screen_h,
                             SDL_WINDOW_OPENGL);
  ACE_ASSERT (window);
  screen = SDL_GetWindowSurface (window);

  // renderer = SDL_CreateRenderer (window, -1, 0);
  // ACE_ASSERT (renderer);
  // texture = SDL_CreateTexture (renderer,
  //                              SDL_PIXELFORMAT_ARGB8888,
  //                              SDL_TEXTUREACCESS_STREAMING,
  //                              screen_w, screen_h);
  // ACE_ASSERT (texture);
#endif // SDL2_USE
  ACE_ASSERT (screen);

  if (TTF_Init () < 0)
  {
    ACE_DEBUG ((LM_ERROR,
                ACE_TEXT ("failed to TTF_Init: \"%s\", aborting\n"),
                ACE_TEXT (TTF_GetError ())));
    return -1;
  } // end IF

  if (Mix_OpenAudio (MIX_DEFAULT_FREQUENCY,
                     //SOUNDS_DEF_FREQUENCY,
                     MIX_DEFAULT_FORMAT,
                     MIX_DEFAULT_CHANNELS,
                     1024) < 0)
  {
    ACE_DEBUG ((LM_ERROR,
                ACE_TEXT ("failed to Mix_OpenAudio: \"%s\", aborting\n"),
                ACE_TEXT (Mix_GetError ())));
    return -1;
  } // end IF
  if (Mix_AllocateChannels (SOUNDS_DEF_CHANNELS) != SOUNDS_DEF_CHANNELS)
    ACE_DEBUG ((LM_WARNING,
                ACE_TEXT ("failed to Mix_AllocateChannels(%d): \"%s\", continuing\n"),
                SOUNDS_DEF_CHANNELS,
                ACE_TEXT (Mix_GetError ())));

  int mixer_flags_base_i = MIX_INIT_FLAC | MIX_INIT_MOD;
#if defined (ACE_WIN32) || defined (ACE_WIN64)
  int mixer_flags_i = mixer_flags_base_i | MIX_INIT_MP3;
#else
#if defined (SDL1_USE)
  int mixer_flags_i = mixer_flags_base_i | MIX_INIT_OGG | MIX_INIT_FLUIDSYNTH;
#elif defined (SDL2_USE)
  int mixer_flags_i = mixer_flags_base_i | MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_MID | MIX_INIT_OPUS;
#endif // SDL1_USE || SDL2_USE
#endif // ACE_WIN32 || ACE_WIN64
  int result = Mix_Init (mixer_flags_i);
  if (result != mixer_flags_i)
    ACE_DEBUG ((LM_WARNING,
                ACE_TEXT ("failed to Mix_Init(0x%x): 0x%x: \"%s\", continuing\n"),
                mixer_flags_i, result,
                ACE_TEXT (Mix_GetError ())));
  // debug info
  int total = Mix_GetNumChunkDecoders ();
  for (int i = 0; i < total; i++)
    std::cerr << ACE_TEXT_ALWAYS_CHAR ("Supported chunk decoder: ") <<  Mix_GetChunkDecoder (i) << std::endl;

  return 0;
}

void
Game::closeSystems ()
{
  if (screen)
    SDL_FreeSurface (screen);
  if (font)
    TTF_CloseFont (font);
  Mix_CloseAudio ();
  if (collision)
    Mix_FreeChunk (collision);
  if (laser)
    Mix_FreeChunk (laser);
  if (powerup)
    Mix_FreeChunk (powerup);
  Mix_Quit ();
  TTF_Quit ();
  SDL_Quit ();
}

int
Game::Loop ()
{
  bool render_b = false;
  while (running)
  {
    render_b = fps_counter->measureFPS ();
    if (!render_b)
      continue;

    HandleEvents ();
    SDL_FillRect (screen, NULL, 0);

    if (!paused)
      game_state->UpdateState ();
    game_state->RenderState ();

    if (displayFPS)
    {
      char buffer_a[10] = {0};
      ACE_OS::sprintf (buffer_a,
                       ACE_TEXT_ALWAYS_CHAR ("%d fps"),
                       fps_counter->getFPS ());
#if defined (SDL1_USE)
      SDL_WM_SetCaption (buffer_a, NULL);
#elif defined (SDL2_USE)
      SDL_SetWindowTitle (window, buffer_a);
#endif // SDL1_USE || SDL2_USE
      //fps_counter->RenderFPS (30, 30);
    } // end IF
    else
#if defined (SDL1_USE)
      SDL_WM_SetCaption (ACE_TEXT_ALWAYS_CHAR (WINDOW_CAPTION), NULL);
#elif defined (SDL2_USE)
      SDL_SetWindowTitle (window, ACE_TEXT_ALWAYS_CHAR (WINDOW_CAPTION));
#endif // SDL1_USE || SDL2_USE

#if defined (SDL1_USE)
    SDL_Flip (screen);
#elif defined (SDL2_USE)
    SDL_UpdateWindowSurface (window);
    // SDL_UpdateTexture (texture, NULL, screen->pixels, screen_w * sizeof (Uint32));
    // SDL_RenderCopy (renderer, texture, NULL, NULL);
    // SDL_RenderPresent (renderer);
#endif // SDL1_USE || SDL2_USE
  } // end WHILE

  return 0;
}

void
Game::HandleEvents ()
{
  union SDL_Event event;
  while (SDL_PollEvent (&event))
  {
    if (event.type == SDL_QUIT)
      ShutDown ();

    Uint8* keystates =
#if defined(SDL1_USE)
      SDL_GetKeyState (NULL);
#elif defined(SDL2_USE)
      const_cast<Uint8*> (SDL_GetKeyboardState (NULL));
#endif // SDL1_USE || SDL2_USE

    if (event.type != SDL_KEYDOWN)
      goto continue_;
#if defined (SDL1_USE)
    if (keystates[SDLK_q])
#elif defined (SDL2_USE)
    if (keystates[SDL_SCANCODE_Q])
#endif // SDL1_USE || SDL2_USE
      ShutDown ();

#if defined (SDL1_USE)
    if (keystates[SDLK_p])
#elif defined (SDL2_USE)
    if (keystates[SDL_SCANCODE_P])
#endif // SDL1_USE || SDL2_USE
      paused = !paused;

#if defined (SDL1_USE)
    if (keystates[SDLK_m])
#elif defined (SDL2_USE)
    if (keystates[SDL_SCANCODE_M])
#endif // SDL1_USE || SDL2_USE
      music->next (true);

continue_:
    game_state->HandleEvents (keystates, event, control_type);
  } // end WHILE
}

std::string
Game::GetUserName_ ()
{
  std::string result;

  char buffer_a[BUFSIZ];
  ACE_OS::memset (&buffer_a, 0, sizeof (char[BUFSIZ]));
  char* username = buffer_a;
#if defined(ACE_WIN32) || defined(ACE_WIN64)
  DWORD buffer_size = sizeof (char[BUFSIZ]);
  if (!GetUserNameA (buffer_a, &buffer_size))
    ACE_DEBUG ((LM_ERROR, ACE_TEXT ("failed to GetUserName: %d, continuing\n"),
                GetLastError ()));
#else
  username = ACE_OS::getenv ("USER");
#endif // ACE_WIN32 || ACE_WIN64

  result = username;
  
  return result;
}

Mix_Chunk*
Game::GetSfx (enum SFX sfx_in)
{
  switch (sfx_in)
  {
    case COLLISION:
      return collision;
    case LASER:
      return laser;
    case POWERUP:
      return powerup;
    default:
      break;
  } // end SWITCH

  return NULL;
}

void
ChangeState ()
{
  Game* game = g_GamePtr;
  if (game->current_state == PLAYING)
  {
    game->current_state = MENU;
    delete game->game_state;
    game->game_state = new MenuState ();
    game->game_state->InitState ();
  } // end IF
  else if (game->current_state == MENU)
  {
    game->current_state = PLAYING;
    delete game->game_state;
    game->game_state = new PlayingState ();
    game->game_state->InitState ();
  } // end ELSE IF
}

void
ShutDown ()
{
  g_GamePtr->running = false;
}

void
SwitchControl ()
{
  switch (g_GamePtr->control_type)
  {
    case KEYBOARD:
      g_GamePtr->control_type = MOUSE;
      break;
    case MOUSE:
      g_GamePtr->control_type = KEYBOARD;
      break;
    default:
     break;
  } // end SWITCH
  static_cast<MenuState*> (g_GamePtr->GetState ())->UpdateInfo (CONTROL);
}

void
SwitchSfx ()
{
  g_GamePtr->sfxOn = !g_GamePtr->sfxOn;
  static_cast<MenuState*> (g_GamePtr->GetState ())->UpdateInfo (SOUNDON);
}

void
SwitchFPSVisibility ()
{
  g_GamePtr->displayFPS = !g_GamePtr->displayFPS;
  static_cast<MenuState*> (g_GamePtr->GetState ())->UpdateInfo (SHOWFPS);
}

void
Game::Draw (SDL_Surface* screen,
            SDL_Surface* source,
            int x, int y)
{
  struct SDL_Rect offset = {(Sint16)x, (Sint16)y, 0, 0};
  struct SDL_Rect clip = {0, 0, (Uint16)source->w, (Uint16)source->h};
  SDL_BlitSurface (source, &clip, screen, &offset);
}

std::string
IntToStr (int n)
{
  std::stringstream ss;
  ss << n;
  return ss.str ();
}

void
DisplayFinishText (unsigned int ms, const char* text)
{
  char buffer_a[PATH_MAX];
  ACE_OS::getcwd (buffer_a, sizeof (char[PATH_MAX]));
  std::string path_base = buffer_a;
  path_base += ACE_DIRECTORY_SEPARATOR_STR_A;
  path_base += RESOURCE_DIRECTORY;
  std::string file = path_base;
  file += ACE_DIRECTORY_SEPARATOR_STR_A;
  file += ACE_TEXT_ALWAYS_CHAR ("font.ttf");
  TTF_Font* font = TTF_OpenFont (file.c_str (), HUGE_FONT_SIZE);
  if (!font)
  {
    ACE_DEBUG ((LM_ERROR,
                ACE_TEXT ("failed to TTF_OpenFont: \"%s\", returning\n"),
                ACE_TEXT (TTF_GetError ())));
    return;
  }

  SDL_Surface* text_image = NULL;
  SDL_Surface* text_shade = NULL;
  struct SDL_Color color = {0x2b, 0xd7, 0xb7, 0};
  struct SDL_Color shade = {0xff, 0xff, 0xff, 0};
  text_image = TTF_RenderText_Solid (font, text, color);
  text_shade = TTF_RenderText_Solid (font, text, shade);
  ACE_ASSERT (text_image && text_shade);

  int posX = g_GamePtr->GetScreen_W () / 2;
  int posY = g_GamePtr->GetScreen_H () / 2;
  SDL_Surface* screen_p = g_GamePtr->GetScreen ();
  ACE_ASSERT (screen_p);
  Game::Draw (screen_p, text_shade, posX - text_shade->w / 2 + 2, posY - text_shade->h / 2 + 2);
  Game::Draw (screen_p, text_image, posX - text_image->w / 2,     posY - text_image->h / 2);
#if defined (SDL1_USE)
  SDL_Flip (screen_p);
#elif defined (SDL2_USE)
  SDL_UpdateWindowSurface (g_GamePtr->GetWindow ());
  // SDL_Texture* texture_p = g_GamePtr->GetTexture ();
  // ACE_ASSERT (texture_p);
  // SDL_UpdateTexture (texture_p, NULL, screen_p->pixels, g_GamePtr->GetScreen_W () * sizeof (Uint32));
  // SDL_Renderer* renderer_p = g_GamePtr->GetRenderer ();
  // ACE_ASSERT (renderer_p);
  // SDL_RenderCopy (renderer_p, texture_p, NULL, NULL);
  // SDL_RenderPresent (renderer_p);
#endif // SDL1_USE || SDL2_USE

  unsigned int firstMeasure = SDL_GetTicks ();
  while (SDL_GetTicks () - firstMeasure <= ms);

  SDL_FreeSurface (text_shade);
  SDL_FreeSurface (text_image);
  TTF_CloseFont (font);
}
