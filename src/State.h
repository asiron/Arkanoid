#ifndef __Arkanoid__State__
#define __Arkanoid__State__

#include <utility>
#include <string>
#include <list>

#include "SDL.h"

bool Compare (const std::pair<std::string, int>&, const std::pair<std::string, int>&);
int StrToInt (const std::string&);

class State
{
 public:
  State ();
  virtual ~State () {}

  virtual void HandleEvents (Uint8*, const SDL_Event&, int) = 0;
  virtual void RenderState () = 0;
  virtual void UpdateState () = 0;
  virtual void InitState () = 0;

  void PushScore (const std::string&, int);

 protected:
  std::list<std::pair<std::string, int> > highsco_list; // list of all highscores and their names
};

#endif /* defined(__Arkanoid__State__) */
