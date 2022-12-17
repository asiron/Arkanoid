#ifndef __Arkanoid__Singleton__
#define __Arkanoid__Singleton__

#include <cassert>

template <typename T>
class Singleton
{
 public:
  Singleton () { assert (!ms_singleton); ms_singleton = static_cast <T*> (this); }
  virtual ~Singleton() { assert (ms_singleton); ms_singleton = NULL; }

  static T& GetSingleton () { assert (ms_singleton); return *ms_singleton; }
  static T* GetSingletonPtr () { return ms_singleton; }

 private:
  static T* ms_singleton;
};

template <typename T> T* Singleton <T>::ms_singleton = NULL;

#endif /* defined(__Arkanoid__Singleton__) */
