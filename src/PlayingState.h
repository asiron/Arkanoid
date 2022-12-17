#ifndef __Arkanoid__PlayingState__
#define __Arkanoid__PlayingState__

#include <list>

#include "SDL.h"

#include "State.h"
#include "GameObject.h"
#include "Platform.h"
#include "Ball.h"
#include "Block.h"
#include "Effect.h"
#include "Projectile.h"
#include "MapLoader.h"
#include "Gui.h"

class PlayingState
 : public State
{
 public:
  PlayingState ();
  ~PlayingState ();

  virtual void InitState ();
  virtual void RenderState ();
  virtual void UpdateState ();
  virtual void HandleEvents (Uint8*, const SDL_Event&, int);

  void SetChangingStateFlag (bool flag) { changingstate = flag; }
  bool GetChangingStateFlag () { return changingstate; }

  Effect** GetEffects () { return effects; }
  Projectile** GetProjectiles () { return projectiles; }
  Platform* GetPlatform () { return platform; }
  Ball* GetBall () { return ball; }

  void LaunchSecondBall ();

 private:
  typedef State inherited;

  bool changingstate;

  std::list<GameObject*> gobjects;
  MapLoader* map_loader;
    
  Ball* ball;
  Ball* second_ball;
  Platform* platform;
  Effect** effects;
  Projectile** projectiles;
  Gui* gui;

  int second_ball_flag;

  void SaveHighscores ();
};

#endif /* defined(__Arkanoid__PlayingState__) */
