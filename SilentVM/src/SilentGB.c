#include "SilentGB.h"
#include "SilentVM.h"
#include <stdlib.h>

SilentGB* createSilentGB()
{
    SilentGB* gb = malloc(sizeof(SilentGB));
    gb->pointers = createVector(sizeof(void*));
    gb->currentMark = 0;
    return gb;
}

void silentSweep(SilentGB* gb, SilentMemory* memory)
{
    gb->currentMark = (gb->currentMark == 0) ? 1 : 0;

    silentBlock** gbData = (silentBlock**)(gb->pointers->voidPtr);
    silentBlock** storageData = memory->storage;
    for(int i = 0; i < memory->storageSize; i++)
    {
        if(storageData[i] != NULL)
        {
            storageData[i]->marked = gb->currentMark;
        }
    }

    for(int i = 0; i < gb->pointers->dataCount; i++)
    {
        if(gbData[i]->marked != gb->currentMark)
        {
            free(gbData[i]->data);
            free(gbData[i]);
        }
    }


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