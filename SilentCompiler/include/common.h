#pragma once
typedef unsigned long long int uint64;

typedef struct SilentId{
    char* str;
}SilentId;

typedef struct SymbolTable{
    SilentId* symbols;
    uint64 count;
    uint64 allocated;
}SymbolTable;

typedef struct SilentIdLink SilentIdLink;
typedef struct SilentIdLink{
    char* id;
    SilentIdLink* next;
}SilentIdLink;

typedef struct SymbolTableLink{
    SilentIdLink* root;
    uint64 count;
}SymbolTableLink;


SymbolTable* SymbolTableInit();
SymbolTable* SymbolTableLoad(SymbolTableLink *tableLink);
void SymbolTableFree(SymbolTable *table);
char SymbolTableAppend(SymbolTable *table, SilentId item);


SymbolTableLink* SymbolTableLinkInit();
void SymbolTableLinkFree(SymbolTableLink* table);
void SymbolTableLinkInsert(SymbolTableLink* table, char** str);

