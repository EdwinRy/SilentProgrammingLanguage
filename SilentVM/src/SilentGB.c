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
        printf("storage %i\n",*storageCount);
        printf("sweeping\n");

        gb->currentMark             = (gb->currentMark == 0) ? 1 : 0;
        silentBlock** gbData        = gb->pointers->voidPtr;
        silentBlock** storageData   = memory->storage;

        printf("marking\n");
        for(int i = 0; i < *storageCount; i++)
        {
            if(storageData[i] != NULL)
            {
                storageData[i]->marked = gb->currentMark;
                //printf("here\n");
                //printf("%i\n",*storageData[i]->data);
            }
        }

        printf("freeing\n");
        for(int i = 0; i < gb->pointers->dataCount; i++)
        {
            if(gbData[i] != NULL)
            {
                if(gbData[i]->marked != gb->currentMark)
                {          
                    void* dataPtr   = gbData[i]->data;
                    void* objPtr    = gbData[i]; 
                    vectorRemove(gb->pointers->voidPtr,i);       
                    //printf("%i\n",*gbData[i]->data);
                    free(gbData[i]->data);
                    free(gbData[i]);
                    //*storageCount -= 1;
                }
            }
        }
    }else{
        printf("not worth sweeping\n");
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