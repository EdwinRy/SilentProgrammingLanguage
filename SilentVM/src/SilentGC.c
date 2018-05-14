#include "SilentGC.h"
#include "SilentVM.h"
#include <math.h>
#include <string.h>
#include <stdio.h>
typedef long long uint64;
SilentGC* createSilentGC(SilentMemory* memory)
{
    SilentGC* gc = malloc(sizeof(SilentGC));
    gc->memory = memory;
    return gc;
}

void SilentSweep(SilentGC* gc)
{

}

void SilentFree(SilentGC* gc, uint64* ptr)
{

}

void* SilentAlloc(SilentGC* gc, uint64 size)
{
    SilentMemory* mem = gc->memory;
    if(mem->heapSize - mem->heapPointer < 2)
    {
        SilentSweep(gc);
        if(mem->heapSize - mem->heapPointer < 2)
        {
            mem->heapSize = ceil(mem->heapSize/10)+mem->heapSize;
            void* temp = realloc(mem->heap,mem->heapSize);
            if(temp != NULL)
            {
                mem->heap = temp;
            }
            else
            {
                printf("Couldn't allocate more heap memory\n");
                printf("Press any key to retry...\n");
                getchar();
                return SilentAlloc(gc,size);
            } 
        }
    }
    for(uint64 i = 0; i < mem->heapSize; i++)
    {
        if(mem->heap[i].occupied == 0)
        {
            mem->heap[i].data = malloc(size);
            return mem->heap + i;
            break;
        }
    }
    return NULL;
}