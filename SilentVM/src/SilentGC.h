#ifndef SILENT_GB
#define SILENT_GB
#include "SilentVM.h"

typedef struct SilentGC
{
    SilentMemory* memory;
}SilentGC;

SilentGC* createSilentGC(SilentMemory* memory);
void SilentSweep(SilentGC* gc);
void* SilentAlloc(SilentGC* gc, unsigned long long size);
#endif