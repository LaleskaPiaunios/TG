#ifndef __EXTERNALPLUGIN_CONFIG_H
#define __EXTERNALPLUGIN_CONFIG_H

#define EXTERNALPLUGIN_PLUGIN_NAME "externalplugin"
#ifdef WIN32
  #ifdef EXTERNALPLUGINDLL
    #define EXTERNALPLUGINEXPORT  __declspec(dllexport)   // export DLL information
  #else
    #define EXTERNALPLUGINPEXPORT  __declspec(dllimport)   // import DLL information
  #endif 
#else
  #define EXTERNALPLUGINEXPORT
#endif

#endif  // __EXTERNALPLUGIN_CONFIG_H

