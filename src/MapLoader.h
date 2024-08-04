#ifndef __Arkanoid__MapLoader__
#define __Arkanoid__MapLoader__

#include <list>
#include <string>
#include <map>

#include "SDL.h"

// forward declarations
class ConfigFile;
class GameObject;

typedef std::list<GameObject*> map_t;

class MapLoader
{
 public:
  MapLoader (const std::string&);
  ~MapLoader ();

  map_t LoadMap (const std::string&);

 private:
  ConfigFile* configfile;
  std::map<char, SDL_Surface*> bitmaps;

  void LoadBitmaps();
  void UnloadBitmaps();
};

#endif /* defined(__Arkanoid__MapLoader__) */
