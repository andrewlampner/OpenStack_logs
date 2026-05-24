#pragma once
#include <string.h>

typedef struct {
    char* time;
    char* event;
} lifeCycleEvents;

typedef struct {
    char* instance;
    int eventCount;
    int eventCapacity;
    lifeCycleEvents* event;
} virtualMachine;

typedef struct {
    virtualMachine* VMList;
    int vmCount;
    int vmCapacity;
}VMmasterList;

void initEventArray (virtualMachine* virtualMachine);
void insertVMEvent (virtualMachine* virtualMachine, char* time, char* event);
void growEventArray(virtualMachine* virtualMachine);
void initVMList(VMmasterList* VMList);
void freeVMList(VMmasterList* VMList);
void insertVMList(VMmasterList* VMList, char* time, char* event, char* UID);
void growVMList(VMmasterList* VMList);
virtualMachine* searchVMList (VMmasterList* VMList, char* UID);
