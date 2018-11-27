#pragma once
#include <iostream>
#include <fstream>
#include <string.h>
namespace Silent
{
    namespace File
    {
        char* ReadAllText(char* path)
        {
            char* text;
            FILE *f;

            if((f = fopen(path,"r"))==NULL)
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

        void WriteAllText(const char* path, std::string text)
        {
            std::ofstream outFile;
            outFile.open(path, std::ios::out | std::ios::binary);
            outFile << text;
            outFile.close();
        }

        char* ReadAllBytes(char* path)
        {
            char* text;
            FILE *f;

            if((f = fopen(path,"rb"))==NULL)
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

        void WriteAllBytes(const char* path, const char* text)
        {
            FILE *f;
            if((f = fopen(path,"wb"))==NULL)
            {
                printf("File %s could not be opened!\n", path);
                exit(-1);
            }
            fprintf(f, text);
            fclose(f);
        }
    }
}
