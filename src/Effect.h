#ifndef __Arkanoid__Effect__
#define __Arkanoid__Effect__

#include "GameObject.h"

class Effect
 : public GameObject
{
 public:
  Effect (const char* = NULL, int = 0, int = 0, int = 0, int = 0, int = 0, int = 0);
  virtual ~Effect () {}

  void Destroy ();
  inline void Init () {}
  void Init (int x, int y);
  void Render ();
  int Update ();

  inline void SetEffectType (int type) { effect_type = type;}
  virtual void Collided (int ObjectID, enum col_dir dir);

 private:
  int effect_type;
};

#endif /* defined(__Arkanoid__Effect__) */
