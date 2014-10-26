#ifndef __Arkanoid__Ball__
#define __Arkanoid__Ball__

#include "GameObject.h"

class Ball
 : public GameObject
{
 public:
  Ball (const char* = NULL,
        int = 0,
        int = 0,
        int = 0,
        int = 0,
        int = 0,
        int = 0);
  virtual ~Ball ();

  void Destroy ();
  void Init ();
  void Render ();
  int Update ();

  void MorphBall (int effect) { has_effect = effect; }
  void LoseEffect () { has_effect = -1;}
  void StartFlying ();

  //respond to collision function
  void Collided (int, col_dir);

 private:
  typedef GameObject inherited;

  int has_effect;
  bool stand_on_platform;
};

#endif /* defined(__Arkanoid__Ball__) */
