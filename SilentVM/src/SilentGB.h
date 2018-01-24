#include "SilentVector.h"
#ifndef SILENT_GARBAGE_COLLECTOR
#define SILENT_GARBAGE_COLLECTOR

typedef struct SilentMemory SilentMemory;

typedef struct silentBlock
{
    char marked;
    void* data;
}silentBlock;

typedef struct SilentGB
{
    vector* pointers;
    char currentMark;
    int size;
}SilentGB;

SilentGB* createSilentGB();
void silentSweep(SilentGB* gb, SilentMemory* memory, int* storageCount);
void silentSavePointer(SilentGB* gb, void* ptr);
void silentDeletePointer(SilentGB* gb, void* ptr);
void silentDeleteGB(SilentGB* gb);
#endif