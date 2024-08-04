#include "stdafx.h"

#include "Music.h"

#include "ace/OS.h"
#include "ace/Dirent_Selector.h"
#include "ace/Log_Msg.h"

#include "defines.h"
#include "Game.h"
#include "MenuState.h"

void
SwitchMusic ()
{
  g_GamePtr->GetMusic ()->musicOn_ = !g_GamePtr->GetMusic ()->musicOn_;
  static_cast<MenuState*> (g_GamePtr->GetState ())->UpdateInfo (MUSICON);

  if (g_GamePtr->GetMusic ()->isMusicOn ())
  {
    if (Mix_PlayingMusic () == 0)
    {
      if (Mix_FadeInMusic (g_GamePtr->GetMusic ()->music_, 0, SOUNDS_MUSIC_FADE_PERIOD) == -1)
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to Mix_FadeInMusic: \"%s\", continuing\n"),
                    ACE_TEXT (Mix_GetError ())));
    }
    else
      if (Mix_PlayMusic (g_GamePtr->GetMusic ()->music_, 0) == -1)
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to Mix_PlayMusic: \"%s\", continuing\n"),
                    ACE_TEXT (Mix_GetError ())));
  }
  else
  {
    if (Mix_PlayingMusic ())
    {
      if (Mix_FadeOutMusic (SOUNDS_MUSIC_FADE_PERIOD) == -1)
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to Mix_FadeOutMusic: \"%s\", continuing\n"),
                    ACE_TEXT (Mix_GetError ())));
    }
    else
      if (Mix_HaltMusic () == -1)
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("failed to Mix_HaltMusic: \"%s\", continuing\n"),
                    ACE_TEXT (Mix_GetError ())));
  }
}

int
dirent_selector_MIDI_files (const dirent* entry_in)
{
  static std::string MIDI_extension = ACE_TEXT_ALWAYS_CHAR (SOUNDS_MIDI_EXT);

  std::string filename (entry_in->d_name);
  std::string::size_type position =
    filename.rfind (MIDI_extension, std::string::npos);
  if ((position == std::string::npos) ||
      (position != (filename.size () - MIDI_extension.size ())))
      //                     -1) != (filename.size() - extension.size()))
  {
    //     ACE_DEBUG((LM_DEBUG,
    //                ACE_TEXT("ignoring \"%s\"...\n"),
    //                ACE_TEXT(entry_in->d_name)));

    return 0;
  } // end IF

  return 1;
}

int
dirent_selector_mp3_files (const dirent* entry_in)
{
  static std::string mp3_extension = ACE_TEXT_ALWAYS_CHAR (SOUNDS_MP3_EXT);

  std::string filename (entry_in->d_name);
  std::string::size_type position =
    filename.rfind (mp3_extension, std::string::npos);
  if ((position == std::string::npos) ||
      (position != (filename.size () - mp3_extension.size ())))
      //                     -1) != (filename.size() - extension.size()))
  {
    //     ACE_DEBUG((LM_DEBUG,
    //                ACE_TEXT("ignoring \"%s\"...\n"),
    //                ACE_TEXT(entry_in->d_name)));

    return 0;
  } // end IF

  return 1;
}

int
dirent_comparator (const dirent** entry1_in,
                   const dirent** entry2_in)
{
  return ACE_OS::strcmp ((*entry1_in)->d_name,
                         (*entry2_in)->d_name);
}

void 
music_finished ()
{
  Music* music = g_GamePtr->GetMusic ();
  ACE_ASSERT (music);

  music->next ();
}

Music::Music (const std::string& baseDirectory_in,
              Music_t type_in)
 : musicOn_ (false)
 , music_ (NULL)
 , musicDirectory_ ()
 , musicFiles_ ()
 , repeat_ (false)
 , currentIndex_ (-1)
{
  Mix_HookMusicFinished (music_finished);
  Mix_VolumeMusic (SOUNDS_MUSIC_VOLUME);

  musicDirectory_ = baseDirectory_in;
  if (musicDirectory_.empty ())
  {
    char buffer_a[PATH_MAX];
    ACE_OS::getcwd (buffer_a, sizeof (char[PATH_MAX]));
    musicDirectory_ = buffer_a;
    musicDirectory_ += ACE_DIRECTORY_SEPARATOR_STR_A;
    musicDirectory_ += ACE_TEXT_ALWAYS_CHAR (RESOURCE_DIRECTORY);
  }
  musicDirectory_ += ACE_DIRECTORY_SEPARATOR_STR_A;
  musicDirectory_ += ACE_TEXT_ALWAYS_CHAR (SOUNDS_DIRECTORY);
  update (musicDirectory_, type_in);

  next (false);
}

Music::~Music ()
{
  if (Mix_PlayingMusic ())
    if (Mix_HaltMusic () == -1)
      ACE_DEBUG ((LM_ERROR,
                  ACE_TEXT ("failed to Mix_HaltMusic: \"%s\", continuing\n"),
                  ACE_TEXT (Mix_GetError ())));

  if (music_)
    Mix_FreeMusic (music_);
}

void
Music::play (const std::string& filename_in,
             bool loop_in)
{
  if (Mix_PlayingMusic ())
    if (Mix_HaltMusic () == -1)
      ACE_DEBUG ((LM_ERROR,
                  ACE_TEXT ("failed to Mix_HaltMusic: \"%s\", continuing\n"),
                  ACE_TEXT (Mix_GetError ())));

  ACE_ASSERT (!musicFiles_.empty ());

  if (music_)
  {
    Mix_FreeMusic (music_); music_ = NULL;
  }

  for (currentIndex_ = 0;
       currentIndex_ < static_cast<int> (musicFiles_.size ());
       currentIndex_++)
    if (musicFiles_[currentIndex_] == filename_in)
      break;
  if (currentIndex_ == static_cast<int> (musicFiles_.size ()))
  {
    ACE_DEBUG ((LM_ERROR,
                ACE_TEXT ("invalid/unknown filename: \"%s\", returning\n"),
                ACE_TEXT (filename_in.c_str ())));
    return;
  }
  std::string filename = musicDirectory_;
  filename += ACE_DIRECTORY_SEPARATOR_STR_A;
  filename += musicFiles_[currentIndex_];
  music_ = Mix_LoadMUS (filename.c_str ());
  if (!music_)
  {
    ACE_DEBUG ((LM_ERROR,
                ACE_TEXT ("failed to Mix_LoadMUS (\"%s\"): \"%s\", returning\n"),
                ACE_TEXT (filename.c_str ()),
                ACE_TEXT (Mix_GetError ())));
    return;
  }

  if (musicOn_)
    if (Mix_PlayMusic (music_, (loop_in ? -1 : 0)) == -1)
      ACE_DEBUG ((LM_ERROR,
                  ACE_TEXT ("failed to Mix_PlayMusic: \"%s\", continuing\n"),
                  ACE_TEXT (Mix_GetError ())));
}

void
Music::next (bool startPlaying_in)
{
  if (Mix_PlayingMusic ())
    if (Mix_HaltMusic () == -1)
      ACE_DEBUG ((LM_ERROR,
                  ACE_TEXT ("failed to Mix_HaltMusic: \"%s\", continuing\n"),
                  ACE_TEXT (Mix_GetError ())));

  ACE_ASSERT (!musicFiles_.empty ());

  if (!repeat_)
  {
    if (music_)
    {
      Mix_FreeMusic (music_); music_ = NULL;
    } // end IF

    currentIndex_ = (ACE_OS::rand_r (g_GamePtr->GetRandomSeedPtr ()) % musicFiles_.size ());
    std::string filename = musicDirectory_;
    filename += ACE_DIRECTORY_SEPARATOR_STR_A;
    filename += musicFiles_[currentIndex_];
    music_ = Mix_LoadMUS (filename.c_str ());
    if (!music_)
    {
      ACE_DEBUG ((LM_ERROR,
                  ACE_TEXT ("failed to Mix_LoadMUS (\"%s\"): \"%s\", returning\n"),
                  ACE_TEXT (filename.c_str ()),
                  ACE_TEXT (Mix_GetError ())));
      return;
    }
  }

  if (musicOn_ && startPlaying_in)
    if (Mix_PlayMusic (music_, 0) == -1)
      ACE_DEBUG ((LM_ERROR,
                  ACE_TEXT ("failed to Mix_PlayMusic: \"%s\", continuing\n"),
                  ACE_TEXT (Mix_GetError ())));
}

void
Music::update (const std::string& directory_in,
               Music_t type_in)
{
  musicFiles_.clear ();

  ACE_SCANDIR_SELECTOR selector = NULL;
  switch (type_in)
  {
    case MUSIC_MP3:
      selector = dirent_selector_mp3_files; break;
    case MUSIC_MIDI:
      selector = dirent_selector_MIDI_files; break;
    default:
      ACE_DEBUG ((LM_ERROR,
                  ACE_TEXT ("invalid/unknown music type (was: %d), continuing\n"),
                  type_in));
      return;
  }

  ACE_Dirent_Selector entries;
  if (entries.open (ACE_TEXT_CHAR_TO_TCHAR (directory_in.c_str ()),
                    selector,
                    &::dirent_comparator) == -1)
  {
    ACE_DEBUG ((LM_ERROR,
                ACE_TEXT ("failed to ACE_Dirent_Selector::open(\"%s\"): \"%m\", continuing\n"),
                ACE_TEXT (directory_in.c_str ())));
    return;
  } // end IF

  for (int i = 0;
       i < entries.length ();
       i++)
    musicFiles_.push_back (ACE_TEXT_ALWAYS_CHAR (entries[i]->d_name));

  if (entries.close () == -1)
    ACE_DEBUG ((LM_ERROR,
                ACE_TEXT ("failed to ACE_Dirent_Selector::close: \"%m\", continuing\n")));
}
