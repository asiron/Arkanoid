#include "stdafx.h"

#include "Block.h"

#include "ace/OS.h"

#include "Animation.h"
#include "defines.h"
#include "Game.h"
#include "Effect.h"
#include "Platform.h"
#include "PlayingState.h"

Block::Block (const char* filename,
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
 , health (0)
 , maxhealth (0)
{
  //setting ID to PLAYER and calling superclass constructor
  SetID (BLOCK);
}

Block::Block (SDL_Surface* image,
              int maxFrame,
              int frameDelay,
              int frameWidth,
              int frameHeight,
              int animationColumns,
              int animationDirection)
 : GameObject (image,
               maxFrame,
               frameDelay,
               frameWidth,
               frameHeight,
               animationColumns,
               animationDirection)
 , health (0)
 , maxhealth (0)
{
  //setting ID to PLAYER and calling superclass constructor
  SetID (BLOCK);
}

void
Block::Destroy ()
{
  //calling superclass destructor
  GameObject::Destroy ();
}

void
Block::Init (float x,
             float y,
             int speed,
             int dirX,
             int health_in)
{
  // Initializing block
  GameObject::Init (x, y,
                    static_cast<float> (speed), 0.0f,
                    dirX, 0,
                    animation->GetFrameWidth () / 2.0f, animation->GetFrameHeight () / 2.0f);
  SetAlive (true);

  health = health_in;
  maxhealth = health_in;
}

int
Block::Update ()
{
  if (isAlive ())
  {
    GameObject::Update ();

    // performing boundary checking
    if (x > g_Game.GetScreen_W () - boundX || x < boundX)
      dirX *= -1;

    // updating of animation exists
    if (animation)
      animation->Animate ();
  }

  return 0;
}

void
Block::Render ()
{
  if (isAlive ())
  {
    GameObject::Render ();

    //Rendering if animation exists
    if (animation)
      animation->Draw (x - boundX, y - boundY);
  }
}

void
Block::Collided (int objectID, enum col_dir dir)
{
  if (dir == NO_COLLISION)
    return;

  if (objectID == BALL || objectID == PROJECTILE)
  {
    --health;
    if (health == 0)
    {
      PlayingState* playing_state_p = static_cast<PlayingState*> (g_GamePtr->GetState ());
      Effect** effs = playing_state_p->GetEffects ();
      bool all_effects_are_alive_b = true;
      for (int i = 0; i < DEFAULT_EFFECTS_MAX; i++)
        if (!effs[i]->isAlive ())
        {
          all_effects_are_alive_b = false;
          break;
        } // end IF
      int index_i;
      if ((ACE_OS::rand_r (g_GamePtr->GetRandomSeedPtr ()) % 2) &&  // 50% of the time
          !all_effects_are_alive_b) // spawn an effect ?
      {
        // spawn first found 'dead' effect
        while (true)
        {
          index_i = ACE_OS::rand_r (g_GamePtr->GetRandomSeedPtr ()) % DEFAULT_EFFECTS_MAX;
          if (!effs[index_i]->isAlive ())
          {
            effs[index_i]->Init (static_cast<int> (x), static_cast<int> (y));
            break;
          } // end IF
        } // end WHILE
      } // end IF

      Platform* platform = playing_state_p->GetPlatform ();
      platform->AddPoint ();

      SetAlive (false);
    }
    else if (animation->IsAutoAnimation ())
      animation->SetFrame (maxhealth - health);

    if (g_GamePtr->isSfxOn ())
      Mix_PlayChannel (-1, g_GamePtr->GetSfx (COLLISION), 0);
  }
  else if (objectID == BLOCK)
  {
    if (dir == LEFT)
      dirX = -1;
    else if (dir == RIGHT)
      dirX = 1;
  }
}
