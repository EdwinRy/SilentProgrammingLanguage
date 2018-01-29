#include "SilentGB.h"
#include "SilentVM.h"
#include <stdlib.h>

SilentGB* createSilentGB(int size)
{
    SilentGB* gb    = malloc(sizeof(SilentGB));
    gb->pointers    = createVector(sizeof(void*));
    gb->size        = size;
    gb->currentMark = 0;
    return gb;
}

void silentSweep(SilentGB* gb, SilentMemory* memory, int* storageCount)
{
    //If more than half the realloc size is used
    if(*storageCount > memory->reallocSize / 2)
    {
        gb->currentMark             = (gb->currentMark == 0) ? 1 : 0;
        silentBlock** gbData        = gb->pointers->voidPtr;
        silentBlock** storageData   = memory->storage;
        int storageItems            = *storageCount;

        printf("marking\n");
        for(int i = 0; i < storageItems; i++)
        {
            if(storageData[i] != NULL)
            {
                storageData[i]->marked = gb->currentMark;
                printf("marked %i\n",*(int*)(storageData[i]->data));
            }
            else{
                storageItems+=1;
            }
        }

        printf("freeing data\n");
        for(int i = 0; i < gb->pointers->dataCount; i++)
        {
            if(gbData[i] != NULL)
            {
                if(gbData[i]->marked != gb->currentMark)
                {          
                    printf("freeing %i\n",*(int*)(gbData[i]->data));
                    void* dataPtr   = gbData[i]->data;
                    void* objPtr    = gbData[i]; 
                    vectorRemove(gb->pointers,i);       
                    free(dataPtr);
                    free(objPtr);
                    *storageCount -= 1;
                    i--;
                }
            }
        }
    }else{
        printf("not worth sweeping\n");
    }
}

void silentSavePointer(SilentGB* gb, void* ptr)
{
    vectorPushBack(gb->pointers, ptr);
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