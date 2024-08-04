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
  inline virtual ~Ball () {}

  void Destroy ();
  void Init ();
  void Render ();
  int Update ();

  inline void MorphBall (int effect) { has_effect = effect; }
  void LoseEffect ();
  bool IsOnPlatform ();
  void StartFlying ();

  //respond to collision function
  virtual void Collided (int, enum col_dir);

 private:
  typedef GameObject inherited;

  int  has_effect;
  bool stand_on_platform;
  bool launching; // still on platform ?
};

#endif /* defined(__Arkanoid__Ball__) */
