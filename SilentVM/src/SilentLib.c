#include "SilentLib.h"
#include <stdio.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

void* SilentLoadLibrary(char* path)
{
    void* library;
    #ifdef _WIN32
    library = LoadLibrary(path);
    #else
    library = dlopen(path, RTLD_LAZY);
    #endif
    if(!library){ printf("Couldn't load library: %s\n", path); return NULL; }
    return library;
}

LibFunc SilentLoadFunc(void* lib, char* name)
{
    LibFunc function;
    #ifdef _WIN32
    function = (LibFunc)GetProcAddress(lib,name);
    #else
    function = (LibFunc)dlsym(lib, name);
    #endif
    if(!function){ printf("Couldn't load function: %s\n", name); return NULL; }
    return function;
}

void SilentFreeLibrary(void* lib)
{
    #ifdef _WIN32
    FreeLibrary(lib);
    #else
    dlclose(lib);
    #endif
}