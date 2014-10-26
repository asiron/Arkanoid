#ifndef __Arkanoid__FpsCounter__
#define __Arkanoid__FpsCounter__

class FpsCounter
{
 public:
  FpsCounter (int);

  int measureFPS ();
  int getFPS ();

 private:
  int fps;
  int lastTime;
  int lastFrame;
  int FPS;
  int frameDelay;
};

#endif /* defined(__Arkanoid__FpsCounter__) */
