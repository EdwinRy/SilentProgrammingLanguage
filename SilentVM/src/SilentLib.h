typedef void (*LibFunc)();

void* SilentLoadLibrary(char* path);
LibFunc SilentLoadFunc(void* lib, char* name);
void SilentFreeLibrary(void* lib);