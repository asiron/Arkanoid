#include "stdafx.h"

#include "MapLoader.h"

#include <fstream>

#include "ace/OS.h"

#include "Block.h"
#include "ConfigFile.h"
#include "defines.h"
#include "Game.h"
#include "GameObject.h"
#include "scaler.h"

// offsets for placing blocks properly on screen such that its edges are aligned with screen edges
#define offsetX g_Game.GetScreen_W()/(float)BASE_SCREEN_X * value.frameWidth/2.0f
#define offsetY g_Game.GetScreen_H()/(float)BASE_SCREEN_Y * value.frameHeight/2.0f

// multipliers for calulating size for scaled bitmap
#define scaleX g_Game.GetScreen_W ()/(float)BASE_SCREEN_X
#define scaleY g_Game.GetScreen_H ()/(float)BASE_SCREEN_Y

MapLoader::MapLoader (const std::string& filename_in)
 : configfile (NULL)
{
  configfile = new ConfigFile (filename_in);

  // load bitmaps
  LoadBitmaps ();
}

MapLoader::~MapLoader ()
{
  if (configfile)
    delete configfile;
  UnloadBitmaps ();
}

map_t
MapLoader::LoadMap (const std::string& filename_in)
{
  // creating list which will contain block objects
  map_t return_val;

  // opening map file
  std::ifstream file;
  file.open (filename_in.c_str (), ios::binary);

  // buffer
  std::string line;

  // setting y-coordinate starting position
  float posY = 0.0f;

  // iterating through lines of map file
  char c;
  while (std::getline (file, line, '\r'))
  {
    // ignore following \n or restore the buffer data
    if ((c = file.get ()) != '\n')
      file.rdbuf ()->sputbackc (c);

    // if line is empty then we continue
    if (!line.length ())
    {
      posY += g_Game.GetScreen_H () / 20;
      continue;
    }

    // setting x-coordinate starting position
    float posX = 0.0f;

    //iterating through characters in line
    for (int i = 0; i < 20; i++)
    {
      //if we have an empty symbol we continue in a row and move posX
      if (line[i] == ' ')
      {
        posX += g_Game.GetScreen_W () / 20;
        continue;
      }

      //Receiving value from config file map
      value_t value = configfile->GetValue_at_Key (line[i]);
      Block* g_object = new Block (bitmaps.find (line[i])->second,
                                   value.maxFrame,
                                   value.frameDelay,
                                   value.frameWidth,
                                   value.frameHeight,
                                   value.animationColumns,
                                   value.animationDirection);
      g_object->Init (posX + static_cast<float> (offsetX), posY + static_cast<float> (offsetY),
                      value.speed,
                      value.dirX,
                      value.health);
      return_val.push_back (g_object);

      posX += g_Game.GetScreen_W () / 20; // calculating X coordinate
    }

    posY += g_Game.GetScreen_H () / 20; // calculating y coordinate
  }

  return return_val;
}

void
MapLoader::LoadBitmaps ()
{
  float scalerX = g_Game.GetScreen_W ()/(float)BASE_SCREEN_X;
  float scalerY = g_Game.GetScreen_H ()/(float)BASE_SCREEN_Y;

  char buffer_a[PATH_MAX];
  ACE_OS::getcwd (buffer_a, sizeof (char[PATH_MAX]));
  std::string path_base = buffer_a;
  path_base += ACE_DIRECTORY_SEPARATOR_STR_A;
  path_base += RESOURCE_DIRECTORY;
  path_base += ACE_DIRECTORY_SEPARATOR_STR_A;
  path_base += ACE_TEXT_ALWAYS_CHAR (GRAPHICS_DIRECTORY);
  path_base += ACE_DIRECTORY_SEPARATOR_STR_A;
  std::string file;
  for (std::map<char, value_t>::iterator iter = configfile->map_begin ();
       iter != configfile->map_end ();
       iter++)
  {
    value_t val = iter->second;

    int scaled_width  = static_cast<int>(scalerX * val.frameWidth  * val.animationColumns);
    int scaled_height = static_cast<int>(scalerY * val.frameHeight * (val.maxFrame + 1) / val.animationColumns);

    file = path_base;
    file += val.filename;
    SDL_Surface* image = LoadScaledBitmap (file.c_str (), scaled_width, scaled_height);
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
