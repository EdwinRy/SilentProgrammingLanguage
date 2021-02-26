typedef void (*LibFunc)(void* ptr);

void* SilentLoadLibrary(char* path);
LibFunc SilentLoadFunc(void* lib, char* name);
void SilentFreeLibrary(void* lib);