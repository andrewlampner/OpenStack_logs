#include "ComputeLogFunctions.h"
#include "dyn_array.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void processSeverity(char* line, logStatistics* stats){
    for (int i = 65; i < 72; i++){
        if (line[i] == 'T'){
            stats->traceCount++;
            return;
        }
        if (line[i] == 'D'){
            stats->debugCount++;
            return;
        }
        if (line[i] == 'I'){
            stats->infoCount++;
            return;
        }
        if (line[i] == 'A'){
            stats->auditCount++;
            return;
        }
        if (line[i] == 'W'){
            stats->warningCount++;
            return;
        }
        if (line[i] == 'E'){
            stats->errorCount++;
            return;
        }
        if (line[i] == 'C'){
            stats->criticalCount++;
            return;
        }
    }
    return;
}
/*=================================================================================================*/
//in hind sight it may have been easier to search for open and closing parenthesis.
//they are the only ones in this type of log. 
int checkIfLifeEvent(char* line, ssize_t length){
    if (line == NULL) return 1;
    char* lifeEvent = "(Lifecycle Event)";
    int targetLength = 17;
    if (length < targetLength) return 1;
    char* endOfLine = line + length - 1;
    if (*endOfLine == '\n') endOfLine--;
    if (*endOfLine == '\r') endOfLine--;
    if (strncmp(endOfLine - targetLength + 1, lifeEvent, targetLength) == 0){
        return 0;
    }
    return 1;
}
/*=================================================================================================*/
#define UUID_LENGTH 36
char* grabInstanceID(char* line){
    char* target = strstr(line, "[instance: ");

    char* UIDStart = target + 11;
    char* UID = malloc(UUID_LENGTH + 1);
    strncpy(UID, UIDStart, UUID_LENGTH);
    UID[UUID_LENGTH] = '\0';
    return UID;
}
/*=================================================================================================*/
char* grabEventName(char* line){
    char* start = strrchr(line, ']');
    start += 2;
    char* end =  strrchr(line, '(');
    end -= 1;

    int eventLength = end - start;
    char* eventName = malloc(eventLength + 1);
    strncpy(eventName, start, eventLength);
    eventName[eventLength] = '\0';
    return eventName;
}
/*=================================================================================================*/
void processLifeEvent(char* UID, char* eventName, char* time, VMmasterList* masterList){
    insertVMList(masterList, time, eventName, UID);
    return;
}