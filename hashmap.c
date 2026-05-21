#include "hashmap.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void initTable(Table* table){
    table->capacity = 0;
    table->count = 0;
    table->entries = NULL;
}
/*=================================================================================================*/
void freeTable(Table* table){
    free(table->entries);
    initTable(table);
}
/*=================================================================================================*/
#define FNV_PRIME 16777619;
#define FNV_OFFSET_BASIS 2166136261U;

uint32_t FNV1aHash (const char* input){
    uint32_t hash = FNV_OFFSET_BASIS;
    while (!input){
        hash ^= (uint8_t)*input;
        hash *= FNV_PRIME;
        input++;
    }
    return hash;
}
/*=================================================================================================*/
void insertTable(Table* table, APIRequest* key){
    uint32_t hash = FNV1aHash(key->request);
    key->hash = hash;
    key->length = strlen(key->request);

    if (table->capacity == 0 || (float)table->count / (float)table->capacity > 0.8){
        growTable(table);
    }
    
    int index  = hash % table->capacity;
    while (table->entries[index].key != NULL){
        if (strcmp(table->entries[index].key->request, key->request) == 0){
            table->entries[index].count++;
            return;
        }
        else {
            index  = (index + 1) % table->capacity;
        }
    }
    table->entries[index].key = key;
    table->entries[index].count++;
    table->count++;
    return;
}
/*=================================================================================================*/
void growTable(Table* table){
    int oldCapacity = table->capacity;
    Entry* oldEntry = table->entries;

    if (table->capacity == 0){
        table->capacity = 16;
    }
    else {
        table->capacity = table->capacity * 2;
    }

    table->entries = calloc(table->capacity, sizeof(Entry));
    table->count = 0;

    for (int i = 0; i < oldCapacity; i++){
        Entry* current = &oldEntry[i];

        if (current->key == NULL) continue;

        int newIndex = current->key->hash % table->capacity;
        while(table->entries[newIndex].key != NULL){
            newIndex = (newIndex + 1) % table->capacity;
        }
        table->entries[newIndex].key = current->key;
        table->entries[newIndex].count = current->count;
        table->count++;
    }
}