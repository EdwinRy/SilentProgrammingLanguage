#ifndef SILENT_HELPER
#define SILENT_HELPER
typedef struct vector
{
    union
    {
        int* integers;
        float* floats;
        char** strings;
        char* characters;
        void* voidPtr;
    };

    //bytes per element
    unsigned int dataSize;
    unsigned int dataCount;

}vector;

char* readAllText(char* path);
vector* createVector(int dataSize);
void vectorPushBack(vector* vec, void* data);
void vectorPopBack(vector* vec);
void vectorInsert(vector* vec, void* data, int index);
void vectorRemove(vector* vec, int index);
#endif