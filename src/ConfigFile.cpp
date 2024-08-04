#include "stdafx.h"

#include "ConfigFile.h"

#include <sstream>
#include <fstream>
#include <typeinfo>
#include <algorithm>

#include "ace/OS.h"

ConfigFile::ConfigFile (const std::string& filename_in)
 : filename (filename_in)
{
  Parse ();
}

template <typename T>
T
ConfigFile::String_to_T (const std::string &val)
{
  std::istringstream istr (val);
  T returnVal;

  if (!(istr >> returnVal))
    ExitWithError (ACE_TEXT_ALWAYS_CHAR ("ConfigFile: Not a valid " + (std::string)typeid (T).name () + " received\n"));

  return returnVal;
}

void
ConfigFile::ExitWithError (const std::string& error)
{
  std::cerr << error;
  std::cin.ignore ();
  std::cin.get ();

  ACE_OS::exit (EXIT_FAILURE);
}

void
ConfigFile::RemoveComment (std::string& line) const
{
  std::string::size_type position = line.find (";");
  if (position != std::string::npos)
    line.erase (position);
}

bool
ConfigFile::HasOnlyWhiteSpace (std::string& line) const
{
  return (line.find_first_not_of (' ') == std::string::npos);
}

bool
ConfigFile::IsValidLine (std::string& line) const
{
  // find first '='
  char target[] = {'='};
  std::string::iterator start_position = line.end ();
  start_position = std::find_first_of (line.begin (), line.end (), target, target + 1);
  if (start_position == line.end ())
    return false;

  //count commas
  std::string::iterator::difference_type count = 0;
  count = std::count (start_position + 1, line.end (), ',');

  return (count == 9);
}

bool
ConfigFile::KeyExists (char key) const
{
  return content.find (key) != content.end ();
}

value_t
ConfigFile::GetValue_at_Key (char key)
{
  return content.find (key)->second;
}

void
ConfigFile::ExtractKey (char& key, std::string& line) const
{
  key = line[0];
  line.erase (0,2);
}

void
ConfigFile::ExtractValue (value_t& value,
                          std::string& line) const
{
  size_t it = 0;

  value.filename = line.substr (0, it = line.find_first_of (','));
  line.erase (0, it + 1);
  value.maxFrame = String_to_T<int> (line.substr (0, it = line.find_first_of (',')));
  line.erase (0, it + 1);
  value.frameDelay = String_to_T<int> (line.substr (0, it = line.find_first_of (',')));
  line.erase (0, it + 1);
  value.frameWidth = String_to_T<int> (line.substr (0, it = line.find_first_of (',')));
  line.erase (0, it + 1);
  value.frameHeight = String_to_T<int> (line.substr (0, it = line.find_first_of (',')));
  line.erase (0, it + 1);
  value.animationColumns = String_to_T<int> (line.substr (0, it = line.find_first_of (',')));
  line.erase (0, it + 1);
  value.animationDirection = String_to_T<int> (line.substr (0, it = line.find_first_of (',')));
  line.erase (0, it + 1);
  value.speed = String_to_T<int> (line.substr (0, it = line.find_first_of (',')));
  line.erase (0, it + 1);
  value.dirX = String_to_T<int> (line.substr (0, it = line.find_first_of (',')));
  line.erase (0, it + 1);
  value.health = String_to_T<int> (line.substr (0, it = line.find_first_of (',')));
}

//struct is_whitespace_t
// //: public std::unary_function<char, bool>
//{
//  bool operator () (char character_in)
//  {
//    char target[] = {' ', '\t', 0};
//
//    return (std::find (target, target + 2, character_in) != 0);
//  }
//};

void
ConfigFile::ExtractContents (std::string& line,
                             size_t line_number)
{
  // remove any whitespace
//  // *TODO*: find a solution using function binding...
//  is_whitespace_t predicate;
//  std::remove_if(line.begin(), line.end(), predicate);
  line.erase (std::remove (line.begin (), line.end (), ' '), line.end ());
  std::cerr << line;

  char key;
  value_t value;

  ExtractKey (key, line);
  ExtractValue (value, line);

  if (!KeyExists (key))
    content.insert (std::pair<char, value_t> (key, value));
  else
    ExitWithError ("ConfigFile: Only unique keys are allowed at " + line_number);
}

void
ConfigFile::Parse ()
{
  std::ifstream file;
  file.open (filename);
  if (!file)
    ExitWithError ("ConfigFile: file " + filename + "not found\n");

  std::string line;
  size_t line_number = 0;
  while (std::getline (file, line))
  {
    ++line_number;

    // support comments
    RemoveComment (line);
    if (HasOnlyWhiteSpace (line)) // empty line so we continue
      continue;

    // validate schema
    if (!IsValidLine (line))
      ExitWithError ("ConfigFile: Found invalid line at " + line_number);

    // parse line
    ExtractContents (line, line_number);
  }

  file.close ();
}
