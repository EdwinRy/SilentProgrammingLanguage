#include <stdio.h>
#include <ctype.h>
typedef enum silentType
{
    
}silentType;
typedef struct silentInstruction
{
    char* data;

}silentType;
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
        char buffer[255];
        char* instructions[2];
        for(int i = 0; i < size+1;i++)
        {
            if(isalpha(line[i]))
            {
                //Number of characters
			    unsigned char count = 0;
			    //prepare space for the value
			    char* value;
			    //As long as the current character isn't whitespace
			    while(isalpha(line[i+count]) ||
				    isdigit(line[i+count]))
			    {
				    //Assign buffer to the current character
				    buffer[count++] = line[i+count];
			    }
			    //Allocate space for the value and terminator
			    value = malloc(count+1);
			    //Copy the value from the buffer
			    memcpy(value,buffer,count);
			    value[count] = '\0';
			    token.value = value;
			    token.type = silentIdentifierToken;
            }
        }
    }

    return 1;
}

int main()
{
    char* in = "";
    char* out = "";
    assemble(in, out);
}