#ifndef ENET_PLUS_DLL_HPP_
#define ENET_PLUS_DLL_HPP_

#ifdef _MSC_VER
  #ifdef ENET_PLUS_DLL
    #define ENET_PLUS_DECL __declspec(dllexport)
  #else
    #define ENET_PLUS_DECL __declspec(dllimport)
  #endif
#else
  #define ENET_PLUS_DECL
#endif

#endif // ENET_PLUS_DLL_HPP_
