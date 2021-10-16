#include <stdio.h>



int grid[9][9];
int* p = &grid[0][0];


///<summary>
///Creates the sudoku grid from a string
///</summary>
void load(char path[])
{   
    char c;
    int x = 0;
    int y = 0;
    FILE* f = fopen(path, "r");
    while ((c = fgetc(f))!= EOF)
    {
        if ((c >= '1' && c <= '9' )|| c == '.')
        {
            if (c == '.')
                c = '0';
            *(p + y*9 + x) = c - '0';
            x+=1;
        }   
        if (x == 9)
        {
            y += 1;
            x = 0;
        }
    }    

    
    fclose(f);
}

/// <summary>
/// Prints the grid on the console
/// </summary>
void finalProduct(char path[])
{
    char newpath[8] = ".result";
    char str3[100];
  
    int i = 0, j = 0;
  
    // Insert the first string in the new string
    while (path[i] != '\0') {
        str3[j] = path[i];
        i++;
        j++;
    }
  
    // Insert the second string in the new string
    i = 0;
    while (newpath[i] != '\0') {
        str3[j] = newpath[i];
        i++;
        j++;
    }

    str3[j] = '\0';
    FILE* f = fopen(str3, "w");
    for (int y = 0; y < 9; y++)
    {
        if ((y == 3) || (y==6))
        {
           fprintf(f,"\n");
        }
        
        for (int x = 0; x < 9 ; x++)
        {   
            int val = *(p + y*9 + x);
            if (x == 8)
                fprintf(f,"%i\n",val);
            else if ((x == 2)||(x==5))
                fprintf(f,"%i ",val);
            else
                fprintf(f,"%i",val);
        }
        
    }
    fclose(f);
    
} 

/// <summary>
/// Returns true if the given column is solved
/// </summary>
/// <param name="x"> index of the column</param>
int is_column_solved(int x)
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

/// <summary>
/// Returns true if the given line is solved
/// </summary>
/// <param name="y"> index of the line</param>
int is_line_solved(int y)
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

/// <summary>
/// Returns true if the 3x3 square containing the given coords is solved
/// </summary>
/// <param name="x"> index of the column</param>
/// <param name="y"> index of the line</param>
int is_square_solved(int x, int y)
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

/// <summary>
/// Returns true the grid is solved
/// Here is a exemple of a solved grid : 435269781682571493197834562826195347374682915951743628519326874248957136763418259
/// </summary>
int is_solved()
{
    for (int i = 0; i < 9; i++)
    {
        if (!is_column_solved(i) || !is_line_solved(i) || !is_square_solved(i / 3, i % 3))
            return 0;
    }
    return 1;
}

/// <summary>
/// Returns true if the given column already contains the given value
/// </summary>
/// <param name="x"> index of the column</param>
/// <param name="val"> value that must be checked</param>
int already_in_column(int x, int val)
{
    for (int y = 0; y < 9; y++)
    {
        if (*(p + y*9 + x) == val)
            return 1;
    }

    return 0;
}

/// <summary>
/// Returns true if the given line already contains the given value
/// </summary>
/// <param name="y"> index of the line</param>
/// <param name="val"> value that must be checked</param>
int already_in_line(int y, int val)
{
    for (int x = 0; x < 9; x++)
    {
        if (*(p + y*9 + x) == val)
            return 1;
    }

    return 0;
}

/// <summary>
/// Returns true if the 3x3 square containing the given already contains the given value
/// </summary>
/// <param name="x"> index of the column</param>
/// <param name="y"> index of the line</param>
/// <param name="val"> value that must be checked</param>
int already_in_square(int x, int y, int val)
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

int solve_rec(int x, int y)
{
    if (y >= 9)
        return 1;

    int nextX = x + 1;
    int nextY = y;
    SetNextCoords(&nextX, &nextY);

    if (*(p + y*9 + x) != 0)
        return solve_rec(nextX,nextY);

    for (int i = 1; i <= 9; i++)
    {
        if (!already_in_column(x, i) &&
         !already_in_line(y, i) && 
         !already_in_square(x, y, i))
        {
            *(p + y*9 + x) = i;
            if (solve_rec(nextX, nextY))
                return 1;
            *(p + y*9 + x) = 0;
        }
    }

    return 0;
}

/// <summary>
/// Solves the grid
/// </summary>
void solve()
{
    solve_rec(0, 0);
}