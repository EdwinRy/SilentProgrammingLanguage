#include "SilentGC.h"
SilentGC* createSilentGB(SilentMemory* memory)
{
    SilentGC* gc = malloc(sizeof(SilentGC));
    gc->memory = memory;
    return gc;
}

void SilentSweep(SilentGC* gc)
{

}

void* SilentAlloc(SilentGC* gc, unsigned long long size)
{
    
}