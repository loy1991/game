#ifndef BL_MUTEX_H
#define BL_MUTEX_H

#include <stdlib.h>
#include <pthread.h>
#include <iostream>

class BL_Mutex
{
public:
  BL_Mutex()
    : _isCreated( false )
  {
    pthread_mutex_init( &_mutex, NULL );
    _isCreated = true;
  }
  ~BL_Mutex()
  {
    if ( _isCreated )
    {
      pthread_mutex_destroy ( &_mutex );
    }
  }
public:
  void lock()
  {
    pthread_mutex_lock( &_mutex );
  }
  void unlock()
  {
    pthread_mutex_unlock ( &_mutex );
  };
  void trylock()
  {
    pthread_mutex_trylock ( &_mutex );
  }
private:
  bool _isCreated;
  pthread_mutex_t      _mutex;
};

class BL_Lock
{
  public:
    BL_Lock( BL_Mutex *bl_mutex )
      : _lpMutex ( bl_mutex )
    {
      if ( _lpMutex )
        _lpMutex->lock();
      std::cout << "BL_Lock" << std::endl;
    }
    ~BL_Lock()
    {
      if ( _lpMutex )
      {
        _lpMutex->unlock();
        std::cout << "~BL_Lock" << std::endl;
      }
    }
  private:
    BL_Mutex *_lpMutex;
};

#endif // BL_MUTEX_H
