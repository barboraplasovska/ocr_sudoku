#include <stdio.h>
#include <string.h>

        

        int grid[9][9];

        ///<summary>
        ///Creates the sudoku grid from a string
        ///</summary>
        void sudokuGrid(char str[])
        {
            if (str.Length != 81)
                return;
            
            for (int y = 0; y < 9; y++)
            {
                for (int x = 0; x < 9; x++)
                {
                    char c = str[y * 9 + x];
                    if (c == '.')
                        c = '0';
                    else if (c < '1' || c > '9')
                        return;
                    grid[y][x] = c - '0';
                }
            }
        }

        /// <summary>
        /// Prints the grid on the console
        /// </summary>
        void finalProduct(int[] grid)
        {
            for (int y = 0; y < 9; y++)
            {
                if (y % 3 == 0)
                    printf("-------------------------------");

                for (int x = 0; x < 9; x++)
                {
                    if (x % 3 == 0)
                        printf('|');

                    int c = grid[y][x];
                    if (c == 0)
                        printf("   ");
                    else
                        printf(" %i ",c);
                }
                printf('|');
            }
            printf("-------------------------------");
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
                    if (grid[y][x] == i)
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
                    if (grid[y][x] == i)
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
                        if (grid[Y + y * 3][X + x * 3] == i)
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
                if (grid[y][x] == val)
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
                if (grid[y][x] == val)
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
                    if (grid[Y + y * 3][ X + x * 3] == val)
                        return 1;
                }
            }

            return 0;
        }

        int SetNextCoords(int *nextX,int *nextY)
        {
            if (*nextX >= 9)
            {
                *nextX = 0;
                *nextY++;
            }
        }

        int solve_rec(int x, int y)
        {
            if (y >= 9)
                return 1;

            int nextX = x + 1;
            int nextY = y;
            SetNextCoords(&nextX, &nextY);

            if (grid[y][x] != 0)
                return solve_rec(nextX, nextY);

            for (int i = 1; i <= 9; i++)
            {
                if (!already_in_column(x, i) && !already_in_line(y, i) && !already_in_square(x, y, i))
                {
                    grid[y][x] = i;
                    if (solve_rec(nextX, nextY))
                        return 1;
                    grid[y][x] = 0;
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
    }
