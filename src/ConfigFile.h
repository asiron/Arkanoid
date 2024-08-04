#ifndef __Arkanoid__ConfigFile__
#define __Arkanoid__ConfigFile__

#include <map>
#include <string>

struct value_t
{
  std::string filename;
  int maxFrame;
  int frameDelay;
  int frameWidth;
  int frameHeight;
  int animationColumns;
  int animationDirection;
  int speed;
  int dirX;
  int health;
};

class ConfigFile
{
 public:
  ConfigFile (const std::string&);
  value_t GetValue_at_Key (char key);
  std::map<char, value_t>::iterator map_begin () { return content.begin (); }
  std::map<char, value_t>::iterator map_end () { return content.end (); }

 private:
  std::map<char, value_t> content;
  std::string filename;
    
  template <typename T>
  static T String_to_T (const std::string&);
  static void ExitWithError (const std::string&);

  void RemoveComment (std::string&) const;
  bool HasOnlyWhiteSpace (std::string&) const;
  bool IsValidLine (std::string&) const;
  bool KeyExists (char key) const;

  void ExtractKey (char&, std::string&) const;
  void ExtractValue (value_t&, std::string&) const;
  void ExtractContents (std::string&, size_t);
  void Parse ();
};

#endif /* defined(__Arkanoid__ConfigFile__) */
