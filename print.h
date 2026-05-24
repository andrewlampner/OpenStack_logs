#pragma once
#include "dyn_array.h"
#include "hashmap.h"
#include "log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void printLineCount(FILE* outFile, int count);
void printStartEndTimes(FILE* outFile, char* startTime, char* endTime);
void printErrorCodes(FILE* outFile, int trace, int debug, int info, int audit, int warning, int error, int critical);
void printAPIPerformance(FILE* outFile, logStatistics* logStats);
void TopFiveAPIRequests(FILE* outfile, Table* table);
Entry** extractFromHashTable(Table* table, int* count);
int howToSortEntries(const void* a, const void* b);
void sortExtractedEntriesFromHashTable(Entry** entries, int count);
void printTopFiveAPIRequests(FILE* outfile, Entry** entries, int count);
void printVMLifeEvents(FILE* outFile, VMmasterList* masterList);
void masterPrintFunction(FILE* outFile, logStatistics* stats,  VMmasterList* masterList, Table* table);