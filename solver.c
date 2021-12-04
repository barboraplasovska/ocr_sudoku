#include <stdio.h>

//int grid[9][9];
//int* p = &grid[0][0];

/// Returns true if the given column is solved
/// <param name="x"> index of the column</param>
int is_column_solved(int x, int *p)
{
    for (int i = 1; i <= 9; i++)
    {
        int found = 0;
        for (int y = 0; !found && y < 9; y++)
        {
            if (*(p + y*9 + x) == i)
                found = 1;
        }

        if (!found)
            return 0;
    }

    return 1;
}

/// Returns true if the given line is solved
/// <param name="y"> index of the line</param>
int is_line_solved(int y, int *p)
{
    for (int i = 1; i <= 9; i++)
    {
        int found = 0;
        for (int x = 0; !found && x < 9; x++)
        {
            if (*(p + y*9 + x) == i)
                found = 1;
        }

        if (!found)
            return 0;
    }

    return 1;
}

/// Returns true if the 3x3 square containing the given coords is solved
/// <param name="x"> index of the column</param>
/// <param name="y"> index of the line</param>
int is_square_solved(int x, int y, int* p)
{
    x /= 3;
    y /= 3;

    for (int i = 1; i <= 9; i++)
    {
        int found = 0;
        for (int Y = 0; !found && Y < 3; Y++)
        {
            for (int X = 0; !found && X < 3; X++)
            {
                int v1 = Y + y * 3;
                int v2 = X + x * 3;
                if (*(p + v1*9 + v2) == i)
                    found = 1;
            }
        }

        if (!found)
            return 0;
    }

    return 1;
}

/*
/// Returns true the grid is solved
int is_solved(int *p)
{
    for (int i = 0; i < 9; i++)
    {
        if (!is_column_solved(i, p) || !is_line_solved(i, p) ||
        !is_square_solved(i / 3, i % 3, p))
            return 0;
    }
    return 1;
}*/

/// <summary>
/// Returns true if the given column already contains the given value
/// </summary>
/// <param name="x"> index of the column</param>
/// <param name="val"> value that must be checked</param>
int already_in_column(int x, int val, int *p)
{
    for (int y = 0; y < 9; y++)
    {
        if (*(p + y*9 + x) == val)
            return 1;
    }

    return 0;
}

/// Returns true if the given line already contains the given value
/// <param name="y"> index of the line</param>
/// <param name="val"> value that must be checked</param>
int already_in_line(int y, int val, int* p)
{
    for (int x = 0; x < 9; x++)
    {
        if (*(p + y*9 + x) == val)
            return 1;
    }

    return 0;
}

/// Returns true if the 3x3 square containing the given 
/// already contains the given value
/// <param name="x"> index of the column</param>
/// <param name="y"> index of the line</param>
/// <param name="val"> value that must be checked</param>
int already_in_square(int x, int y, int val, int *p)
{
    x /= 3;
    y /= 3;

    for (int Y = 0; Y < 3; Y++)
    {
        for (int X = 0; X < 3; X++)
        {
            int v1 = Y + y * 3;
            int v2 = X + x * 3;
            if (*(p + v1*9 + v2) == val)
                return 1;
        }
    }

    return 0;
}

void SetNextCoords(int *nextX,int *nextY)
{
    if (*nextX >= 9)
    {
        *nextX = 0;
        *nextY += 1;
    }
}

int _solve(int x, int y, int* p)
{
    if (y >= 9)
        return 1;

    int nextX = x + 1;
    int nextY = y;
    SetNextCoords(&nextX, &nextY);

    if (*(p + y*9 + x) != 0)
        return _solve(nextX,nextY, p);

    for (int i = 1; i <= 9; i++)
    {
        if (!already_in_column(x, i, p) &&
         !already_in_line(y, i, p) && 
         !already_in_square(x, y, i, p))
        {
            *(p + y*9 + x) = i;
            if (_solve(nextX, nextY, p))
                return 1;
            *(p + y*9 + x) = 0;
        }
    }

    return 0;
}

/// Solves the grid
void solve(int* p)
{
    //*p = &g;
    _solve(0, 0, p);
}
