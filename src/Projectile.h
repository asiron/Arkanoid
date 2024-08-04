#ifndef __Arkanoid__Projectile__
#define __Arkanoid__Projectile__

#include "GameObject.h"

class Projectile
 : public GameObject
{
 public:
  Projectile (const char* = NULL, int = 0, int = 0, int = 0, int = 0, int = 0, int = 0);
  virtual ~Projectile () {}

  void Destroy ();
  void Init (int, int, int);
  void Render ();
  int Update ();

  //respond to collision function
  void Collided (int, enum col_dir);
};

#endif /* defined(__Arkanoid__Projectile__) */
