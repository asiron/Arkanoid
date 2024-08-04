#ifndef __Arkanoid__GameObject__
#define __Arkanoid__GameObject__

#include "SDL.h"

enum OBJECT_TYPE {PLAYER, BALL, BLOCK, EFFECT, PROJECTILE};                          // enum for defining an object, used during collisions
enum col_dir {NO_COLLISION, LEFT, RIGHT, TOP, BOTTOM, TLCOR, TRCOR, BLCOR, BRCOR};  // enum for indicating how objects collided, COR stands for corner
enum EFFECT_TYPE {GUN, MAGNET, SECONDBALL, LARGE};

// forward declarations
class Animation;

class GameObject
{
 public:
  GameObject (const char* = NULL, int = 0, int = 0, int = 0, int = 0, int = 0, int = 0);
  GameObject (SDL_Surface* = NULL, int = 0, int = 0, int = 0, int = 0, int = 0, int = 0);
  inline virtual ~GameObject () {}

  virtual void Destroy ();

  inline virtual void Init () {} // reinitialization function

  void Init (float, float, float, float, int, int, float, float);
  virtual void Render ();
  virtual int Update ();

  float GetX () { return x; }
  float GetY () { return y; }

  inline void SetX (float x_in) { x = x_in; }
  inline void SetY (float y_in) { y = y_in; }

  inline float GetBoundX () { return boundX; }
  inline float GetBoundY () { return boundY; }

  inline int GetID () { return ID; }
  inline void SetID (int ID_in) { ID = ID_in; }

  inline bool isAlive () { return alive; }
  inline void SetAlive (bool alive_in) { alive = alive_in; }

  inline bool isCollidable () { return collidable; }
  inline void setCollidable (bool collidable_in) { collidable = collidable_in; }

  enum col_dir detectCollision (GameObject*);
  inline virtual void Collided (int, enum col_dir) {}
  inline bool Collidable () { return alive && collidable; }

 protected:
  // object's properties for moving and collision
  float      x;
  float      y;
  float      velX;
  float      velY;

  int        dirX;
  int        dirY;
  float      boundX;
  float      boundY;

  Animation* animation;

 private:
  bool       alive;
  bool       collidable;
  int        ID;
};

#endif /* defined(__Arkanoid__GameObject__) */
