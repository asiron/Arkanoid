#include "stdafx.h"

#include <cstdlib>

#include "ace/OS.h"

#include "SDL.h"

#include "Game.h"

//#if defined(ACE_WIN32) || defined(ACE_WIN64)
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
int
SDL_main (int argc_in,
          char** argv_in)
//#else
//int
//ACE_TMAIN (int argc_in,
//           ACE_TCHAR* argv_in[])
//#endif
{
  ACE_OS::srand (SDL_GetTicks ()); // Initializing random seed
  Game game (argc_in, argv_in);  // Creating game
  return game.Loop ();     // Starting game
}
//#if defined(ACE_WIN32) || defined(ACE_WIN64)
#ifdef __cplusplus
}
#endif /* __cplusplus */
