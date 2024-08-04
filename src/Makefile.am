METASOURCES = AUTO

ACLOCAL_AMFLAGS = -I .
AM_CPPFLAGS =

bin_PROGRAMS = Arkanoid

arkanoid_SOURCES = \
  Animation.h \
  Animation.cpp \
  Background.h \
  Background.cpp \
  Ball.h \
  Ball.cpp \
  Block.h \
  Block.cpp \
  ConfigFile.h \
  ConfigFile.cpp \
  Effect.h \
  Effect.cpp \
  FpsCounter.h \
  FpsCounter.cpp \
  Game.h \
  Game.cpp \
  GameObject.h \
  GameObject.cpp \
  Gui.h \
  Gui.cpp \
  main.cpp \
  MapLoader.cpp \
  MapLoader.h \
  MenuState.cpp \
  MenuState.h \
  Platform.cpp \
  Platform.h \
  PlayingState.cpp \
  PlayingState.h \
  Projectile.cpp \
  Projectile.h \
  scaler.cpp \
  scaler.h \
  Singleton.cpp \
  Singleton.h \
  State.cpp \
  State.h
Arkanoid_CPPFLAGS = $(AM_CPPFLAGS) $(pkg_libs_CFLAGS)
Arkanoid_LDADD = \
  $(pkg_libs_LIBS)
