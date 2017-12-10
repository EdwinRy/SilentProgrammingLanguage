#include <stdio.h>
#include <ctype.h>
char assemble(char* inFile, char* outFile)
{
    FILE* f;
    f = fopen(inFile,"r");
    if(f==NULL)
    {
        return 0;
        printf("file %s could not be opened\n", inFile);
    }

    char* line;
    size_t s = 0;
    int size = 0;
    size = getline(&line,&s,f);

    while(size != -1)
    {
        
    }

    return 1;
}

int main()
{
    char* in = "";
    char* out = "";

}