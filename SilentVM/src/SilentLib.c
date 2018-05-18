#include "SilentLib.h"
#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

typedef void (*dllFunc)();

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

void* SilentLoadLibraryFunction(void* lib, char* name)
{
    dllFunc function;
    #ifdef _WIN32
    function = (dllFunc)GetProcAddress(lib,name);
    #else
    function = (dllFunc)dlsym(lib, name);
    #endif
    if(!function){ printf("Couldn't load function: %s\n", name); return NULL; }
    return function;
}