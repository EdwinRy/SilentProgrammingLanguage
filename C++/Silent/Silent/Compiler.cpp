#include "Compiler.h"

Silent::Compiler::Compiler()
{
}

void Silent::Compiler::compile(const char *filePath, const char *outputPath)
{

    char *file_contents;
    long input_file_size;


    FILE *input_file = fopen(filePath, "rb");
    fseek(input_file, 0, SEEK_END);
    input_file_size = ftell(input_file);
    rewind(input_file);
    file_contents = new char[input_file_size];
    fread(file_contents, sizeof(char), input_file_size, input_file);
    fclose(input_file);

}

