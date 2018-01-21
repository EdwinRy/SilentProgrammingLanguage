#include "SilentGB.h"
#include "SilentVM.h"
#include <stdlib.h>

SilentGB* createSilentGB(int size)
{
    SilentGB* gb = malloc(sizeof(SilentGB));
    gb->pointers = createVector(sizeof(void*));
    gb->size = size;
    gb->currentMark = 0;
    return gb;
}

void silentSweep(SilentGB* gb, SilentMemory* memory, int localStorage)
{
    printf("sweeping\n");
    gb->currentMark = (gb->currentMark == 0) ? 1 : 0;
    silentBlock** gbData = (silentBlock**)(gb->pointers->voidPtr);
    silentBlock** storageData = memory->storage;
    printf("localSt %i\n",localStorage);
    for(int i = 0; i < localStorage; i++)
    {
        printf("here\n");
        if(storageData[i] != NULL)
        {
            storageData[i]->marked = gb->currentMark;
        }
    }

    for(int i = 0; i < gb->pointers->dataCount; i++)
    {
        printf("here2\n");
        if(gbData[i] != NULL)
        {
            if(gbData[i]->marked != gb->currentMark)
            {
                free(gbData[i]->data);
                free(gbData[i]);
                printf("freed\n");
            }
        }
    }


}

void silentSavePointer(SilentGB* gb, void* ptr)
{
    vectorPushBack(gb->pointers, &ptr);
}

void silentDeletePointer(SilentGB* gb, void* ptr)
{
    silentBlock** gbData = (silentBlock**)(gb->pointers->voidPtr);
    for(int i = 0; i < gb->pointers->dataCount; i++)
    {
        if(ptr == gbData[i]->data)
        {
            free(gbData[i]->data);
            free(gbData[i]);
            break;
        }
    }
}

void silentDeleteGB(SilentGB* gb)
{
    vectorDelete(gb->pointers);
    free(gb);
}