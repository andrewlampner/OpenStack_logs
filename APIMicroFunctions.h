#pragma once
#include "log.h"
#include <sys/types.h>

void processAPILogSeverity (char* line, logStatistics* stats);
double retrieveAPIRequestTime (char* line, ssize_t length, logStatistics* stats);
char* retrieveAPIRequestString(char* line, ssize_t length);
void evaluateSlowestAPITime (char* request, double requestTime, logStatistics* stats);
void averageAPITime (double requestTime, logStatistics* stats);
void insertIntoHashMap(char* request, Table* table);
