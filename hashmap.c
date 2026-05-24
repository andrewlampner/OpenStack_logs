#include "hashmap.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void initTable(Table* table){
    table->capacity = 0;
    table->count = 0;
    table->entries = NULL;
    return;
}
/*=================================================================================================*/
void freeTable(Table* table){
    if (table == NULL || table->entries == NULL) return;
    for (int i = 0; i < table->capacity; i++){
        APIRequest* node = table->entries[i].key;
        if(node != NULL){  
            free(node->request);
            free(node);
            table->entries[i].key = NULL;
        }
    }
    free(table->entries);
    table->entries = NULL;
    return;
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
int insertTable(Table* table, APIRequest* key){
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
            return 0;
        }
        else {
            index  = (index + 1) % table->capacity;
        }
    }
    table->entries[index].key = calloc(1, sizeof(APIRequest));
    table->entries[index].key->request = strdup(key->request);
    free(key->request);
    table->entries[index].key->hash = key->hash;
    table->entries[index].key->length = key->length;
    free(key);
    table->entries[index].count++;
    table->count++;
    return 1;
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
    free(oldEntry);
}