#pragma once
#include <stddef.h>

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
} logStatistics;

char* retrieveStartTime(char* line);
char* retrieveEndTime(char* line);
void processLog(char* line);
void processAPILog(char* line);
void processComputeLog(char* line);
void processSchedulerLog(char* line);