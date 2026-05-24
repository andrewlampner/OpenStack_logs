#include "dyn_array.h"
#include "hashmap.h"
#include "log.h"
#include "print.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

int main()
{
    FILE *logFile = fopen("OpenStack_2k.log", "r");
    if (logFile == NULL){
        printf("Log file failed to load");
        return 1;
    }
    FILE *outputFile = fopen("outputFile.txt", "w");
    if (outputFile == NULL){
        printf("Output file failed to load.");
        return 2;
    }
    Table table;   
    initTable(&table);

    virtualMachine* VM;
    VM = (virtualMachine*)malloc(sizeof(virtualMachine));
    initEventArray(VM);

    VMmasterList* VMList;
    VMList = (VMmasterList*)malloc(sizeof(VMmasterList));
    initVMList(VMList);

    logStatistics logStats = {0};

    char *currentLine = NULL;
    size_t currentLength = 0;
    ssize_t currentRead;

    char* nextLine = NULL;
    size_t nextLength = 0;
    ssize_t nextRead;

    nextRead = getline(&nextLine, &nextLength, logFile);
    char* startTimeStamp = retrieveTimeStamp(nextLine);
    if (startTimeStamp == NULL) free(startTimeStamp);
    if (startTimeStamp != NULL){
        logStats.startTime = startTimeStamp;
    }

    while (nextRead != -1){
        char* temp = currentLine;
        currentLine = nextLine;
        nextLine = temp;

        size_t tempLength = currentLength;
        currentLength = nextLength;
        nextLength = tempLength;
        
        currentRead = nextRead;
        
        processLog(currentLine, currentRead, &logStats, &table, VMList);
        nextRead = getline(&nextLine, &nextLength, logFile);
        
        if (nextRead == -1) {
            char* endTimeStamp = retrieveTimeStamp(nextLine);
            if (endTimeStamp == NULL) free(endTimeStamp);
            if (endTimeStamp != NULL){
                logStats.endTime = endTimeStamp;
            }         
        }
        logStats.lineCount++;
    }


    masterPrintFunction(outputFile, &logStats, VMList, &table);

    freeVMList(VMList);
    free(VMList);
    free(VM);
    free(startTimeStamp);
    free(logStats.endTime);
    free(logStats.slowestAPI.request);
    free(currentLine);
    free(nextLine);
    freeTable(&table);

    fclose(logFile);
    fclose(outputFile);
    return 0;
}
