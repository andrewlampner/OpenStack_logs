#include "dyn_array.h"

void initEventArray (virtualMachine* virtualMachine){
    virtualMachine->event = NULL;
    virtualMachine->eventCount = 0;
    virtualMachine->eventCapacity = 0;
}
/*=================================================================================================*/
growEventArray(virtualMachine* virtualMachine){

}
/*=================================================================================================*/
void insertVMEvent (virtualMachine* virtualMachine, char* time, char* event){
    if (virtualMachine->eventCapacity == 0 || 
    virtualMachine->eventCount >= virtualMachine->eventCapacity){
        growEventArray(virtualMachine);
    }
//NOT FINISHED
}
/*=================================================================================================*/
void freeEventArray (virtualMachine* virtualMachine){
    free(virtualMachine->event);
    initArray(virtualMachine);
}
/*=================================================================================================*/
void initVMList(VMmasterList* VMList){

}
/*=================================================================================================*/
void growVMList(VMmasterList* VMList){

}
/*=================================================================================================*/
virtualMachine* searchVMList (VMmasterList* VMList, char* UID){

}
/*=================================================================================================*/
void insertVMList(VMmasterList* VMList, char* time, char* event, char* UID){

}
/*=================================================================================================*/
void freeVMList(VMmasterList* VMList){

}