#include "SilentGB.h"
#include <stdlib.h>

SilentGB* createSilentGB()
{
    SilentGB* gb = malloc(sizeof(SilentGB));
    gb->pointers = createVector(sizeof(void*));
    return gb;
}

void silentSweep(SilentGB* gb, char** storage)
{

}

void silentSavePointer(SilentGB* gb, void* ptr)
{
    vectorPushBack(gb->pointers, &ptr);
}

void silentDeleteGB(SilentGB* gb)
{
    vectorDelete(gb->pointers);
    free(gb);
}