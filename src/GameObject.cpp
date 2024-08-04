#include "stdafx.h"

#include "GameObject.h"

#include "Animation.h"

GameObject::GameObject (const char* filename,
                        int maxFrame,
                        int frameDelay,
                        int frameWidth,
                        int frameHeight,
                        int animationColumns,
                        int animationDirection)
 : x (0.0f)
 , y (0.0f)
 , velX (0.0f)
 , velY (0.0f)
 , dirX (0)
 , dirY (0)
 , boundX (0.0f)
 , boundY (0.0f)
 , animation (NULL)
 , alive (false)
 , collidable (true)
{
  //creating Animation object if we have provided a filename
  if (filename)
    animation = new Animation (filename,
                               maxFrame,
                               frameDelay,
                               frameWidth,
                               frameHeight,
                               animationColumns,
                               animationDirection);
}

GameObject::GameObject (SDL_Surface* image,
                        int maxFrame,
                        int frameDelay,
                        int frameWidth,
                        int frameHeight,
                        int animationColumns,
                        int animationDirection)
 : x (0.0f)
 , y (0.0f)
 , velX (0.0f)
 , velY (0.0f)
 , dirX (0)
 , dirY (0)
 , boundX (0.0f)
 , boundY (0.0f)
 , animation (NULL)
 , alive (false)
 , collidable (true)
{
  //creating Animation object if we have provided an image
  if (image)
    animation = new Animation (image,
                               maxFrame,
                               frameDelay,
                               frameWidth,
                               frameHeight,
                               animationColumns,
                               animationDirection);
}

void
GameObject::Destroy ()
{
  //Destroying GameObject
  if (animation)
    delete animation; // deleting Animation if it existed

  delete this;
}

void
GameObject::Init (float x, float y,
                  float velX, float velY,
                  int dirX, int dirY,
                  float boundX, float boundY)
{
  GameObject::x = x;
  GameObject::y = y;
  GameObject::velY = velY;
  GameObject::velX = velX;
  GameObject::dirX = dirX;
  GameObject::dirY = dirY;
  GameObject::boundX = boundX;
  GameObject::boundY = boundY;
}

// Assuming no gravity, every object is updated the same way based on its velocity
int
GameObject::Update ()
{
  //Simple updating for every GameObject
  x += velX * dirX;
  y += velY * dirY;

  return 0;
}

// Every object is rendered in different way, 
void
GameObject::Render ()
{

}

// Collision detection function for every object
enum col_dir
GameObject::detectCollision (GameObject* otherObject)
{
  if (!(Collidable () && otherObject->Collidable ()) ||
      (this == otherObject))
    return NO_COLLISION;

  float otherObjectX = otherObject->GetX ();
  float otherObjectY = otherObject->GetY ();
  float otherObjectBoundX = otherObject->GetBoundX ();
  float otherObjectBoundY = otherObject->GetBoundY ();

  if ((x + boundX > otherObjectX - otherObjectBoundX) &&
      (x - boundX < otherObjectX + otherObjectBoundX) &&
      (y + boundY > otherObjectY - otherObjectBoundY) &&
      (y - boundY < otherObjectY + otherObjectBoundY))
  {
    if (std::abs (x - otherObjectX) < otherObjectBoundX) //Vertical collision
    {
      if (y < otherObjectY)
        return TOP;
      else
        return BOTTOM;
    }
    else if (std::abs (y - otherObjectY) < otherObjectBoundY) //Horizontal collision
    {
      if (x < otherObjectX)
        return LEFT;
      else
        return RIGHT;
    }
    else { //Diagonal collision
      if (x < otherObjectX && y < otherObjectY)
        return TLCOR;
      else if (x < otherObjectX && y > otherObjectY)
        return BLCOR;
      else if (x > otherObjectX && y < otherObjectY)
        return TRCOR;
      else
        return BRCOR;
    }
  }

  return NO_COLLISION;
}
