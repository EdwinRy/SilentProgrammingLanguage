#ifndef SILENT_VECTOR
#define SILENT_VECTOR

//Dynamic array structure
typedef struct vector
{
    //Using a pointer allows for the use of any datatype
    union
    {
        int* integers;
        float* floats;
        char** strings;
        char* characters;
        void* voidPtr;
    };

    //Bytes per item
    unsigned int dataSize;
    //Number of items int he array
    unsigned int dataCount;

}vector;
//Create new dynamic array
vector* createVector(int dataSize);
//Append to the back of the dynamic array
void vectorPushBack(vector* vec, void* data);
//Remove the last item of the dynamic array
void vectorPopBack(vector* vec);
//Insert an item at a given index
void vectorInsert(vector* vec, void* data, int index);
//Remove an item at an index
void vectorRemove(vector* vec, int index);
//Delete a vector (free)
void vectorDelete(vector* vec);
//Return a void pointer to an item at a specific index
void* vectorGet(vector* vec,int index);
#endif