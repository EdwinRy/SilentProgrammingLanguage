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
        vector->vectorSize += vector->reallocSize;
        vector->spaceLeft += vector->reallocSize;
        void* temp = realloc(vector->data, vector->vectorSize);
        if(temp == NULL)
        {
            printf("Couldn't reallocate memory, press any key to reattempt\n");
            getchar();
            vectorResize(vector);
        }
        else
        {
            vector->data = temp;
        }
    }
    else if(vector->spaceLeft > vector->reallocSize)
    {
        vector->vectorSize -= vector->reallocSize;
        vector->spaceLeft -= vector->reallocSize;
        void* temp = realloc(vector->data, vector->vectorSize);
        if(temp == NULL)
        {
            printf("Couldn't reallocate memory, press any key to reattempt\n");
            getchar();
            vectorResize(vector);
        }
        else
        {
            vector->data = temp;
        }
    }
}

SilentVector* SilentCreateVector(uint64 reallocSize, uint64 dataSize)
{
    SilentVector* vector = malloc(sizeof(SilentVector));
    vector->data = malloc(reallocSize);
    vector->ptr = 0;
    vector->spaceLeft = reallocSize;
    vector->vectorSize = reallocSize;
    vector->dataSize = dataSize;
    vector->reallocSize = reallocSize;
    return vector;
}

void SilentPushBack(SilentVector* vector, void* data)
{
    vectorResize(vector);
    memcpy((char*)vector->data + vector->ptr, data, vector->dataSize);
    vector->ptr += vector->dataSize;
    vector->spaceLeft -= vector->dataSize;
}

void SilentPopBack(SilentVector* vector)
{
    vector->ptr -= vector->dataSize;
    vector->spaceLeft += vector->dataSize;
    vectorResize(vector);
}

void SilentDeleteVector(SilentVector* vector)
{
    free(vector->data);
    free(vector);
}