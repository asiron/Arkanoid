#ifndef __Arkanoid__Block__
#define __Arkanoid__Block__

#include "SDL.h"

#include "GameObject.h"

class Block
 : public GameObject
{
 public:
  Block (const char* filename = NULL, int maxFrame = 0, int frameDelay = 0, int frameWidth = 0,
         int frameHeight = 0, int animationColumns = 0, int animationDirection = 0 ) ;
  Block (SDL_Surface* image = NULL, int maxFrame = 0, int frameDelay = 0, int frameWidth = 0,
         int frameHeight = 0, int animationColumns = 0, int animationDirection = 0 ) ;

  void Destroy ();
  void Init (float x, float y, int speed, int dirX, int health);
  void Render ();
  int Update ();

  //respond to collision function
  void Collided (int ObjectID, col_dir dir);

 private:
  int health;
  int maxhealth;
};

#endif /* defined(__Arkanoid__Block__) */
