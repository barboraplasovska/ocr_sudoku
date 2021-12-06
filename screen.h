#ifndef SCREEN_H
#define SCREEN_H

typedef enum Color {WHITE, RED, GREEN, YELLOW, BLUE, CYAN} Color;
static const Color DEFAULT_TEXT_COLOR = WHITE;

void clearScreen(void);
void setColor(Color c);
void locateCursor(const int row, const int col);

#endif