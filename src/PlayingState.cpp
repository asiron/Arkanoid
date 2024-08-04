#include "stdafx.h"

#include "PlayingState.h"

#include <fstream>
#include <sstream>
#include <string>

#include "ace/OS.h"
#include "ace/Log_Msg.h"

#include "Ball.h"
#include "Block.h"
#include "defines.h"
#include "Effect.h"
#include "Game.h"
#include "GameObject.h"
#include "Gui.h"
#include "MapLoader.h"
#include "Platform.h"
#include "Projectile.h"

PlayingState::PlayingState ()
 : inherited ()
 , changingstate (false)
 , gobjects ()
 , current_level (ACE_TEXT_ALWAYS_CHAR (DEFAULT_MAP_FILE))
 , map_loader (NULL)
 , balls ()
 , platform (NULL)
 , effects (NULL)
 , projectiles (NULL)
 , gui (NULL)
 , n_ball_flag (DEFAULT_BALLS_MAX, false) // setting-up flag, n wasnt intialized yet
                                          // (i.e. pushed on the list)
 , remove_n_ball (DEFAULT_BALLS_MAX, false) // remove nth ball from list ?
{
  char buffer_a[PATH_MAX];
  ACE_OS::getcwd (buffer_a, sizeof (char[PATH_MAX]));
  std::string path_base = buffer_a;
  path_base += ACE_DIRECTORY_SEPARATOR_STR_A;
  path_base += ACE_TEXT_ALWAYS_CHAR (RESOURCE_DIRECTORY);
  path_base += ACE_DIRECTORY_SEPARATOR_STR_A;
  std::string graphics_base = path_base;
  graphics_base += ACE_TEXT_ALWAYS_CHAR (GRAPHICS_DIRECTORY);
  graphics_base += ACE_DIRECTORY_SEPARATOR_STR_A;
  std::string file = graphics_base;
  file += ACE_TEXT_ALWAYS_CHAR ("platformw.png");
  platform = new Platform (file.c_str (), 3, 0, 66, 18, 4, 0);
  file = graphics_base;
  file += ACE_TEXT_ALWAYS_CHAR ("ball.png");
  for (int i = 0; i < DEFAULT_BALLS_MAX; i++)
    balls.push_back (new Ball (file.c_str (), 0, 1, 16, 16, 1, 1));

  // pushing ball and platform on the list
  gobjects.push_back (platform);
  gobjects.push_back (balls[0]);

  // loading blocks
  file = path_base;
  file += ACE_DIRECTORY_SEPARATOR_STR_A;
  file += ACE_TEXT_ALWAYS_CHAR ("config.cfg");
  map_loader = new MapLoader (file.c_str ());
  file = path_base;
  file += ACE_DIRECTORY_SEPARATOR_STR_A;
  file += current_level;
  gobjects.splice (gobjects.end (), map_loader->LoadMap (file.c_str ()));

  // loading projectiles
  file = graphics_base;
  file += ACE_TEXT_ALWAYS_CHAR ("effect.png");
  projectiles = new Projectile*[DEFAULT_PROJECTILES_MAX]; // Creating 10 projectile pointers as arbitrary value
  for (int i = 0; i < DEFAULT_PROJECTILES_MAX; i++)
  {
    projectiles[i] = new Projectile (file.c_str (), 23, 4, 16, 14, 24, 1);
    gobjects.push_back (projectiles[i]);
  }

  effects = new Effect*[DEFAULT_EFFECTS_MAX];
  file = graphics_base;
  file += ACE_TEXT_ALWAYS_CHAR ("effect.png");  
  effects[0] = new Effect (file.c_str (), 23, 4, 16, 14, 24, 1);
  effects[0]->SetEffectType (GUN);
  file = graphics_base;
  file += ACE_TEXT_ALWAYS_CHAR ("effect2.png");
  effects[1] = new Effect (file.c_str (), 10, 7, 34, 29, 11, 1);
  effects[1]->SetEffectType (MAGNET);
  file = graphics_base;
  file += ACE_TEXT_ALWAYS_CHAR ("effect3.png");
  effects[2] = new Effect (file.c_str (), 59, 1, 60, 60, 8, 1);
  effects[2]->SetEffectType (SECONDBALL);
  file = graphics_base;
  file += ACE_TEXT_ALWAYS_CHAR ("effect2.png");
  effects[3] = new Effect (file.c_str (), 10, 7, 34, 29, 11, 1);
  effects[3]->SetEffectType (LARGE);

  // we push effects as last elem. beacause they have to be rendered as last objs
  for (int i = 0; i < DEFAULT_EFFECTS_MAX; i++)
    gobjects.push_back (effects[i]);

  gui = new Gui ();
  gui->Init (g_GamePtr->GetScreen_W () - 135, g_GamePtr->GetScreen_H () - 52);
}

PlayingState::~PlayingState ()
{
  delete map_loader;
  for (std::list<GameObject*>::iterator iter = gobjects.begin (); iter != gobjects.end (); iter++)
    (*iter)->Destroy ();
  delete [] projectiles;
  delete [] effects;
  delete gui;

  SaveHighscores (); // save list of highscores to file
}

void
PlayingState::RenderState ()
{
  for (std::list<GameObject*>::iterator iter = gobjects.begin (); iter != gobjects.end (); iter++)
    (*iter)->Render ();

  gui->Render ();

  if (changingstate)
  {
    DisplayFinishText (3000, levelcomplete ? ACE_TEXT_ALWAYS_CHAR ("Level complete") : ACE_TEXT_ALWAYS_CHAR ("Game over"));
    changingstate = false;
    if (levelcomplete)
    {
      if (!LoadNextMap ())
      {
        PushScore (g_GamePtr->GetUserName_ (), platform->GetScore ());
        ChangeState (); // *TODO*: completed last level...
      } // end IF
    } // end IF
    else
      ChangeState ();
  } // end IF
}

void
PlayingState::UpdateState ()
{
  for (std::list<GameObject*>::iterator iter = gobjects.begin (); iter != gobjects.end (); iter++)
    for (std::list<GameObject*>::iterator iter2 = gobjects.begin (); iter2 != gobjects.end (); iter2++)
      (*iter)->Collided ((*iter2)->GetID (), (*iter)->detectCollision (*iter2));

  gui->Update ();

  for (std::list<GameObject*>::iterator iter = gobjects.begin (); iter != gobjects.end (); iter++)
    if ((*iter)->Update () == -1)
      break;  // if an update function returns -1 then we break from loop because we are about to change state

  // remove nth ball from rendered objects ?
  for (int i = 1; i < DEFAULT_BALLS_MAX; i++)
    if (remove_n_ball[i])
    {
      remove_n_ball[i] = false;

      ACE_ASSERT (n_ball_flag[i]);
      std::list<GameObject*>::iterator iter_2 = gobjects.end ();
      for (std::list<GameObject*>::iterator iter = gobjects.begin (); iter != gobjects.end (); iter++)
        if (*iter == static_cast<GameObject*> (balls[i]))
        {
          iter_2 = iter;
          break;
        } // end IF
      ACE_ASSERT (iter_2 != gobjects.end ());
      gobjects.erase (iter_2);
      n_ball_flag[i] = false;
    } // end IF

  // test: level complete ?
  levelcomplete = true;
  for (std::list<GameObject*>::iterator iter = gobjects.begin (); iter != gobjects.end (); iter++)
  {
    if ((*iter)->GetID () != BLOCK)
      continue;
    Block* block_p = static_cast<Block*> (*iter);
    if (block_p->GetHealth () > 0)
    {
      levelcomplete = false;
      break;
    } // end IF
  } // end FOR
  if (levelcomplete)
    changingstate = true;
}

void
PlayingState::HandleEvents (Uint8* keystates, const SDL_Event& event, int control_type)
{
#if defined (SDL1_USE)
  if (keystates[SDLK_ESCAPE])
#elif defined (SDL2_USE)
  if (keystates[SDL_SCANCODE_ESCAPE])
#endif // SDL1_USE || SDL2_USE
  {
    ChangeState ();
    return;
  }

  // *TODO*: remove these cheats
#if defined (_DEBUG)
  bool is_powerup_b = false;
  if (event.type != SDL_KEYDOWN)
    goto continue_;
  if (event.key.keysym.sym == SDLK_n)
    LoadNextMap ();
  else if (event.key.keysym.sym == SDLK_F1)
  {
    platform->MorphPlatform (MAGNET);
    for (int i = 0; i < DEFAULT_BALLS_MAX; i++)
      balls[i]->MorphBall (MAGNET);

    is_powerup_b = true;
  }
  else if (event.key.keysym.sym == SDLK_F2)
  {
    platform->MorphPlatform (GUN);
    for (int i = 0; i < DEFAULT_BALLS_MAX; i++)
      balls[i]->LoseEffect ();

    // *NOTE*: this ensures that one cannot simply clear the level when
    //         picking up a gun when no ball(s) is(/are) in play...
    if (!balls[0]->isAlive ())
      balls[0]->StartFlying ();
    for (int i = 1; i < DEFAULT_BALLS_MAX; i++)
      if (balls[i]->isAlive ())
        balls[i]->StartFlying ();

    is_powerup_b = true;
  }
  else if (event.key.keysym.sym == SDLK_F3)
  {
    LaunchAdditionalBall ();
    platform->MorphPlatform (SECONDBALL);
    for (int i = 0; i < DEFAULT_BALLS_MAX; i++)
      balls[i]->LoseEffect ();

    is_powerup_b = true;
  }
  else if (event.key.keysym.sym == SDLK_F4)
  {
    platform->MorphPlatform (LARGE);
    for (int i = 0; i < DEFAULT_BALLS_MAX; i++)
      balls[i]->LoseEffect ();
 
    is_powerup_b = true;
  }
  else if (event.key.keysym.sym == SDLK_F10)
    platform->AddLife ();
continue_:
  if (is_powerup_b && g_GamePtr->isSfxOn ())
    Mix_PlayChannel (-1, g_GamePtr->GetSfx (POWERUP), 0);
#endif // _DEBUG

  // Movement controls with keyboard
  if (control_type == KEYBOARD)
  {
#if defined (SDL1_USE)
    if (keystates[SDLK_SPACE])
#elif defined (SDL2_USE)
    if (keystates[SDL_SCANCODE_SPACE])
#endif // SDL1_USE || SDL2_USE
    {
      if (!balls[0]->isAlive () || balls[0]->IsOnPlatform ())
        balls[0]->StartFlying ();
      else
        for (int i = 1; i < DEFAULT_BALLS_MAX; i++)
          if (balls[i]->isAlive () && balls[i]->IsOnPlatform ())
          {
            balls[i]->StartFlying ();
            break;
          } // end IF
    } // end IF

#if defined (SDL1_USE)
    if (keystates[SDLK_LEFT])
#elif defined (SDL2_USE)
    if (keystates[SDL_SCANCODE_LEFT])
#endif // SDL1_USE || SDL2_USE
      platform->MoveLeft ();
#if defined (SDL1_USE)
    else if (keystates[SDLK_RIGHT])
#elif defined (SDL2_USE)
    else if (keystates[SDL_SCANCODE_RIGHT])
#endif // SDL1_USE || SDL2_USE
      platform->MoveRight ();
    else
      platform->StopMoving ();

#if defined (SDL1_USE)
    if (keystates[SDLK_x])
#elif defined (SDL2_USE)
    if (keystates[SDL_SCANCODE_X])
#endif // SDL1_USE || SDL2_USE
      platform->Shoot ();
  } // end IF
  else if (control_type == MOUSE)
  {
    int x; // mouse x coordinate position
    SDL_GetMouseState (&x, NULL);
    if (x - 10 > platform->GetX ())
      platform->MoveRight ();
    else if (x + 10 < platform->GetX ())
      platform->MoveLeft ();
    else
      platform->StopMoving ();

    if (event.button.type == SDL_MOUSEBUTTONDOWN)
    {
      if (event.button.button == SDL_BUTTON_LEFT)
        platform->Shoot ();
      else if (event.button.button == SDL_BUTTON_MIDDLE)
      {
        if (!balls[0]->isAlive () || balls[0]->IsOnPlatform ())
          balls[0]->StartFlying ();
        else
          for (int i = 1; i < DEFAULT_BALLS_MAX; i++)
            if (balls[i]->isAlive () && balls[i]->IsOnPlatform ())
            {
              balls[i]->StartFlying ();
              break;
            } // end IF
      } // end ELSE IF
    } // end IF
  } // end ELSE IF
}

void
PlayingState::InitState ()
{
  for (std::list<GameObject*>::iterator iter = gobjects.begin (); iter != gobjects.end (); iter++)
    (*iter)->Init ();
}

void
PlayingState::SaveHighscores ()
{
  char buffer_a[PATH_MAX];
  ACE_OS::getcwd (buffer_a, sizeof (char[PATH_MAX]));
  std::string path_base = buffer_a;
  path_base += ACE_DIRECTORY_SEPARATOR_STR_A;
  path_base += RESOURCE_DIRECTORY;
  path_base += ACE_DIRECTORY_SEPARATOR_STR_A;
  std::string filename = path_base + ACE_TEXT_ALWAYS_CHAR ("highscores");
  std::ofstream file;
  file.open (filename.c_str (), ios::binary);

  for (std::list<std::pair<std::string, int> >::iterator iter = highsco_list.begin (); iter != highsco_list.end (); iter++)
    file << iter->first << ACE_TEXT_ALWAYS_CHAR (", ") << iter->second << ACE_TEXT_ALWAYS_CHAR ("\r\n"); // write windows(TM)-style newlines

  file.close ();
}

unsigned int
PlayingState::CurrentLevel ()
{
  unsigned int result = 0;

  std::string::size_type position = current_level.find ('.', 0);
  ACE_ASSERT (position != std::string::npos);
  std::string level_string = current_level;
  level_string.erase (position, std::string::npos); // remove trailing ".cfg"
  level_string.erase (0, 3); // remove leading "map"
  std::stringstream converter;
  converter << level_string;
  converter >> result;

  return result;
}

bool
PlayingState::LoadNextMap ()
{
  bool result = true;

  // step1: remove additional balls from rendered objects
  std::list<GameObject*>::iterator iter_2;
  for (int i = 1; i < DEFAULT_BALLS_MAX; i++)
  {
    iter_2 = gobjects.end ();
    for (std::list<GameObject*>::iterator iter = gobjects.begin (); iter != gobjects.end (); iter++)
      if (*iter == static_cast<GameObject*> (balls[i]))
      {
        iter_2 = iter;
        break;
      } // end IF
    if (iter_2 != gobjects.end ())
      gobjects.erase (iter_2);

    n_ball_flag[i] = false;
    remove_n_ball[i] = false;
  } // end FOR

  // step2: throw out all (dead) blocks
  gobjects.erase (std::remove_if (gobjects.begin (), 
                                  gobjects.end (),
                                  [](GameObject* object_in) { return object_in->GetID () == BLOCK; }),
                  gobjects.end ());

  // step3: load the next set of blocks and insert to just before the first projectile
  iter_2 = gobjects.end ();
  for (std::list<GameObject*>::iterator iter = gobjects.begin (); iter != gobjects.end (); iter++)
    if ((*iter)->GetID () == PROJECTILE)
    {
      iter_2 = iter;
      break;
    } // end IF
  ACE_ASSERT (iter_2 != gobjects.end ());
  char buffer_a[PATH_MAX];
  ACE_OS::getcwd (buffer_a, sizeof (char[PATH_MAX]));
  std::string path_base = buffer_a;
  path_base += ACE_DIRECTORY_SEPARATOR_STR_A;
  path_base += ACE_TEXT_ALWAYS_CHAR (RESOURCE_DIRECTORY);
  std::string file = path_base;
  file += ACE_DIRECTORY_SEPARATOR_STR_A;
  // *NOTE*: the maps are enumerated and have the format (map[#level].cfg)
  unsigned int level_i = CurrentLevel ();
  if (!level_i)
    level_i++;
  level_i++;
  std::stringstream converter;
  converter << level_i;
  std::string level_string;
  level_string += ACE_TEXT_ALWAYS_CHAR ("map");
  level_string += converter.str ();
  level_string += ACE_TEXT_ALWAYS_CHAR (".cfg");
  file += level_string;
  map_t blocks_a = map_loader->LoadMap (file.c_str ());
  if (blocks_a.empty ())
  {
    ACE_DEBUG ((LM_WARNING,
                ACE_TEXT ("failed to MapLoader::LoadMap(\"%s\"), aborting\n"),
                ACE_TEXT (file.c_str ())));
    result = false;
  } // end IF
  else
    current_level = level_string;
  gobjects.splice (iter_2, blocks_a);

  // step4: deactivate any nth ball, projectiles and effects
  for (int i = 1; i < DEFAULT_BALLS_MAX; i++)
    balls[i]->SetAlive (false);
  for (std::list<GameObject*>::iterator iter = gobjects.begin (); iter != gobjects.end (); iter++)
    if ((*iter)->GetID () == PROJECTILE)
      (*iter)->SetAlive (false);
    else if ((*iter)->GetID () == EFFECT)
      (*iter)->SetAlive (false);

  // step5: deactivate any effects
  for (int i = 0; i < DEFAULT_BALLS_MAX; i++)
    balls[i]->LoseEffect ();
  platform->MorphPlatform (-1);

  // step6: reset the ball
  balls[0]->SetAlive (false);

  return result;
}

void
PlayingState::LaunchAdditionalBall ()
{
  for (int i = 1; i < DEFAULT_BALLS_MAX; i++)
    if (!balls[i]->isAlive ())
    {
      int t_dirX = (ACE_OS::rand_r (g_GamePtr->GetRandomSeedPtr ()) % 2 + 1) * 2 - 3;
      int t_dirY = (ACE_OS::rand_r (g_GamePtr->GetRandomSeedPtr ()) % 2 + 1) * 2 - 3;
      balls[i]->GameObject::Init (balls[0]->GetX (), balls[0]->GetY (),
                                  static_cast<float> (ACE_OS::rand_r (g_GamePtr->GetRandomSeedPtr ()) % 2 + 3), static_cast<float> (ACE_OS::rand_r (g_GamePtr->GetRandomSeedPtr ()) % 2 + 3),
                                  t_dirX, t_dirY,
                                  balls[0]->GetBoundX (), balls[0]->GetBoundY ());
      balls[i]->SetAlive (true);

      if (!n_ball_flag[i]) // already in gobjects ?
      {
        // insert after balls[0]
        std::list<GameObject*>::iterator iter_2 = gobjects.end ();
        for (std::list<GameObject*>::iterator iter = gobjects.begin (); iter != gobjects.end (); iter++)
          if (*iter == static_cast<GameObject*> (balls[0]))
          {
            iter_2 = iter;
            break;
          } // end IF
        ACE_ASSERT (iter_2 != gobjects.end ());
        gobjects.insert (iter_2, balls[i]);
        n_ball_flag[i] = true;
      } // end IF
      break; // launch a single ball only
    } // end IF
}

void
PlayingState::RemoveAdditionalBall (Ball* ball_in)
{
  int n = -1;
  for (int i = 1; i < DEFAULT_BALLS_MAX; i++)
    if (balls[i] == ball_in)
    {
      n = i;
      break;
    } // end IF
  ACE_ASSERT (n != -1);

  // *WARNING*: cannot manipulate gobjects here (it's currently being iterated over !)
  remove_n_ball[n] = true;
}

void
PlayingState::SwitchBalls (int n)
{ ACE_ASSERT (!balls[0]->isAlive () && balls[n]->isAlive ());

  Ball* temp_p = balls[n];
  balls[n] = balls[0];
  balls[0] = temp_p;

  // *WARNING*: cannot manipulate gobjects here (it's currently being iterated over !)
  remove_n_ball[n] = true;
}
