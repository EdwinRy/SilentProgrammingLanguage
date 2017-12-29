#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SilentHelper.h"
//HELPER
//Read all characters in a file
char* readAllText(char* path)
{
    //Text buffer to be returned
    char* text;
    //File to be read from
	FILE *f;
    //If opening of the file fails
	if((f = fopen(path,"r"))==NULL)
	{
        //output about file not existing
		printf("File %s doesn't exist!\n",path);
        //Close the program
		exit(-1);
	}
    //Seek the end of the file
	fseek(f,0,SEEK_END);
    //Allocate enough space for the entire file
	text = malloc(ftell(f));
    //Rewind the file back to beginning
	fseek(f,0,SEEK_SET);
    //Declare counter for the character array
	long count = 0;
    //Initialise buffer for single character
	char c;
    //Iterate for as long as the character is not
    //the end of the file
	while((c = fgetc(f))!=EOF)
	{
        //Add the character
		text[count++] = (char)c;
	}
    //Close the file
    fclose(f);
    //Return the source
    return text;
}

//Create new dynamically allocated vector (list)
vector* createVector(int dataSize)
{
    vector* vec = malloc(sizeof(vector));
    if(vec == NULL)
    {
        printf("Couldn't allocate memory\n");
        exit(1);
    }
    vec->integers = malloc(dataSize);
    vec->dataCount = 0;
    vec->dataSize = dataSize;
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
    vector* temp = malloc(sizeof(vector));
    temp->characters = malloc(vec->dataSize * vec->dataCount);
    if(temp == NULL)
    {
        printf("Couldn't allocate memory\n");
        exit(1);
    }
    memcpy(temp->characters,vec->characters,vec->dataSize * vec->dataCount);
    char* temp2 = realloc(vec->characters, (vec->dataCount-=1)*vec->dataSize);
    if(temp2 == NULL)
    {
        printf("Couldn't reallocate memory\n");
        free(vec);
        exit(1);
    }
    vec->characters = temp2;
    unsigned int count = 0;
    for(int i = 0; i < vec->dataCount; i++)
    {
        if(i == index){count++;}
        memcpy(
            vec->characters + (vec->dataSize * i),
            temp->characters + (vec->dataSize * count),
            vec->dataSize
        );
        count++;
    }
}

//Insert an element at an index
void vectorInsert(vector* vec, void* data, int index)
{
    vector* temp = malloc(sizeof(vector));
    temp->characters = malloc(vec->dataSize * vec->dataCount);
    if(temp == NULL)
    {
        printf("Couldn't allocate memory\n");
        exit(1);
    }
    memcpy(temp->characters,vec->characters,vec->dataSize * vec->dataCount);
    char* temp2 = realloc(vec->characters, (vec->dataCount+=1)*vec->dataSize);
    if(temp2 == NULL)
    {
        printf("Couldn't reallocate memory\n");
        free(vec);
        exit(1);
    }
    vec->characters = temp2;
    unsigned int count = 0;
    for(int i = 0; i < vec->dataCount; i++)
    {
        if(i == index)
        {
            memcpy(
            vec->characters + (vec->dataSize * i),
            data,
            vec->dataSize
            );
            continue;
        }
        memcpy(
            vec->characters + (vec->dataSize * i),
            temp->characters + (vec->dataSize * count),
            vec->dataSize
        );
        count++;
    }
}