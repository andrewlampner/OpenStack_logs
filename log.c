#include "APIMicroFunctions.h"
#include "ComputeLogFunctions.h"
#include "log.h"
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

char* retrieveTimeStamp(char* line){
    int timeStampLength = 24;
    char* output = malloc((timeStampLength + 1) * sizeof(char));

    if (line[5] == 'a'){
        strncpy(output, line + 34, timeStampLength);
        output[timeStampLength] = '\0';
        return output;
    }

    if (line[5] == 'c'){
        strncpy(output, line + 39, timeStampLength);
        output[timeStampLength] = '\0';
        return output;
    }

    if (line[5] == 's'){
        strncpy(output, line + 41, timeStampLength);
        output[timeStampLength] = '\0';
        return output;
    }

    free(output);
    return NULL;
}
/*=================================================================================================*/
void processLog(char* line, ssize_t length, logStatistics* stats,
Table* table, VMmasterList* VMList){
    if (line[5] == 'a') processAPILog(line, length, stats, table);
    if (line[5] == 'c') processComputeLog(line, length, stats, VMList);
    if (line[5] == 's') processSchedulerLog(line, stats);
    return;
}
/*=================================================================================================*/
//in this function I need to:
//(1) grab severity. event codes start at 60.
//(2) add to the average API struct.
//(3) determine if this is slowest API request.  last byte is length - 1.
//(4) request to hash map and increment counter, so we can grab the most common API requests later
void processAPILog(char* line, ssize_t length, logStatistics* stats, Table* table){
    
    processAPILogSeverity(line, stats);

    double requestTime = retrieveAPIRequestTime(line, length);

    averageAPITime(requestTime, stats);

    char* requestString = retrieveAPIRequestString(line, length);
    if (requestString != NULL){
        evaluateSlowestAPITime(requestString, requestTime, stats);
        insertIntoHashMap(requestString, table);
    }
    return;
}
/*=================================================================================================*/
void processComputeLog(char* line, ssize_t length, logStatistics* stats, VMmasterList* VMList){
    processSeverity(line, stats);
    if (checkIfLifeEvent(line, length) == 0){
        char* time = retrieveTimeStamp(line);
        char* UID = grabInstanceID(line);
        char* eventName = grabEventName(line);
        processLifeEvent(UID, eventName, time, VMList);
    }
    return;
}
/*=================================================================================================*/
//nothing interesting happens in these logs, just grab the severity level for the count.
void processSchedulerLog(char* line, logStatistics* stats){
    //the event codes for sched logs start at byte 65.  
    //I just set i to 65 instead of doing 65+i
    for (int i = 65; i < 72; i++){
        if (line[i] == 'T') {
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