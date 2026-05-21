#include "APIMicroFunctions.h"
#include "log.h"
#include <stddef.h>
#include <string.h>
#include <sys/types.h>

void processAPILogSeverity (char* line, logStatistics* stats){
    for (int i = 60; i < 68; i++){
        if (line[i] == 'T') stats->traceCount++; break;
        if (line[i] == 'D') stats->debugCount++; break;
        if (line[i] == 'I') stats->infoCount++; break;
        if (line[i] == 'A') stats->auditCount++; break;
        if (line[i] == 'W') stats->warningCount++; break;
        if (line[i] == 'E') stats->errorCount++; break;
        if (line[i] == 'C') stats->criticalCount++; break;
    }
}
/*=================================================================================================*/
double retrieveAPIRequestTime (char* line, ssize_t length, logStatistics* stats){
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

    char* endptr;
    double timeDouble = strtod(timeStr, &endptr);
    return timeDouble;
}
/*=================================================================================================*/
void averageAPITime (double requestTime, logStatistics* stats){
    stats->averageAPI.sum += requestTime;
    stats->averageAPI.count++;
}
/*=================================================================================================*/
char* retrieveAPIRequestString(char* line, ssize_t length){
    size_t offset = 100;
    if (length <= offset) return NULL;
    
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
        stats->slowestAPI.request = request;
    }
    return;
}
/*=================================================================================================*/
void insertIntoHashMap(char* request, Table* table){
    insertTable(&table, request);
}