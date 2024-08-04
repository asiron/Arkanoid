#include "stdafx.h"

#include "Effect.h"

#include "ace/OS.h"

#include "Animation.h"
#include "Ball.h"
#include "defines.h"
#include "Game.h"
#include "Platform.h"
#include "PlayingState.h"

Effect::Effect (const char* filename,
                int maxFrame,
                int frameDelay,
                int frameWidth,
                int frameHeight,
                int animationColumns,
                int animationDirection )
 : GameObject (filename,
               maxFrame,
               frameDelay,
               frameWidth,
               frameHeight,
               animationColumns,
               animationDirection)
{
  SetID (EFFECT);
}

void
Effect::Destroy ()
{
  GameObject::Destroy ();
}

void
Effect::Init (int x, int y)
{
  GameObject::Init (static_cast<float> (x), static_cast<float> (y),
                    0.0f, static_cast<float> (ACE_OS::rand_r (g_GamePtr->GetRandomSeedPtr ()) % 2 + 1),
                    0, 1,
                    animation->GetFrameWidth () / 2.0f, animation->GetFrameHeight () / 2.0f);
  SetAlive (true);
}

int
Effect::Update ()
{
  if (isAlive ())
  {
    GameObject::Update ();

    if (static_cast<int> (y) >= g_Game.GetScreen_H ())
      SetAlive (false);
    else if (animation)
      animation->Animate ();
  } // end IF

  return 0;
}

void
Effect::Render ()
{
  if (isAlive ())
  {
    GameObject::Render ();
    if (animation)
      animation->Draw (x - boundX, y - boundY);
  }
}

void
Effect::Collided (int ObjectID, enum col_dir dir)
{
  if (dir == NO_COLLISION)
    return;

  if (ObjectID == PLAYER)
  {
    if (effect_type == MAGNET)
    {
      PlayingState* ps = static_cast<PlayingState*> (g_GamePtr->GetState ());
      ps->GetPlatform ()->MorphPlatform (effect_type);
      std::vector<Ball*>& balls_r = ps->GetBalls ();
      for (int i = 0; i < DEFAULT_BALLS_MAX; i++)
        balls_r[i]->MorphBall (effect_type);
      SetAlive (false);
    }
    else if (effect_type == GUN)
    {
      PlayingState* ps = static_cast<PlayingState*> (g_GamePtr->GetState ());
      ps->GetPlatform ()->MorphPlatform (effect_type);
      std::vector<Ball*>& balls_r = ps->GetBalls ();
      for (int i = 0; i < DEFAULT_BALLS_MAX; i++)
        balls_r[i]->LoseEffect ();
      SetAlive (false);

      // *NOTE*: this ensures that one cannot simply clear the level when
      //         picking up a gun when no ball(s) is(/are) in play...
      if (!balls_r[0]->isAlive ())
        balls_r[0]->StartFlying ();
      for (int i = 1; i < DEFAULT_BALLS_MAX; i++)
        if (balls_r[i]->isAlive ())
          balls_r[i]->StartFlying ();
    }
    else if (effect_type == SECONDBALL)
    {
      PlayingState* ps = static_cast<PlayingState*> (g_GamePtr->GetState ());
      ps->LaunchAdditionalBall ();
      ps->GetPlatform ()->MorphPlatform (effect_type);
      std::vector<Ball*>& balls_r = ps->GetBalls ();
      for (int i = 0; i < DEFAULT_BALLS_MAX; i++)
        balls_r[i]->LoseEffect ();
      SetAlive (false);
    }
    else if (effect_type == LARGE)
    {
      PlayingState* ps = static_cast<PlayingState*> (g_GamePtr->GetState ());
      ps->GetPlatform ()->MorphPlatform (effect_type);
      std::vector<Ball*>& balls_r = ps->GetBalls ();
      for (int i = 0; i < DEFAULT_BALLS_MAX; i++)
        balls_r[i]->LoseEffect ();
      SetAlive (false);
    } 

    if (g_GamePtr->isSfxOn ())
      Mix_PlayChannel (-1, g_GamePtr->GetSfx (POWERUP), 0);
  }
}
