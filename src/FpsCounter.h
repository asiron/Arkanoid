#ifndef __Arkanoid__FpsCounter__
#define __Arkanoid__FpsCounter__

class FpsCounter
{
 public:
  FpsCounter (int);

  bool measureFPS (); // return value: render next frame ?
  inline int getFPS () { return FPS; }

  void RenderFPS (int, int);

 private:
  int fps;
  int lastTime;
  int lastFrame;
  int FPS;
  int frameDelay; // ms
};

#endif /* defined(__Arkanoid__FpsCounter__) */
