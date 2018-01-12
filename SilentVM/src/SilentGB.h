#include "SilentVector.h"
#ifndef SILENT_GARBAGE_COLLECTOR
#define SILENT_GARBAGE_COLLECTOR

typedef struct silentBlock
{
    char marked;
    char* data;
}silentBlock;

typedef struct SilentGB
{
    vector* pointers;
    char currentMark;
}SilentGB;

SilentGB* createSilentGB();
void silentSweep(SilentGB* gb, char** storage);
void silentSavePointer(SilentGB* gb, void* ptr);
void silentDeleteGB(SilentGB* gb);
#endif