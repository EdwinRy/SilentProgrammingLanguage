#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SilentVector.h"
typedef unsigned int uint;
typedef unsigned long long uint64;
typedef long long int64;

void vectorResize(SilentVector* vector)
{
    if(vector->spaceLeft < vector->dataSize)
    {
        uint64 tempSize = vector->vectorSize + vector->reallocSize;
        uint64 tempSpaceLeft = vector->spaceLeft + vector->reallocSize;
        void* temp = realloc(vector->data, tempSize);
        if(temp == NULL)
        {
            printf("Couldn't reallocate memory, press any key to reattempt\n");
            getchar();
            vectorResize(vector);
        }
        else
        {
            vector->vectorSize = tempSize;
            vector->spaceLeft = tempSpaceLeft;
            vector->data = temp;
        }
    }
    else if(vector->spaceLeft > vector->reallocSize)
    {
        uint64 tempSize = vector->ptr + vector->reallocSize;
        uint64 tempSpaceLeft = vector->reallocSize;
        void* temp = realloc(vector->data, tempSize);
        if(temp == NULL)
        {
            printf("Couldn't reallocate memory, press any key to reattempt\n");
            getchar();
            vectorResize(vector);
        }
        else
        {
            vector->vectorSize = tempSize;
            vector->spaceLeft = tempSpaceLeft;
            vector->data = temp;
        }
    }
}

void vectorReserve(SilentVector* vector, uint64 toReserve)
{
    uint64 tempSize = vector->vectorSize + toReserve;
    uint64 tempSpaceLeft = vector->spaceLeft + toReserve;

    void* temp = realloc(vector->data, tempSize);
    if(temp == NULL)
    {
        printf("Couldn't reallocate memory, press any key to reattempt\n");
        getchar();
        vectorReserve(vector, toReserve);
    }
    else
    {
        vector->vectorSize = tempSize;
        vector->spaceLeft = tempSpaceLeft;
        vector->data = temp;
    }
}

SilentVector* SilentCreateVector(uint64 reallocSize, uint64 dataSize)
{
    SilentVector* vector = malloc(sizeof(SilentVector));
    vector->data = calloc(reallocSize,1);
    vector->ptr = 0;
    vector->spaceLeft = reallocSize;
    vector->vectorSize = reallocSize;
    vector->dataSize = dataSize;
    vector->reallocSize = reallocSize;
    return vector;
}

// void SilentPushBack(SilentVector* vector, void* data)
// {
//     vectorResize(vector);
//     memcpy((char*)vector->data + vector->ptr, data, vector->dataSize);
//     vector->ptr += vector->dataSize;
//     vector->spaceLeft -= vector->dataSize;
// }

// void SilentPushMultiple(SilentVector* vector, uint64 count, void* data)
// {
//     vectorResize(vector);
//     uint64 toAppend = vector->dataSize * count;
//     if(toAppend > vector->spaceLeft)
//     {
//         vectorReserve(vector, toAppend - vector->spaceLeft);
//     }
//     memcpy(vector->data + vector->ptr, data, toAppend);
//     vector->spaceLeft -= toAppend;
//     vector->ptr += toAppend;
// }

// void SilentPopBack(SilentVector* vector)
// {
//     vector->ptr -= vector->dataSize;
//     vector->spaceLeft += vector->dataSize;
//     vectorResize(vector);
// }

// void SilentPopMultiple(SilentVector* vector, uint64 count)
// {
//     uint64 toRemove = vector->dataSize * count;
//     vector->ptr -= toRemove;
//     vector->spaceLeft += toRemove;
//     vectorResize(vector);
// }

void SilentDeleteVector(SilentVector* vector)
{
    //free(vector->data);
    //free(vector);
}