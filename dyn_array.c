#include "dyn_array.h"
#include <stdlib.h>

void initEventArray (virtualMachine* virtualMachine){
    virtualMachine->event = NULL;
    virtualMachine->eventCount = 0;
    virtualMachine->eventCapacity = 0;
}
/*=================================================================================================*/
void growEventArray(virtualMachine* virtualMachine){
    if (virtualMachine->eventCapacity == 0){
        virtualMachine->eventCapacity = 4;
        virtualMachine->event = malloc(virtualMachine->eventCapacity * sizeof(lifeCycleEvents));
        return;
    }
    virtualMachine->eventCapacity *= 2;
    virtualMachine->event = realloc(virtualMachine->event, 
                                    virtualMachine->eventCapacity * sizeof(lifeCycleEvents));
    return;
}
/*=================================================================================================*/
void insertVMEvent (virtualMachine* virtualMachine, char* time, char* event){
    if (virtualMachine->eventCapacity == 0 || 
    virtualMachine->eventCount >= virtualMachine->eventCapacity){
        growEventArray(virtualMachine);
    }

    virtualMachine->event[virtualMachine->eventCount].event = event;
    virtualMachine->event[virtualMachine->eventCount].time  = time;
    virtualMachine->eventCount++;
}
/*=================================================================================================*/
void initVMList(VMmasterList* VMList){
    VMList->VMList = NULL;
    VMList->vmCount = 0;
    VMList->vmCapacity = 0;
}
/*=================================================================================================*/
void growVMList(VMmasterList* VMList){
    if (VMList->vmCapacity == 0){
        VMList->vmCapacity = 4;
        VMList->VMList = malloc(VMList->vmCapacity * sizeof(virtualMachine));
        return;
    }

    VMList->vmCapacity *= 2;
    VMList->VMList = realloc(VMList->VMList, VMList->vmCapacity * sizeof(virtualMachine));
}
/*=================================================================================================*/
virtualMachine* searchVMList (VMmasterList* VMList, char* UID){
    for (int i = 0; i < VMList->vmCount; i++){
        if (strcmp(VMList->VMList[i].instance, UID) == 0){
            return &VMList->VMList[i];
        }
    }
    return NULL;
}
/*=================================================================================================*/
void insertVMList(VMmasterList* VMList, char* time, char* event, char* UID){
    virtualMachine *targetVM = searchVMList(VMList, UID);

    if (targetVM == NULL){
        if (VMList->vmCount >= VMList->vmCapacity){
            growVMList(VMList);
        }
        targetVM = &VMList->VMList[VMList->vmCount];
        targetVM->instance = UID;
        targetVM->eventCount = 0;
        targetVM->eventCapacity = 0;
        targetVM->event = NULL;
        VMList->vmCount++;
    }
    else{
        free(UID);
    }
    insertVMEvent(targetVM, time, event);
    return;
}
/*=================================================================================================*/
void freeVMList(VMmasterList* VMList){
    for (int i = 0; i < VMList->vmCount; i++){
        free(VMList->VMList[i].instance);
        for (int j = 0; j < VMList->VMList[i].eventCount; j++){
            free(VMList->VMList[i].event[j].time);
            free(VMList->VMList[i].event[j].event);
        }
        free(VMList->VMList[i].event);
    }
    free(VMList->VMList);
}