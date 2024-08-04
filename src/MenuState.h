#ifndef __Arkanoid__MenuState__
#define __Arkanoid__MenuState__

#include <tuple>
#include <list>

#include "SDL.h"
#include "SDL_ttf.h"

#include "State.h"
#include "Background.h" 

//ID enums for current menu state and option ID
enum MENU_STATE {MAIN_MENU, OPTIONS, HIGHSCORES};
enum MENU_OPTION_IDS {CONTROL, MUSICON, SOUNDON, SHOWFPS};

// Function pointer to function that is run when object is clicked
typedef void (*Clicked) (void);

// typdefs for cleaner implementation of menu objects
typedef std::tuple<SDL_Surface*, SDL_Surface*, SDL_Surface*> Text;

typedef std::tuple<Text, bool, Text> Highscore_Text;
typedef std::tuple<Text, bool, Clicked> MainMenuText;
typedef std::tuple<Text, bool, Clicked, Text, Text, bool, int> OptionsText;

//friend functions
void GotoOptions ();
void GotoHighscores ();
void GotoMainMenu ();

class MenuState
 : public State
{
 public:
  MenuState ();

  void Destroy ();

  void UpdateInfo (int);

  virtual void HandleEvents (Uint8*, const SDL_Event&, int);
  virtual void RenderState ();
  virtual void UpdateState ();
  virtual void InitState () {};

  friend void GotoOptions ();
  friend void GotoHighscores ();
  friend void GotoMainMenu ();

 private:
  typedef State inherited;

  std::list<Background*>    bgs;

  std::list<MainMenuText>   menu_main;
  std::list<Highscore_Text> menu_highscores;
  std::list<OptionsText>    menu_options;

  int                       mouse_pos_x;
  int                       mouse_pos_y;

  int                       offsetX;    // offset from left to first menu message
  int                       offsetY;    // offset from top to menu messages

  int                       distance_between_msg;       // vertical distance between consecutive MSGs
  int                       distance_between_opt;       // horizontal distance between Option and value

  int                       curMenu;

  struct SDL_Color          text;
  struct SDL_Color          shade;
  struct SDL_Color          highlight;

  TTF_Font*                 font;
  inline void Draw (SDL_Surface*, int, int) const;

  // function pointers to menu event listener
  Clicked                   startgame;
  Clicked                   options;
  Clicked                   highscores;
  Clicked                   quit;
  Clicked                   mainmenu;
  Clicked                   switchcontrol;
  Clicked                   musicon;
  Clicked                   soundon;
  Clicked                   showfps;

  template <typename T> void UpdateList (std::list<T>&);
  template <typename T> void RunCommand (std::list<T>&);
};

#endif /* defined(__Arkanoid__MenuState__) */
