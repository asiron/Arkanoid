#include "stdafx.h"

#include "Platform.h"

#include "ace/OS.h"
#include "ace/Log_Msg.h"

#include "Animation.h"
#include "Ball.h"
#include "defines.h"
#include "Game.h"
#include "Projectile.h"
#include "PlayingState.h"
#include "scaler.h"

Platform::Platform (const char* filename,
                    int maxFrame,
                    int frameDelay,
                    int frameWidth,
                    int frameHeight,
                    int animationColumns,
                    int animationDirection)
 : GameObject (filename,
               maxFrame,
               frameDelay,
               frameWidth,
               frameHeight,
               animationColumns,
               animationDirection)
 , lives (PLATFORM_DEFAULT_LIVES)
 , score (0)
 , has_effect (-1)
 , temp_bound_x (boundX)
 , temp_image (NULL)
{
  //setting ID to PLAYER and calling superclass constructor
  SetID (PLAYER);
}

void
Platform::Destroy ()
{
  //calling superclass destructor
  GameObject::Destroy ();
}

void
Platform::Init ()
{
  // Initializing platform
  GameObject::Init (g_Game.GetScreen_W () / 2.0f, static_cast<float> (g_Game.GetScreen_H ()) - 10.0f,
                    PLATFORM_BASE_SPEED, 0.0f,
                    0, 0,
                    animation->GetFrameWidth () / 2.0f, animation->GetFrameHeight () / 2.0f);
  SetAlive (true);

  lives = PLATFORM_DEFAULT_LIVES;
  score = 0;
}

int
Platform::Update ()
{
  if (isAlive ())
  {
    GameObject::Update ();
    // boundary checking
    if (x > g_Game.GetScreen_W () - boundX)
      x = g_Game.GetScreen_W () - boundX;
    else if (x < boundX)
      x = boundX;

    // updating if animation exists
    if (animation)
      animation->Animate ();
  } // end IF

  if (!lives)
  {
    PlayingState* playing_state = static_cast<PlayingState*> (g_GamePtr->GetState ());
    playing_state->PushScore (g_GamePtr->GetUserName_ (), score);
    playing_state->SetChangingStateFlag (true); // *TODO*: show highscores next
  } // end IF

  return 0;
}

void
Platform::Render ()
{
  if (isAlive ())
  {
    GameObject::Render ();

    // render if animation exists
    if (animation)
      animation->Draw (x - boundX, y - boundY);
  }
}

void
Platform::Collided (int objectID, enum col_dir dir)
{
  if (dir == NO_COLLISION)
    return;

}

void
Platform::Shoot ()
{
  if (has_effect == GUN)
  {
    Projectile** projectiles_temp =
      static_cast<PlayingState*> (g_GamePtr->GetState ())->GetProjectiles ();
    for (int i = 0; i < DEFAULT_PROJECTILES_MAX; i++)
      if (!projectiles_temp[i]->isAlive ())
      {
        projectiles_temp[i]->Init (static_cast<int> (x), static_cast<int> (y),
                                   DEFAULT_PROJECTILES_SPEED);

        if (g_GamePtr->isSfxOn ())
          Mix_PlayChannel (-1, g_GamePtr->GetSfx (LASER), 0);

        break;
      } // end IF
  } // end IF
}

void
Platform::MorphPlatform (int effect_type)
{
  static int frame_width_i = 0; // *TODO*: turn this into a member

  if (has_effect == LARGE && effect_type != LARGE)
  {
    boundX = temp_bound_x;
    SDL_FreeSurface (animation->GetImage ());
    animation->SetImage (temp_image, frame_width_i); temp_image = NULL;
  } // end IF

  has_effect = effect_type;
  animation->SetFrame (effect_type + 1);

  if (has_effect == LARGE)
  {
    temp_bound_x = boundX;
    boundX *= 2.0f;
    temp_image = animation->GetImage ();
    
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

    struct SDL_Rect rect_s = { 0, 0, 66, 18 };
    SDL_Surface* temp = LoadCroppedBitmap (file.c_str (), rect_s);
    ACE_ASSERT (temp);
    frame_width_i = static_cast<int>  (g_Game.GetScreen_W () / static_cast<float> (BASE_SCREEN_X) * 66);
    int frameHeight = static_cast<int> (g_Game.GetScreen_H () / static_cast<float> (BASE_SCREEN_Y) * 18);
    SDL_Surface* temp2 = ScaleSurface (temp, frame_width_i * 2, frameHeight);
    ACE_ASSERT (temp2);
    SDL_FreeSurface (temp); temp = NULL;

    animation->SetImage (temp2, frame_width_i * 2); temp2 = NULL;
  } // end IF
}
