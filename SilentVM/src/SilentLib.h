void* SilentLoadLibrary(char* path);
void* SilentLoadFunc(void* lib, char* name);
void SilentFreeLibrary(void* lib);