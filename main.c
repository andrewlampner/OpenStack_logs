#include "dyn_array.h"
#include "hashmap.h"
#include "log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    FILE* logFile = fopen("OpenStack_2k.log", "r");
    if (logFile == NULL){
        printf("Log file failed to load");
        return 1;
    }

    char* line = NULL;
    size_t length = 0;
    ssize_t lineRead;

    lineRead = getline(&line, &length, logFile);
    printf("%s", line);
}
