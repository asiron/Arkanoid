#include "stdafx.h"

#include "scaler.h"

#include <iostream>

#include "SDL_image.h"

#include "ace/OS.h"

SDL_Surface*
LoadScaledBitmap (const char* filename, int width, int height)
{
  // load image to temp file
  SDL_Surface *temp = IMG_Load (filename);
  if (!temp)
  {
    std::cerr << ACE_TEXT_ALWAYS_CHAR ("IMG_Load: ") << IMG_GetError () << std::endl;
    ACE_OS::exit (1);
  } // end IF

  // apply alpha channel
#if defined (SDL1_USE)
  SDL_Surface* temp2 = SDL_DisplayFormatAlpha (temp);
#elif defined (SDL2_USE)
  SDL_Surface* temp2 = SDL_ConvertSurfaceFormat (temp,
                                                 SDL_PIXELFORMAT_RGBA8888,
                                                 0);
#endif // SDL1_USE || SDL2_USE
  ACE_ASSERT (temp2);

  // scale bitmap
  SDL_Surface* image = ScaleSurface (temp2, width, height);
  ACE_ASSERT (image);

  // delete junk
  SDL_FreeSurface (temp);
  SDL_FreeSurface (temp2);

  // returning pointer to scaled bitmap
  return image;
}

SDL_Surface*
ScaleSurface (SDL_Surface* Surface, int Width, int Height)  // doesnt delete source bitmap, but we deal with it in LoadScaledBitmap
{
  //// if source is null pointer or height/width is not specified then return NULL
  //if (!Surface ||
  //    !Width   || !Height)
  //  return NULL;

  // create bitmap for a scaled bitmap
  SDL_Surface* _ret =
    SDL_CreateRGBSurface (Surface->flags, Width, Height, Surface->format->BitsPerPixel,
                          Surface->format->Rmask, Surface->format->Gmask, Surface->format->Bmask, Surface->format->Amask);

  // calculate stretching factor for x  and y
  float _stretch_factor_x = (Width  / static_cast<float> (Surface->w)),
        _stretch_factor_y = (Height / static_cast<float> (Surface->h));

  // perform scaling for every pixel of source map
  for (int y = 0; y < Surface->h; y++)
    for (int x = 0; x < Surface->w; x++)
      for (int o_y = 0; o_y < _stretch_factor_y; ++o_y)
        for (int o_x = 0; o_x < _stretch_factor_x; ++o_x)
          putpixel (_ret,
                    static_cast<int> (_stretch_factor_x * x) + o_x,
                    static_cast<int> (_stretch_factor_y * y) + o_y,
                    getpixel (Surface, x, y));
    
  // return scaled bitmap
  return _ret;
}

SDL_Surface*
LoadCroppedBitmap (const char* filename_in, const struct SDL_Rect& rect_in)
{
  // load image to temp file
  SDL_Surface *temp = IMG_Load (filename_in);
  if (!temp)
  {
    std::cerr << ACE_TEXT_ALWAYS_CHAR ("IMG_Load: ") << IMG_GetError () << std::endl;
    ACE_OS::exit (1);
  } // end IF

  // apply alpha channel
#if defined (SDL1_USE)
  SDL_Surface* temp2 = SDL_DisplayFormatAlpha (temp);
#elif defined (SDL2_USE)
  SDL_Surface* temp2 = SDL_ConvertSurfaceFormat (temp,
                                                 SDL_PIXELFORMAT_RGBA8888,
                                                 0);
#endif // SDL1_USE || SDL2_USE
  ACE_ASSERT (temp2);

  // scale bitmap
  SDL_Surface* image = CropSurface (temp2, rect_in);
  ACE_ASSERT (image);

  // delete junk
  SDL_FreeSurface (temp);
  SDL_FreeSurface (temp2);

  // returning pointer to scaled bitmap
  return image;
}

SDL_Surface*
CropSurface (SDL_Surface* surface, const struct SDL_Rect& rect_in)
{ ACE_ASSERT (rect_in.w <= surface->w && rect_in.h <= surface->h);

  // create bitmap for a cropped bitmap
  SDL_Surface* _ret =
    SDL_CreateRGBSurface (surface->flags, rect_in.w, rect_in.h, surface->format->BitsPerPixel,
                          surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, surface->format->Amask);

  // crop pixels out of surface
  for (int y = 0; y < rect_in.h; y++)
    for (int x = 0; x < rect_in.w; x++)
      putpixel (_ret,
                x, y,
                getpixel (surface, rect_in.x + x, rect_in.y + y));
    
  // return cropped bitmap
  return _ret;
}

// Functions taken from SDL site
///////////////////////////////////////////////

Uint32
getpixel (SDL_Surface* surface, int x, int y)
{
  /* Here p is the address to the pixel we want to retrieve */
  Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * surface->format->BytesPerPixel;

  switch (surface->format->BytesPerPixel)
  {
    case 1:
      return *p;
    case 2:
      return *(Uint16 *)p;
    case 3:
      if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
        return p[0] << 16 | p[1] << 8 | p[2];
      else
        return p[0] | p[1] << 8 | p[2] << 16;
    case 4:
      return *(Uint32 *)p;
    default:
      break;
  }

  return 0; /* shouldn't happen, but avoids warnings */
}

void
putpixel (SDL_Surface* surface, int x, int y, Uint32 pixel)
{
  /* Here p is the address to the pixel we want to set */
  Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * surface->format->BytesPerPixel;

  switch (surface->format->BytesPerPixel)
  {
    case 1:
      *p = pixel;
      break;
    case 2:
      *(Uint16*)p = pixel;
      break;
    case 3:
      if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
      {
        p[0] = (pixel >> 16) & 0xff;
        p[1] = (pixel >> 8) & 0xff;
        p[2] = pixel & 0xff;
      }
      else
      {
        p[0] = pixel & 0xff;
        p[1] = (pixel >> 8) & 0xff;
        p[2] = (pixel >> 16) & 0xff;
      }
      break;
    case 4:
      *(Uint32*)p = pixel;
      break;
  }
}
