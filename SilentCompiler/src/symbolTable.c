#include <stdlib.h>
#include <string.h>
#include "common.h"

#define SymbolTableAlloc 1024

SymbolTable* SymbolTableInit()
{
    SymbolTable* st = malloc(sizeof(SymbolTable));
    st->symbols = calloc(SymbolTableAlloc, sizeof(SilentId));
    st->count = 0;
    st->allocated = SymbolTableAlloc;
    return st;
}

SymbolTable* SymbolTableLoad(SymbolTableLink *tableLink){
    SymbolTable* st = malloc(sizeof(SymbolTable));
    st->symbols = calloc(tableLink->count, sizeof(SymbolTable));
    st->count = tableLink->count;
    st->allocated = tableLink->count;

    SilentIdLink* current = tableLink->root;
    for(uint64 i = 0; i < tableLink->count; i++)
    {
        st->symbols[i].str = current->id;
        current = current->next;
    }

    return st;
}

void SymbolTableFree(SymbolTable *table)
{
    free(table->symbols);
    free(table);
}

char SymbolTableAppend(SymbolTable *table, SilentId item)
{
    if(table->count == table->allocated)
    {
        SilentId* temp = realloc(table->symbols, 
            table->allocated + SymbolTableAlloc);

        if(temp == NULL) return 0;

        table->allocated += SymbolTableAlloc;
        table->symbols = temp;
    }

    table->symbols[++table->count] = item;
    return 1;
}

SymbolTableLink* SymbolTableLinkInit(){
    SymbolTableLink* stl = calloc(1, sizeof(SymbolTableLink));
    return stl;
}

void SymbolTableLinkFree(SymbolTableLink* table)
{
    if(table->root != NULL)
    {
        SilentIdLink* current = table->root;
        SilentIdLink* temp;
        do{
            temp = current->next;
            free(current);
            current = temp;
        }while(current != NULL);
    }
    free(table);
}

// Takes in a pointer to a string
// If the string already exists in the list it is freed and the value assigned
// to the list's existing char*
void SymbolTableLinkInsert(SymbolTableLink* table, char** str)
{
    // First Insert
    if(table->root == NULL)
    {
        table->root = calloc(1, sizeof(SilentIdLink));
        table->root->id = *str;
        table->count = 1;
    }
    // Inserting into a list
    else
    {
        SilentIdLink* current = table->root;
        SilentIdLink* temp;
        do{
            temp = current->next;
            int cmpres = strcmp(current->id, *str);
            if(cmpres < 0)
            {
                SilentIdLink* new = calloc(1, sizeof(SilentIdLink));
                new->id = *str;
                new->next = temp;
                current->next = new;
                table->count++;
                break;
            }
            else if(cmpres == 0) 
            {
                free(*str);
                *str = current->id;
                break;
            }
            current = temp;
        }while(current != NULL);
    }
}

