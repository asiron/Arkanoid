#include "stdafx.h"

#include "Projectile.h"

#include "Animation.h"
#include "Game.h"
#include "Platform.h"
#include "PlayingState.h"

Projectile::Projectile (const char* filename,
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
{
  SetID (PROJECTILE);
}

void
Projectile::Destroy ()
{
  GameObject::Destroy ();
}

void
Projectile::Init (int x, int y, int velY)
{
  GameObject::Init (static_cast<float> (x), static_cast<float> (y),
                    0.0f, static_cast<float> (velY),
                    0, -1,
                    animation->GetFrameWidth () / 2.0f, animation->GetFrameHeight () / 2.0f);
  SetAlive (true);
}

int
Projectile::Update ()
{
  if (isAlive ())
  {
    GameObject::Update ();

    if (y <= boundY)
      SetAlive (false);

    if (animation)
      animation->Animate ();
  }

  return 0;
}

void
Projectile::Render ()
{
  if (isAlive ())
  {
    GameObject::Render ();
    if (animation)
      animation->Draw (x-boundX, y-boundY);
  }
}

void
Projectile::Collided (int ObjectID, col_dir dir)
{
  if (dir == NO_COLLISION)
    return;

  if (ObjectID == BLOCK)
  {
    SetAlive (false);
    //static_cast<PlayingState*> (g_GamePtr->GetState ())->GetPlatform ()->AddPoint ();
  }
}
