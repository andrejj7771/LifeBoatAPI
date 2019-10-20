#ifndef LIFEBOATAPI_GLOBAL_H
#define LIFEBOATAPI_GLOBAL_H

#ifdef _WIN32
#  ifdef LIFEBOAT_API_EXPORTS
#    define LIFEBOAT_API __declspec(dllexport)
#  else
#    define LIFEBOAT_API __declspec(dllimport)
#  endif
#else
#  ifdef LIFEBOAT_API_EXPORTS
#    define LIFEBOAT_API __attribute__((visibility("default")))
#  else
#    define LIFEBOAT_API  __attribute__((visibility ("hidden")))
#  endif
#endif

#endif // LIFEBOATAPI_GLOBAL_H
