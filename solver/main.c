#include <stdio.h>
#include "functions.h"

    char fileName[];
    scanf("%s", fileName);
    FILE *in_file  = fopen(fileName,"r+");
    char string[102];
    if (in_file != NULL)
    {
        fgets(string, 101, in_file);
    }

    finalProduct(sudokuGrid(string));