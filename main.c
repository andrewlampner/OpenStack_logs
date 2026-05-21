#include "dyn_array.h"
#include "hashmap.h"
#include "log.h"
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

    logStatistics logStats = {0};

    char *currentLine = NULL;
    size_t currentLength = 0;
    ssize_t currentRead;

    char* nextLine = NULL;
    size_t nextLength = 0;
    ssize_t nextRead;

    nextRead = getline(&nextLine, &nextLength, logFile);
    logStats.startTime = retrieveTimeStamp(nextLine);

    while (nextRead != -1){
        char* temp = currentLine;
        currentLine = nextLine;
        nextLine = temp;

        size_t tempLength = currentLength;
        currentLength = nextLength;
        nextLength = tempLength;
        
        currentRead = nextRead;
        
        processLog(currentLine, currentRead, &logStats, &table);

        nextRead = getline(&nextLine, &nextLength, logFile);
        
        if (nextRead == -1) {
            logStats.endTime = retrieveTimeStamp(nextLine);
        }
    }
    free(currentLine);
    free(nextLine);
    fclose(logFile);
    fclose(outputFile);
    return 0;
}
