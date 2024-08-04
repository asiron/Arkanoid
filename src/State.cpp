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
  return a.second > b.second;
}

State::State ()
 : highsco_list ()
{
  char buffer_a[PATH_MAX];
  ACE_OS::getcwd (buffer_a, sizeof (char[PATH_MAX]));
  std::string path_base = buffer_a;
  path_base += ACE_DIRECTORY_SEPARATOR_STR_A;
  path_base += RESOURCE_DIRECTORY;
  std::string filename = path_base;
  filename += ACE_DIRECTORY_SEPARATOR_STR_A;
  filename += ACE_TEXT_ALWAYS_CHAR ("highscores");
  std::ifstream file;
  file.open (filename.c_str (), ios::binary);

  std::string line;
  size_t it = 0; // character pointer
  char c;
  while (std::getline (file, line, '\r'))
  {
    // ignore following \n or restore the buffer data
    if ((c = file.get ()) != '\n')
      file.rdbuf ()->sputbackc (c);

    it = 0; // character pointer
    line.erase (std::remove (line.begin (), line.end (), ' '), line.end ());    //removing whitespaces
    std::string name = line.substr (0, it = line.find_first_of (","));     //getting players name
    line.erase (0, it + 1);

    highsco_list.push_back (std::make_pair (name, StrToInt (line)));
  } // end WHILE

  file.close ();
}

void
State::PushScore (const std::string& name,
                  int highscore)
{
  highsco_list.push_back (std::make_pair (name, highscore));
  highsco_list.sort (Compare);
}
