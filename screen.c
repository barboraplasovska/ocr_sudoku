#include <stdio.h>
#include <string.h>
#include "screen.h"

void clearScreen()
{
    printf("\e[1;1H\e[2J");
}

void setColor(Color c)
{
    char esc[5];
    strcpy(esc, "0;00");
    switch (c) {
        case WHITE : strcpy(esc, "0;00");
            break;
        case RED   : strcpy(esc, "1;31");
            break;
        case GREEN : strcpy(esc, "1;32");
            break;
        case YELLOW: strcpy(esc, "1;33");
            break;
        case BLUE  : strcpy(esc, "1;34");
            break;
        case CYAN  : strcpy(esc, "1;36");
            break;
    }
    printf("%c[%sm",27,esc);
}

void locateCursor(const int row, const int col)
{
    printf("%c[%d;%dH",27,row,col);
}