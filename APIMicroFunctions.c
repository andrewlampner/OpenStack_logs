#include "APIMicroFunctions.h"
#include "log.h"
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

void processAPILogSeverity (char* line, logStatistics* stats){
    for (int i = 60; i < 68; i++){
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
double retrieveAPIRequestTime (char* line, ssize_t length){
    char* timeStr = malloc(16 * sizeof(char));
    if (timeStr == NULL){
        free(timeStr);
        return 0.0;
    }

    int timeLength = 0;
    for (int i = 1; i < length; i++){
        if (line[length - i] == ' '){
            break;
        } 
        timeLength++;
    }

    size_t copyLength = (timeLength > 15) ? 15 : timeLength;
    strncpy(timeStr, line + (length - timeLength), copyLength);
    timeStr[copyLength] = '\0';

    char* endptr;
    double timeDouble = strtod(timeStr, &endptr);

    //there are a couple of API logs that don't end in timestamps.  this filters those out.
    if (timeDouble > 10.0 || isinf(timeDouble)){
        free(timeStr);
        return 0.0;
    }
    free(timeStr);
    return timeDouble;
}
/*=================================================================================================*/
void averageAPITime (double requestTime, logStatistics* stats){
    if(requestTime == 0.0) return;
    stats->averageAPI.sum += requestTime;
    stats->averageAPI.count++;
    return;
}
/*=================================================================================================*/
char* retrieveAPIRequestString(char* line, ssize_t length){
    size_t offset = 100;
    if ((size_t)length <= offset) return NULL;
    
    char* firstQuote = strchr(line + offset, '"');
    if (firstQuote == NULL) return NULL;

    firstQuote++;
    char* endQuote = strchr(firstQuote, '"');
    if (endQuote == NULL) return NULL;
    
    ptrdiff_t APIStrLength = endQuote - firstQuote;
    char* requestStr = malloc(APIStrLength + 1);

    strncpy(requestStr, firstQuote, APIStrLength);
    requestStr[APIStrLength] = '\0';

    return requestStr;
}
/*=================================================================================================*/
void evaluateSlowestAPITime (char* request, double requestTime, logStatistics* stats){
    if (stats->slowestAPI.time < requestTime){
        stats->slowestAPI.time = requestTime;
        free(stats->slowestAPI.request);
        stats->slowestAPI.request = strdup(request);
    }
    return;
}
/*=================================================================================================*/
void insertIntoHashMap(char* request, Table* table){
    if (request == NULL) return;
    APIRequest* newAPIRequest = (APIRequest*)malloc(sizeof(APIRequest));
    newAPIRequest->request = request;
    if(insertTable(table, newAPIRequest) == 0){
        free(newAPIRequest->request);
        free(newAPIRequest);
        return;
    }
}