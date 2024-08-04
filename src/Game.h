#ifndef __Arkanoid__Game__
#define __Arkanoid__Game__

#include <string>

#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"

#include "Singleton.h"

#define g_Game        Game::GetSingleton ()
#define g_GamePtr     Game::GetSingletonPtr ()

enum GAME_STATE {MENU, PLAYING};
enum SFX {COLLISION, LASER, POWERUP};

// forward declarations
class FpsCounter;
class MenuState;
class Music;
class PlayingState;
class State;

// helper function for conversion
std::string IntToStr (int);
// helper function for Displaying Finish Text
void DisplayFinishText (unsigned int, const char*);

// forward declaration for use as callback functions
void SwitchControl ();
void SwitchSfx ();
void SwitchFPSVisibility();
void ChangeState ();
void ShutDown ();

class Game
 : public Singleton<Game>
{
 public:
  Game (int, char*[], u_int);
  virtual ~Game ();

  int Loop ();
  void HandleEvents ();

  inline void SetScreen_W (int screen_w_in) { screen_w = screen_w_in; }
  inline int GetScreen_W () { return screen_w; }

  inline void SetScreen_H (int screen_h_in) { screen_h = screen_h_in; }
  inline int GetScreen_H () { return screen_h; }

  inline int GetControl () { return control_type; }
  inline bool isSfxOn () { return sfxOn; }

  inline bool isFPSVisible () { return displayFPS; }
  inline void setFPSVisile () { displayFPS = true; }

  std::string GetUserName_ ();
  inline u_int* GetRandomSeedPtr () { return &randomSeed; }
  inline Music* GetMusic () { return music; }
  Mix_Chunk* GetSfx (enum SFX);
  inline SDL_Surface* GetScreen () { return screen; }
#if defined (SDL2_USE)
  inline SDL_Window* GetWindow () { return window; }
  // inline SDL_Renderer* GetRenderer () { return renderer; }
  // inline SDL_Texture* GetTexture () { return texture; }
#endif // SDL2_USE
  inline State* GetState () { return game_state; }
  inline TTF_Font* GetMainFont () { return font; }

  friend void ChangeState ();
  friend void ShutDown ();
  friend void SwitchControl ();
  friend void SwitchSfx ();
  friend void SwitchFPSVisibility();

  static void Draw (SDL_Surface*, SDL_Surface*, int, int);  // helper function for drawing

 private:
  int initSystems ();
  void closeSystems ();

  u_int        randomSeed;
  bool         running;
  bool         paused;

  bool         displayFPS;
  bool         sfxOn;

  int          control_type;

  int          current_state;

  int          screen_w;
  int          screen_h;
  int          gameFPS;

  State*       game_state;

  FpsCounter*  fps_counter;

#if defined (SDL2_USE)
  SDL_Window*   window;
  //SDL_Renderer* renderer;
  //SDL_Texture*  texture;
#endif // SDL2_USE
  SDL_Surface* screen;
  Music*       music;
  Mix_Chunk*   collision;
  Mix_Chunk*   laser;
  Mix_Chunk*   powerup;
  TTF_Font*    font;
};

#endif /* defined(__Arkanoid__Game__) */
