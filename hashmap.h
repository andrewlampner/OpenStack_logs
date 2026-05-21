#pragma once
#include <stdint.h>

typedef struct {
    char* request;
    uint32_t hash;
    int length;
} APIRequest;

typedef struct {
    APIRequest* key;
    int count;
} Entry;

typedef struct {
    int capacity;
    int count; 
    Entry* entries;
} Table;

void initTable (Table* table);
void freeTable (Table* table);
uint32_t FNV1aHash (const char* input);
void insertTable(Table* table, APIRequest* key);
void growTable(Table* table);
