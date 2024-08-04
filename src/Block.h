#ifndef __Arkanoid__Block__
#define __Arkanoid__Block__

#include "SDL.h"

#include "GameObject.h"

class Block
 : public GameObject
{
 public:
  Block (const char* = NULL, int = 0, int = 0, int = 0, int = 0, int = 0, int = 0);
  Block (SDL_Surface* = NULL, int = 0, int = 0, int = 0, int = 0, int = 0, int = 0);

  void Destroy ();
  void Init (float, float, int, int, int);
  void Render ();
  int Update ();

  inline int GetHealth () { return health; }

  //respond to collision function
  void Collided (int, enum col_dir);

 private:
  int health;
  int maxhealth;
};

#endif /* defined(__Arkanoid__Block__) */
