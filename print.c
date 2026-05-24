#include "print.h"
#include <stdio.h>

void printLineCount(FILE* outFile, int count){
    fprintf(outFile, "Lines parsed: %d\n", count);
    return;
}
/*=================================================================================================*/
void printStartEndTimes(FILE* outFile, char* startTime, char* endTime){
    fprintf(outFile, "Start time: %s\n", startTime);
    fprintf(outFile, "End time: %s\n", endTime);
    return;
}
/*=================================================================================================*/
void printErrorCodes(FILE* outFile, int trace, int debug, int info, int audit, int warning, int error, int critical){
    fprintf(outFile, "Trace:         %d\n", trace);
    fprintf(outFile, "Debug:         %d\n", debug);
    fprintf(outFile, "Info:          %d\n", info);
    fprintf(outFile, "Audit:         %d\n", audit);
    fprintf(outFile, "Warning:       %d\n", warning);
    fprintf(outFile, "Error:         %d\n", error);
    fprintf(outFile, "Crititcal:     %d\n", critical);
    return;
}
/*=================================================================================================*/
void printAPIPerformance(FILE* outFile, logStatistics* logStats){
    double average = logStats->averageAPI.sum / (double)logStats->averageAPI.count;
    fprintf(outFile, "Average API response time: %f\n\n", average);
    fprintf(outFile, "Slowest performing HTTP request: \n");
    fprintf(outFile, "Time: %f\n", logStats->slowestAPI.time);
    fprintf(outFile, "Request: %s\n\n", logStats->slowestAPI.request);
    return;
}
/*=================================================================================================*/
//double pointers are weird.
//we want to reorganize the pointers of the entries, not the entries themselves
//so we point to the pointers rather than point to the values.
Entry** extractFromHashTable(Table* table, int* count){
    if (table->count == 0){
        return NULL;
    }

    Entry** extractedList = malloc(table->count * sizeof(Entry));

    int index = 0;

    for(int i = 0; i < table->capacity; i++){
        if (table->entries[i].key != NULL){
            extractedList[index] = &table->entries[i];
            index++;
        }
    }
    
    *count = index;
    return extractedList;
}
/*=================================================================================================*/
//void* means a memory address to an unknowntype.  the function qsort requires this (called in next function)
//first thing you have to is tell compiler to treat the inputs as an address to a specific data type
int howToSortEntries(const void* a, const void* b){
    //a is a void pointer
    //We cast 'a' to be a double pointer
    //then dereference that pointer to be a single pointer to an Entry.
    //Whoever came up with this system must have been out of their minds.
    Entry* entryA = *(Entry**)a;
    Entry* entryB = *(Entry**)b;
    //if B is bigger than A, it returns a positive and the order swaps (descending order).
    return (entryB->count - entryA->count);
}
/*=================================================================================================*/
void sortExtractedEntriesFromHashTable(Entry** entries, int count){
    if (entries == NULL || count <= 1){
        return;
    }
    qsort(entries, count, sizeof(Entry*), howToSortEntries);
}
/*=================================================================================================*/
void TopFiveAPIRequests(FILE* outFile, Table* table){
    int count = 0;

    Entry** entries = extractFromHashTable(table, &count);

    if (entries == NULL || count == 0) return;

    sortExtractedEntriesFromHashTable(entries, count);

    printTopFiveAPIRequests(outFile, entries, count);
    free(entries);
}
/*=================================================================================================*/
void printTopFiveAPIRequests(FILE* outFile, Entry** entries, int count){
    if (outFile == NULL || entries == NULL || count == 0) return;

    for (int i = 0; i < 5 && i < count; i++){
        fprintf(outFile, "%s\ncount: %d\n\n", entries[i]->key->request, entries[i]->count);
    }
    return;
}
/*=================================================================================================*/
void printVMLifeEvents(FILE* outFile, VMmasterList* masterList){
    if (masterList == NULL || masterList->VMList == NULL){
        fprintf(outFile, "No VM lifecycle events detected");
        return;
    }
    for (int i = 0; i < masterList->vmCount; i++){
        virtualMachine* currentVM = &masterList->VMList[i];
        fprintf(outFile, "UID: %s\n", currentVM->instance);
        for(int j = 0; j < currentVM->eventCount; j++){
            lifeCycleEvents* currentEvent = &currentVM->event[j];
            fprintf(outFile, "%s: %s\n", currentEvent->time, currentEvent->event);
        }
        fprintf(outFile, "\n");
    }
    return;
}
/*=================================================================================================*/
void masterPrintFunction(FILE* outFile, logStatistics* logStats, VMmasterList* masterList, Table* table){
        fprintf(outFile, "**===========================================================================================**\n");
        fprintf(outFile, "OpenStack Log Parser\n");
        printLineCount(outFile, logStats->lineCount);
        printStartEndTimes(outFile, logStats->startTime, logStats->endTime);
        fprintf(outFile, "**===========================================================================================**\n");
        fprintf(outFile, "Log Severity Count\n\n");
        printErrorCodes(outFile, logStats->traceCount, logStats->debugCount, logStats->infoCount,
             logStats->auditCount, logStats->warningCount, logStats->errorCount, logStats->criticalCount);
        fprintf(outFile, "**===========================================================================================**\n");
        fprintf(outFile, "API Performance \n\n");
        printAPIPerformance(outFile, logStats);
        fprintf(outFile, "**===========================================================================================**\n");
        fprintf(outFile, "Most Common HTTP Requests\n\n");
        TopFiveAPIRequests(outFile, table);
        fprintf(outFile, "**===========================================================================================**\n");
        fprintf(outFile, "Virtual Machine Lifecycle Events\n\n");
        printVMLifeEvents(outFile, masterList);
        fprintf(outFile, "\n\n\nEnd of summary.");
    }