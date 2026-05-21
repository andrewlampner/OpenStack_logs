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

    return NULL;
}
/*=================================================================================================*/
void processLog(char* line, ssize_t length, logStatistics* stats){
    if (line[5] == 'a') processAPILog(line, length, &stats);
    if (line[5] == 'c') processComputeLog(line, length, &stats);
    if (line[5] == 's') processSchedulerLog(line, &stats);
    return;
}
/*=================================================================================================*/
//in this function I need to:
//grab severity. event codes start at 60.
//determine if this is the slowest time stamp so far.  last byte is length - 1.
//request to hash map and increment counter, so we can grab the most common API requests later
void processAPILog(char* line, ssize_t length, logStatistics* stats){
    for (int i = 60; i < 68; i++){
        if (line[i] == 'T') stats->traceCount++; break;
        if (line[i] == 'D') stats->debugCount++; break;
        if (line[i] == 'I') stats->infoCount++; break;
        if (line[i] == 'A') stats->auditCount++; break;
        if (line[i] == 'W') stats->warningCount++; break;
        if (line[i] == 'E') stats->errorCount++; break;
        if (line[i] == 'C') stats->criticalCount++; break;
    }

    char* timeStr = malloc(13 * sizeof(char));
    int timeLength = 0;
    for (int i = 1; i < length; i++){
        if (line[length - i] == ' '){
            break;
        } 
        timeLength++;
    }

    strncpy(timeStr, line + (length - timeLength), timeLength);
    timeStr[timeLength] = '\0';
}
/*=================================================================================================*/
void processComputeLog(char* line, ssize_t length, logStatistics* stats){

}
/*=================================================================================================*/
//nothing interesting happens in these logs, just grab the severity level for the count.
void processSchedulerLog(char* line, logStatistics* stats){
    //the event codes for sched logs start at byte 65.  
    //I just set i to 65 instead of doing 65+i
    for (int i = 65; i < 72; i++){
        if (line[i] == 'T') stats->traceCount++; return;
        if (line[i] == 'D') stats->debugCount++; return;
        if (line[i] == 'I') stats->infoCount++; return;
        if (line[i] == 'A') stats->auditCount++; return;
        if (line[i] == 'W') stats->warningCount++; return;
        if (line[i] == 'E') stats->errorCount++; return;
        if (line[i] == 'C') stats->criticalCount++; return;
    }
}