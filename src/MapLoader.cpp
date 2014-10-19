#include "MapLoader.h"

#include "defines.h"

// offsets for placing blocks properly on screen such that its edges are aligned with screen edges
#define offsetX g_Game.GetScreen_W ()/(float)BASE_SCREEN_X*value.frameWidth/2.0
#define offsetY g_Game.GetScreen_H ()/(float)BASE_SCREEN_Y*value.frameHeight/2.0

// multipliers for calulating size for scaled bitmap
#define scaleX g_Game.GetScreen_W ()/(float)BASE_SCREEN_X
#define scaleY g_Game.GetScreen_H ()/(float)BASE_SCREEN_Y

MapLoader::MapLoader (std::string filename)
{
  configfile = new ConfigFile (filename + ".cfg");
}

MapLoader::~MapLoader ()
{
  delete configfile;
  UnloadBitmaps ();
}

std::list<GameObject*>
MapLoader::LoadMap (std::string filename)
{
  // creating list which will contain block objects
  std::list<GameObject*> return_val;

  // opening map file
  std::ifstream file;
  file.open (filename.c_str ());

  // buffer
  std::string line;

  // setting y-coordinate starting position
  float posY = 0.0F;

  // loading bitmaps to a map 
  LoadBitmaps ();

  // iterating through lines of map file
  while (::getline (file, line))
  {
    // if line is empty then we continue
    if (!line.length ())
    {
      posY += g_Game.GetScreen_H ()/20;
      continue;
    }

    // setting x-coordinate starting position
    float posX = 0;

    //iterating through characters in line
    for (int i=0; i<20; i++)
    {
      //if we have an empty symbol we continue in a row and move posX
      if (line[i] == ' ')
      {
        posX += g_Game.GetScreen_W ()/20;
        continue;
      }

      //Receiving value from config file map
      Value value = configfile->GetValue_at_Key (line[i]);
      Block* g_object = new Block (bitmaps.find(line[i])->second,
                                   value.maxFrame,
                                   value.frameDelay,
                                   value.frameWidth,
                                   value.frameHeight,
                                   value.animationColumns,
                                   value.animationDirection);
      g_object->Init (posX + static_cast<float>(offsetX), posY + static_cast<float>(offsetY),
                      value.speed,
                      value.dirX,
                      value.health);
      return_val.push_back (g_object);

      posX += g_Game.GetScreen_W ()/20; // calculating X coordinate
    }

    posY += g_Game.GetScreen_H ()/20;     // calculating y coordinate
  }

  return return_val;
}

void
MapLoader::LoadBitmaps ()
{
  float scalerX = g_Game.GetScreen_W ()/(float)BASE_SCREEN_X;
  float scalerY = g_Game.GetScreen_H ()/(float)BASE_SCREEN_Y;

  for (std::map<char, Value>::iterator iter = configfile->map_begin ();
       iter != configfile->map_end ();
       iter++)
  {
    Value val = iter->second;

    int scaled_width  = static_cast<int>(scalerX) * val.frameWidth  * val.animationColumns;
    int scaled_height = static_cast<int>(scalerY) * val.frameHeight * (val.maxFrame + 1) / val.animationColumns;

    SDL_Surface* image = LoadScaledBitmap ((std::string (RESOURCE_DIRECTORY) + val.filename).c_str (), scaled_width, scaled_height);
    bitmaps.insert (std::pair<char, SDL_Surface*> (iter->first, image));
  }
}

void
MapLoader::UnloadBitmaps ()
{
  for (std::map<char, SDL_Surface*>::iterator iter = bitmaps.begin ();
       iter != bitmaps.end ();
       iter++)
    SDL_FreeSurface (iter->second);
}
