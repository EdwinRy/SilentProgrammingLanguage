#pragma once
#include <iostream>
#include <fstream>
#include <string.h>

// Read all characters from parameter passed path
char* readAllText(char* path)
{
    char* text;
    FILE *f;

    fopen_s(&f,path,"r");
    if(f == NULL)
    {
        printf("File %s could not be opened!\n", path);
        exit(-1);
    }
    fseek(f,0,SEEK_END);
    text = (char*)malloc(ftell(f) + 1l);
    if (text != NULL)
    {

        fseek(f,0,SEEK_SET);
        long count = 0;
        char c;
        while((c = fgetc(f))!=EOF) { text[count++] = (char)c; }
        text[count] = '\0';
    }
    fclose(f);
    return text;
}

// Write a string into the file at path 
void writeAllText(const char* path, std::string text)
{
    std::ofstream outFile;
    outFile.open(path, std::ios::out | std::ios::binary);
    outFile << text;
    outFile.close();
}

// Read binary from file
char* readAllBytes(char* path)
{
    char* text;
    FILE *f;

    fopen_s(&f, path, "rb");
    if(f == NULL)
    {
        printf("File %s could not be opened!\n", path);
        exit(-1);
    }
    fseek(f,0,SEEK_END);
    text = (char*)malloc(ftell(f) + 1);
    fseek(f,0,SEEK_SET);
    long count = 0;
    char c;
    while((c = fgetc(f))!=EOF) { text[count++] = (char)c; }
    text[count] = '\0';
    fclose(f);
    return text;
}

// Write binary to file
void writeAllBytes(const char* path, const char* text, unsigned long long size)
{
    std::ofstream outFile (path, std::ofstream::binary);
    outFile.write(text, size);
    outFile.close();
}