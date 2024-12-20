/**
 *  Alex Carbajal
 *  CPT_S 411, Fall 2024
 *  Homework 2: Conway's Game of Life
 */

#include "board.h"

void init_board(int board[][N])
{
    for (int row = 0; row < M; row++)
    {
        for (int col = 0; col < N; col++)
        {
            // random value of 0 or 1
            board[row][col] = rand() % 2;
        }
    }
}

void print_board(int board[][N], int M_local)
{
    for (int row = 0; row < M_local; row++)
    {
        for (int col = 0; col < N; col++)
        {
            printf("%d ", board[row][col]);
        }
        printf("\n");
    }
}

int get_top_neighbor(int rank)
{
    int neighbor_rank = 0;

    // a top neighbor doesn't exist
    if (rank == 0)
    {
        neighbor_rank = MPI_PROC_NULL;
    }
    else
    {
        neighbor_rank = rank - 1;
    }

    return neighbor_rank;
}

int get_bottom_neighbor(int rank, int size)
{
    int neighbor_rank = 0;

    // a bottom neighbor doesn't exist
    if (rank == (size - 1))
    {
        neighbor_rank = MPI_PROC_NULL;
    }
    else
    {
        neighbor_rank = rank + 1;
    }

    return neighbor_rank;
}

int count_neighbors(int board[][N], int row, int col, int M_local, int rank, int size)
{
    int neighbors = 0;
    // edges of the board
    int top = 1;
    // very top row of the board DOESN'T have a top neighbor
    if (rank == 0 && row == 1)
    {
        top = 0;
    }
    int bottom = 1;
    // very bottom row of the board DOESN'T have a bottom neighbor
    if (rank == (size - 1) && row == (M_local))
    {
        bottom = 0;
    }
    int right = (col + 1) < N; // check if rightmost column
    int left = (col - 1) >= 0; // check if leftmost column

    // check that the cell position exists and then check if the cell is alive or dead
    if (top && left && board[row - 1][col - 1])
    {
        neighbors++;
    }
    if (top && board[row - 1][col])
    {
        neighbors++;
    }
    if (top && right && board[row - 1][col + 1])
    {
        neighbors++;
    }
    if (right && board[row][col + 1])
    {
        neighbors++;
    }
    if (bottom && right && board[row + 1][col + 1])
    {
        neighbors++;
    }
    if (bottom && board[row + 1][col])
    {
        neighbors++;
    }
    if (bottom && left && board[row + 1][col - 1])
    {
        neighbors++;
    }
    if (left && board[row][col - 1])
    {
        neighbors++;
    }

    return neighbors;
}

void update_board(int board[][N], int M_local, int rank, int size)
{
    int neighbors = 0; // # of neighbors a cell has
    int new_board[M_local][N];
    int final_row = M_local + 1;

    for (int row = 1; row < final_row; row++)
    {
        for (int col = 0; col < N; col++)
        {
            neighbors = count_neighbors(board, row, col, M_local, rank, size);

            // cell is alive (1)
            if (board[row][col] == 1)
            {
                // underpopulation: less than 2 neighbors -> cell dies
                // overpopulation: greater than 3 neighbors -> cell dies
                if (neighbors < 2 || neighbors > 3)
                {
                    new_board[row - 1][col] = 0;
                }
                // 2 or 3 neighbors -> cell lives
                else
                {
                    new_board[row - 1][col] = 1;
                }
            }
            // cell is not alive (0)
            else
            {
                // reproduction: dead cell with exactly 3 neighbors -> cell becomes alive
                if (neighbors == 3)
                {
                    new_board[row - 1][col] = 1;
                }
                // cell remains dead
                else
                {
                    new_board[row - 1][col] = 0;
                }
            }
        }
    }

    // update the board with the new values
    for (int row = 1; row < final_row; row++)
    {
        for (int col = 0; col < N; col++)
        {
            board[row][col] = new_board[row - 1][col];
        }
    }
}
