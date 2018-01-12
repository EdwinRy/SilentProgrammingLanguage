#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "SilentVector.h"

//Create new dynamically allocated vector (list)
vector* createVector(int dataSize, int reserveSize)
{
    vector* vec = malloc(sizeof(vector));
    if(vec == NULL)
    {
        printf("Couldn't allocate memory\n");
        exit(1);
    }
    vec->voidPtr = malloc(dataSize*reserveSize);
    vec->dataCount = 0;
    vec->dataSize = dataSize;
    vec->reserved;
    return vec;
}

//Append an item to the end of the list
void vectorPushBack(vector* vec, void* data)
{
    char* temp = realloc(vec->characters, (vec->dataCount+1)*vec->dataSize);
    if(temp == NULL)
    {
        printf("Couldn't reallocate memory\n");
        free(vec);
        exit(1);
    }
    vec->characters = temp;
    memcpy(
        vec->characters + (vec->dataCount*vec->dataSize),
        (int*)data,
        vec->dataSize);
    vec->dataCount+=1;
}

//Remove an item at the end of the list
void vectorPopBack(vector* vec)
{
    char* temp = realloc(vec->characters, (vec->dataCount-=1)*vec->dataSize);
    if(temp == NULL)
    {
        printf("Couldn't reallocate memory\n");
        free(vec);
        exit(1);
    }
    vec->characters = temp;
}


//Remove an item at a given index
void vectorRemove(vector* vec, int index)
{
    if(index == vec->dataCount)
    {
        vectorPopBack(vec);
    }
    else
    {
        char* temp = malloc(vec->dataSize * vec->dataCount);
        if(temp == NULL)
        {
            printf("Couldn't allocate memory\n");
            exit(1);
        }
        memcpy(temp,vec->characters,vec->dataSize * vec->dataCount);
        char* temp2 = realloc(vec->characters, (vec->dataCount-=1)*vec->dataSize);
        if(temp2 == NULL)
        {
            printf("Couldn't reallocate memory\n");
            free(vec);
            exit(1);
        }
        vec->characters = temp2;
        if(index == 0)
        {
            memcpy(vec->characters,temp + vec->dataSize,vec->dataCount*vec->dataSize);
        }
        else
        {
            memcpy(vec->characters,temp,index * vec->dataSize);
            memcpy(
                vec->characters + (index*vec->dataSize),
                temp+(index*vec->dataSize)+vec->dataSize,
                (vec->dataCount - index) * vec->dataSize
            );
        }
    }
    
}

//Insert an element at an index
void vectorInsert(vector* vec, void* data, int index)
{
    if(index == vec->dataCount)
    {
        vectorPushBack(vec,data);
    }
    else
    {
        char* temp = malloc(vec->dataSize * vec->dataCount);
        if(temp == NULL)
        {
            printf("Couldn't allocate memory\n");
            exit(1);
        }
        memcpy(temp,vec->characters,vec->dataSize * vec->dataCount);
        char* temp2 = realloc(vec->characters,(vec->dataCount+=1)*vec->dataSize);
        if(temp2 == NULL)
        {
            printf("Couldn't reallocate memory\n");
            free(vec);
            exit(1);
        }
        vec->characters = temp2;
        if(index == 0)
        {
            memcpy(vec->characters,data,vec->dataSize);
            memcpy(vec->characters+vec->dataSize,temp,vec->dataCount*vec->dataSize);
        }
        else
        {
            memcpy(vec->characters,temp,index * vec->dataSize);
            memcpy(
                vec->characters + (index * vec->dataSize),
                data,
                vec->dataSize);
            memcpy(
                vec->characters + ((index+1)*vec->dataSize),
                temp+(index*vec->dataSize),
                (vec->dataCount - index - 1) * vec->dataSize);
        }
    }
}

void vectorDelete(vector* vec)
{
    free(vec->characters);
    free(vec);
}

void* vectorGet(vector* vec,int index)
{
    return (void*)(vec->characters+ (index * vec->dataSize));
}
