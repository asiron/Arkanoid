#ifndef MUSIC_H
#define MUSIC_H

#include <string>
#include <vector>

//#include "ace/Dirent.h"

#include "SDL_mixer.h"

//forward declaration for use as callback functions
struct dirent;

void SwitchMusic ();
int dirent_selector_mp3_files (const dirent*);
int dirent_selector_MIDI_files (const dirent*);
int dirent_comparator (const dirent**,
                       const dirent**);
void music_finished ();

enum Music_t
{
  MUSIC_INVALID = -1,
  MUSIC_MP3 = 0,
  MUSIC_MIDI,
  //
  MUSIC_MAX
};

class Music
{
 public:
  Music (const std::string&,    // FQ directory
         Music_t = MUSIC_MIDI); // music type
  ~Music ();

  inline bool isMusicOn () { return musicOn_; }
  void play (const std::string&, // track filename
             bool = true);       // loop ?
  void next (bool = true); // start playing ?

  friend void SwitchMusic ();
  friend void music_finished ();

  private:
   void update (const std::string&, // FQ directory
                Music_t);           // music type

   bool                     musicOn_;
   Mix_Music*               music_;
   std::string              musicDirectory_;
   std::vector<std::string> musicFiles_;
   bool                     repeat_;
   int                      currentIndex_;
};

#endif // MUSIC_H
