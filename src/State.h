#ifndef __Arkanoid__State__
#define __Arkanoid__State__

#include <iostream>
#include <list>
#include "SDL.h"

bool Compare(std::pair<std::string, int> a, std::pair<std::string, int> b);
int StrToInt(std::string s);

class State {
 public:
  State ();
  virtual ~State () {};

  void virtual HandleEvents (Uint8* keystates, SDL_Event event, int control_type) {};
  void virtual RenderState () {};
  void virtual UpdateState () {};
  void virtual InitState () {};

  void PushScore (std::string name, int highscore);

 protected:
  std::list<std::pair<std::string, int> > highsco_list; // list of all highscores and their names
};

#endif /* defined(__Arkanoid__State__) */
