#ifndef FUNCTIONS_H
#define FUNCTIONS_H



void sudokuGrid(char str[]);
void finalProduct();
int is_column_solved(int x);
int is_line_solved(int y);
int is_square_solved(int x, int y);
int is_solved();
int already_in_column(int x, int val);
int already_in_line(int y, int val);
int already_in_square(int x, int y, int val);
int SetNextCoords(int *nextX,int *nextY);
int solve_rec(int x, int y);
void solve();




#endif // FUNCTIONS_H
