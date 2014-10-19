#include <cstdlib>

#include "SDL.h"

#include "Game.h"

int
main (int argc, char** argv)
{
  ::srand (SDL_GetTicks ()); // Initializing random seed
  Game game (argc, argv);  // Creating game
  return game.Loop ();     // Starting game
}
