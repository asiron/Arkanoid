#include "stdafx.h"

#include "Ball.h"

#include "ace/OS_NS_stdlib.h"

#include "Animation.h"
#include "defines.h"
#include "Game.h"
#include "Platform.h"
#include "PlayingState.h"

Ball::Ball (const char* filename,
            int maxFrame,
            int frameDelay,
            int frameWidth,
            int frameHeight,
            int animationColumns,
            int animationDirection)
 : inherited (filename,
              maxFrame,
              frameDelay,
              frameWidth,
              frameHeight,
              animationColumns,
              animationDirection)
 , has_effect (-1)
 , stand_on_platform (false)
 , launching (false)
{
  // setting ID and calling superclass constructor
  SetID (BALL);
}

void
Ball::Destroy ()
{
  // object destructor calling superclass destructor
  inherited::Destroy ();
}

void
Ball::Init ()
{
  // start from platorm
  Platform* platform = static_cast<PlayingState*> (g_GamePtr->GetState ())->GetPlatform ();
  float posX = platform->GetX ();
  float posY = platform->GetY ();
  int t_dirX = (ACE_OS::rand_r (g_GamePtr->GetRandomSeedPtr ()) % 2 + 1) * 2 - 3; //  picking random direction either left or right
  inherited::Init (posX, posY - (animation->GetFrameHeight () / 2.0f),
                   static_cast<float> (ACE_OS::rand_r (g_GamePtr->GetRandomSeedPtr ()) % 1 + 3), static_cast<float> (ACE_OS::rand_r (g_GamePtr->GetRandomSeedPtr ()) % 1 + 3),
                   t_dirX, -1,
                   animation->GetFrameWidth () / 2.0f, animation->GetFrameHeight () / 2.0f);
}

void
Ball::Render ()
{
  // if object is alive we draw it 
  if (isAlive ())
  {
    inherited::Render ();
    // If there is an animation, we draw it so that the centre of it is at (x,y)
    if (animation)
      animation->Draw (x - boundX, y - boundY);
  }
}

int
Ball::Update ()
{
  // if object is alive we update it
  if (!isAlive ())
    return 0;

  if (stand_on_platform)
  {
    Platform* platform = static_cast<PlayingState*> (g_GamePtr->GetState ())->GetPlatform ();
    x = platform->GetX ();
    y = platform->GetY () - 21.0f;
  } // end IF
  else
  {
    inherited::Update ();

    if (launching && !IsOnPlatform ())
      launching = false;

    // check boundaries
    if ((static_cast<int> (x) >= (g_Game.GetScreen_W () - static_cast<int> (boundX))) ||
        (x <= boundX))
      dirX *= -1;
    if (y <= boundY)
      dirY *= -1;
    if (static_cast<int> (y) >= g_Game.GetScreen_H ())
    {
      SetAlive (false);
      LoseEffect ();

      PlayingState* playing_state_p = static_cast<PlayingState*> (g_GamePtr->GetState ());
      ACE_ASSERT (playing_state_p);
      std::vector<Ball*>& balls_r = playing_state_p->GetBalls ();
      if (this != balls_r[0])
      {
        playing_state_p->RemoveAdditionalBall (this);
        return 0; // nth ball now "dead"
      } // end IF

      ACE_ASSERT (this == balls_r[0]);
      // --> was balls[0]

      int n = -1;
      for (int i = 1; i < DEFAULT_BALLS_MAX; i++)
        if (balls_r[i]->isAlive ())
        {
          n = i;
          break;
        } // end IF
      if (n != -1)
        playing_state_p->SwitchBalls (n);
      else
      {
        Platform* platform = playing_state_p->GetPlatform ();
        platform->LoseLife ();
        platform->MorphPlatform (-1); // calling platform to lose it's effect because (last) ball has just died
      } // end ELSE
    } // end IF
  } // end ELSE

  // we also update its animation if it exists
  if (animation)
    animation->Animate ();

  return 0;
}

void
Ball::LoseEffect ()
{
  has_effect = -1;
  
  stand_on_platform = false;
  while (IsOnPlatform ())
    Update (); //*NOTE*: make sure that it will 'escape' platform
}

bool
Ball::IsOnPlatform ()
{
  // sanity check(s): initial situation ?
  Platform* platform = static_cast<PlayingState*> (g_GamePtr->GetState ())->GetPlatform ();
  return (platform->detectCollision (this) != NO_COLLISION);
}

void
Ball::StartFlying ()
{
  // starting the ball, if it's not alive we dont allow for starting it again, until it dies
  if (!isAlive ())
  {
    launching = true;
    SetAlive (true);
    Init ();
  }
  else if (stand_on_platform)
  {
    stand_on_platform = false;

    // slow down
    float sign_f = ACE_OS::rand_r (g_GamePtr->GetRandomSeedPtr ()) % 2 ? -1.0f : 1.0f;
    velX = sign_f * static_cast<float> (ACE_OS::rand_r (g_GamePtr->GetRandomSeedPtr ()) % 1 + 3);
    dirY = -1;
    velY = static_cast<float> (ACE_OS::rand_r (g_GamePtr->GetRandomSeedPtr ()) % 1 + 3);

    while (IsOnPlatform ())
      Update (); //*NOTE*: make sure that it will 'escape' platform
  }
}

void
Ball::Collided (int ObjectID, enum col_dir dir)
{
  if (dir == NO_COLLISION)
    return;

  // checking for collision with PLAYER
  if (ObjectID == PLAYER)
  {
    switch (dir)
    {
      case LEFT:
        dirX = -1;
        break;
      case RIGHT:
        dirX = 1;
        break;
      case TOP:
        dirY = -1;
        break;
      case TLCOR:
        dirX = -1;
        dirY = -1;
        break;
      case TRCOR:
        dirX = 1;
        dirY = -1;
        break;
      default:
        break;
    } // end SWITCH

    if (has_effect == MAGNET && stand_on_platform == false)
    {
      // award point once (on arrival)
      static_cast<PlayingState*> (g_GamePtr->GetState ())->GetPlatform ()->AddPoint ();

      if (g_GamePtr->isSfxOn ())
        Mix_PlayChannel (-1, g_GamePtr->GetSfx (COLLISION), 0);

      launching = true;
      stand_on_platform = true;
    }

    if (!stand_on_platform)
    {
      if (velX < 0)
        velX -= BALL_VELOCITY_INCREMENT;
      else
        velX += BALL_VELOCITY_INCREMENT;
      if (velY < 0)
        velY -= BALL_VELOCITY_INCREMENT;
      else
        velY += BALL_VELOCITY_INCREMENT;
    } // end IF

    if (!launching && !stand_on_platform)
    {
      static_cast<PlayingState*> (g_GamePtr->GetState ())->GetPlatform ()->AddPoint ();

      if (g_GamePtr->isSfxOn ())
        Mix_PlayChannel (-1, g_GamePtr->GetSfx (COLLISION), 0);
    } // end IF
  }
  else if (ObjectID == BLOCK)
  {
    switch (dir)
    {
      case LEFT:
        dirX = -1;
        break;
      case RIGHT:
        dirX = 1;
        break;
      case TOP:
        dirY = -1;
        break;
      case BOTTOM:
        dirY = 1;
        break;
      case TLCOR:
        dirX = -1;
        dirY = -1;
        break;
      case TRCOR:
        dirX = 1;
        dirY = -1;
        break;
      case BLCOR:
        dirX = -1;
        dirY = 1;
        break;
      case BRCOR:
        dirX = 1;
        dirY = 1;
        break;
      default:
        break;
    } // end SWITCH
  } // end ELSE IF
}
