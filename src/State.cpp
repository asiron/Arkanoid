#include "stdafx.h"

#include "State.h"

#include <fstream>
#include <sstream>
#include <algorithm>

#include "defines.h"

int
StrToInt (std::string s)
{
  std::istringstream istr (s);
  int returnVal;
  istr >> returnVal;
  return returnVal;
}

bool
Compare (std::pair<std::string, int> a,
         std::pair<std::string, int> b)
{
  return a > b;
}

State::State ()
{
  std::ifstream file;

  file.open (std::string (RESOURCE_DIRECTORY) + "/highscores");
  std::string line;
  while (getline (file, line))
  {
    size_t it = 0; // character pointer

#ifdef __GNUC__
    line.erase (std::find (line.begin (), line.end (), ' '), line.end ());                          //removing whitespaces
#else
    line.erase (remove (line.begin (), line.end (), ' '), line.end ());                          //removing whitespaces
#endif
    std::string name = line.substr (0, it = line.find_first_of (","));                                   //getting players name
    line.erase (0, it + 1);

    int score = StrToInt (line);
    highsco_list.push_back (std::pair<std::string, int> (name, score));
  } // end WHILE
  file.close ();
}

void
State::PushScore (std::string name,
                  int highscore)
{
  highsco_list.push_back (std::pair<std::string, int>(name, highscore));
  highsco_list.sort (Compare);
}
