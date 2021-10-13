#include <stdio.h>
#include <stdlib.h>
#include "functions.h"

int main()
{
    char fileName[] = "C:/Users/Elsa/Desktop/S3/OCR/ocr_sudoku/solver/grid_00";
    //scanf("%s", fileName);
    FILE *in_file  = fopen(fileName,"r+");
    char string[102];
    if (in_file != NULL)
    {
        fgets(string, 101, in_file);
    }
    fclose(in_file);

    sudokuGrid(string);
    solve();
    finalProduct();


    return 0;
}
