#include "stdafx.h"

#include "State.h"

#include <fstream>
#include <sstream>
#include <algorithm>

#include "ace/OS.h"

#include "defines.h"

int
StrToInt (const std::string& s)
{
  std::istringstream istr (s);
  int returnVal;
  istr >> returnVal;

  return returnVal;
}

bool
Compare (const std::pair<std::string, int>& a,
         const std::pair<std::string, int>& b)
{
  return a > b;
}

State::State ()
 //: highsco_list ()
{
  char buffer[MAX_PATH];
  ACE_OS::getcwd (buffer, sizeof (buffer));
  std::string path_base = buffer;
  path_base += ACE_DIRECTORY_SEPARATOR_STR;
  path_base += RESOURCE_DIRECTORY;
  std::string filename = path_base;
  filename += ACE_DIRECTORY_SEPARATOR_STR;
  filename += ACE_TEXT_ALWAYS_CHAR ("highscores");
  std::ifstream file;
  file.open (filename.c_str ());

  std::string line;
  size_t it = 0; // character pointer
  while (std::getline (file, line))
  {
    it = 0; // character pointer
#ifdef __GNUC__
    line.erase (std::find (line.begin (), line.end (), ' '), line.end ()); //removing whitespaces
#else
    line.erase (remove (line.begin (), line.end (), ' '), line.end ());    //removing whitespaces
#endif
    std::string name = line.substr (0, it = line.find_first_of (","));     //getting players name
    line.erase (0, it + 1);

    highsco_list.push_back (std::pair<std::string, int> (name, StrToInt (line)));
  } // end WHILE

  file.close ();
}

void
State::PushScore (const std::string& name,
                  int highscore)
{
  highsco_list.push_back (std::pair<std::string, int>(name, highscore));
  highsco_list.sort (Compare);
}
