#include "GameObject.h"

GameObject::GameObject (const char* filename,
                        int maxFrame,
                        int frameDelay,
                        int frameWidth,
                        int frameHeight,
                        int animationColumns,
                        int animationDirection)
 : x (0.0)
 , y (0.0)
 , velX (0.0)
 , velY (0.0)
 , dirX (0)
 , dirY (0)
 , boundX (0.0)
 , boundY (0.0)
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
 : x (0.0)
 , y (0.0)
 , velX (0.0)
 , velY (0.0)
 , dirX (0)
 , dirY (0)
 , boundX (0.0)
 , boundY (0.0)
 , alive (false)
 , collidable (true)
{
  //creating Animation object if we have provided a filename
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
  //Destorying GameObject
  if (animation)
    delete animation;    // deleting Animation if it existed
}

void
GameObject::Init ()
{

}     // Leave that blank for overwriting

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
col_dir
GameObject::detectCollision (GameObject* otherObject)
{
  if (!(Collidable () && otherObject->Collidable ()))
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
    if (::abs (x - otherObjectX) < otherObjectBoundX - 5) //Vertical collision
    {
      if (y < otherObjectY) return TOP;
      else return BOTTOM;
    }
    else if (::abs (y - otherObjectY) < 15) //Horizontal collision
    {
      if (x < otherObjectX) return LEFT;
      else return RIGHT;
    }
//        else { //Diagonal collision
//            if(x < otherObjectX && y < otherObjectY)
//                return TLCOR;
//            else if (x < otherObjectX && y > otherObjectY)
//                return BLCOR;
//            else if (x > otherObjectX && y < otherObjectY)
//                return TRCOR;
//            else
//                return BRCOR;
//        }
  }

  return NO_COLLISION;
}

//Handle collisions differently for every object
void
GameObject::Collided (int objectID, col_dir dir)
{

}
