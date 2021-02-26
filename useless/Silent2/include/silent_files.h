#pragma once
#include <stdio.h>
#include <string.h>

// Read all characters from parameter passed path
char* readAllText(char* path)
{
    char* text;
    FILE* f = fopen(path, "r");

    if (f == NULL)
    {
        printf("File %s could not be opened!\n", path);
        return NULL;
    }
    fseek(f, 0, SEEK_END);
    text = malloc(ftell(f) + 1ll);
    if (text != NULL)
    {

        fseek(f, 0, SEEK_SET);
        long count = 0;
        char c;
        while ((c = fgetc(f)) != EOF) { text[count++] = (char)c; }
        text[count] = '\0';
    }
    fclose(f);
    return text;
}

// Read binary from file
char* readAllBytes(char* path)
{
    char* text;
    FILE* f = fopen(path, "rb");

    if (f == NULL)
    {
        printf("File %s could not be opened!\n", path);
        exit(-1);
    }
    fseek(f, 0, SEEK_END);
    text = (char*)malloc(ftell(f) + 1);
    fseek(f, 0, SEEK_SET);
    long count = 0;
    char c;
    while ((c = fgetc(f)) != EOF) { text[count++] = (char)c; }
    text[count] = '\0';
    fclose(f);
    return text;
}