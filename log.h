#pragma once
#include <stddef.h>
#include <sys/types.h>

typedef struct {
    int lineCount;
    char* startTime;
    char* endTime;
    int traceCount;
    int debugCount;
    int infoCount;
    int auditCount;
    int warningCount;
    int errorCount;
    int criticalCount;
    struct{
        double sum;
        int count;
    } averageAPI;
    struct{
        float time;
        char* request;
    }slowestAPI;
} logStatistics;

char* retrieveTimeStamp(char* line);
void processLog(char* line, ssize_t length, logStatistics* stats);
void processAPILog(char* line, ssize_t length, logStatistics* stats);
void checkSlowestRequestTime(char* time, logStatistics* stats);
void processComputeLog(char* line, ssize_t length, logStatistics* stats);
void processSchedulerLog(char* line, logStatistics* stats);
void printStartTime(char* startTime);
void printEndTime(char* endTime);
void printErrorCodes(int trace, int debug, int info, int audit, int warning, int error, int critical);
