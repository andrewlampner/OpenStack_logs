#pragma once
#include "log.h"

void processSeverity(char* line, logStatistics* stats);
int checkIfLifeEvent(char* line, ssize_t length);
char* grabInstanceID(char* line);
char* grabEventName(char* line);
void processLifeEvent(char* UID, char* eventName, char* time, VMmasterList* masterList);