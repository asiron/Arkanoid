#ifndef __Arkanoid__FpsCounter__
#define __Arkanoid__FpsCounter__

class FpsCounter
{
 public:
  FpsCounter (int FPS_rate) : fps (0), lastTime (0), lastFrame (0), FPS (0)
  {
    frameDelay = static_cast<int>(1000.0/static_cast<double>(FPS_rate));
  };
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
