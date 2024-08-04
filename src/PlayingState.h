#ifndef __Arkanoid__PlayingState__
#define __Arkanoid__PlayingState__

#include <list>
#include <vector>

#include "SDL.h"

#include "State.h"

// forward declarations
class GameObject;
class Platform;
class Ball;
class Effect;
class Projectile;
class MapLoader;
class Gui;

class PlayingState
 : public State
{
  typedef State inherited;

 public:
  PlayingState ();
  virtual ~PlayingState ();

  virtual void InitState ();
  virtual void RenderState ();
  virtual void UpdateState ();
  virtual void HandleEvents (Uint8*, const SDL_Event&, int);

  inline void SetLevelCompleteFlag (bool flag) { levelcomplete = flag; }
  inline void SetChangingStateFlag (bool flag) { changingstate = flag; }
  inline bool GetChangingStateFlag () { return changingstate; }

  inline Effect** GetEffects () { return effects; }
  inline Projectile** GetProjectiles () { return projectiles; }
  inline Platform* GetPlatform () { return platform; }
  inline std::vector<Ball*>& GetBalls () { return balls; }

  void LaunchAdditionalBall ();
  void RemoveAdditionalBall (Ball*);
  void SwitchBalls (int);

  unsigned int CurrentLevel ();

 private:
  bool                   levelcomplete;
  bool                   changingstate;

  std::list<GameObject*> gobjects;
  std::string            current_level;
  MapLoader*             map_loader;

  std::vector<Ball*>     balls;
  Platform*              platform;
  Effect**               effects;
  Projectile**           projectiles;
  Gui*                   gui;

  std::vector<bool>      n_ball_flag;
  std::vector<bool>      remove_n_ball;

  void SaveHighscores ();
  bool LoadNextMap ();
};

#endif /* defined(__Arkanoid__PlayingState__) */
