#ifndef DEFINES_H
#define DEFINES_H

#if defined (ACE_LINUX)
#define RESOURCE_DIRECTORY        "../Arkanoid/data"
#else
#define RESOURCE_DIRECTORY        "data"
#endif // ACE_LINUX
#define GRAPHICS_DIRECTORY        "graphics"
#define SOUNDS_DIRECTORY          "sounds"

#define BALL_VELOCITY_INCREMENT   0.2f
#define PLATFORM_BASE_SPEED       8.0f
#define PLATFORM_DEFAULT_LIVES    5

#define SOUNDS_MIDI_EXT           "mid"
#define SOUNDS_MP3_EXT            "mp3"
#define SOUNDS_MUSIC_FADE_PERIOD  1000 // ms
#define SOUNDS_MUSIC_VOLUME       16   // [0-128]
//#define SOUNDS_DEF_FREQUENCY      48000
#define SOUNDS_DEF_CHANNELS       10

#define SMALL_FONT_SIZE           14  // point size
#define MEDIUM_FONT_SIZE          28  // point size
#define LARGE_FONT_SIZE           35  // point size
#define HUGE_FONT_SIZE            70  // point size

#define BASE_SCREEN_X             960
#define BASE_SCREEN_Y             540
#define BASE_GAME_FPS             30 // try 60 for smooth gameplay (a little too fast for me)
#define DEFAULT_CONTROL_TYPE      KEYBOARD
#define DEFAULT_MAP_FILE          "map.cfg"

#define DEFAULT_BALLS_MAX         10

#define DEFAULT_EFFECTS_MAX       4

#define DEFAULT_PROJECTILES_MAX   10
#define DEFAULT_PROJECTILES_SPEED 15

#define KEYBOARD                  0
#define MOUSE                     1

#define WINDOW_CAPTION            "Arkanoid"

#endif // DEFINES_H
