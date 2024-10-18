/**
 *  Alex Carbajal
 *  CPT_S 411, Fall 2024
 *  Homework 2: Conway's Game of Life
 */

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "board.h"

// root process
#define ROOT 0

int main(int argc, char **argv)
{
    srand(time(NULL));
    MPI_Init(&argc, &argv);

    int rank = 0, size = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // exit the program if the number of rows is not divisble by the number of processes
    if ((M % size) != 0)
    {
        if (rank == ROOT)
        {
            printf("Error: %d rows not divisble by %d processes\n", M, size);
        }
        MPI_Finalize();
        return 0;
    }

    int **board = NULL;
    // setup the initial board
    if (rank == ROOT)
    {
        alloc_board(&board, M);
        init_board(board);
        print_board(board, M, ROOT);
    }

    int rows_per_process = M / size;
    int total_rpp = rows_per_process + 2; // +2 for the top & bottom neighbor rows
    int **local_board = NULL;
    alloc_board(&local_board, total_rpp);
    int local_board_size = rows_per_process * N;
    int top_neighbor = get_top_neighbor(rank);
    int bottom_neighbor = get_bottom_neighbor(rank, size);

    // send each process their rows
    MPI_Scatter(&(board[0][0]), local_board_size, MPI_INT, &(local_board[1][0]), local_board_size, MPI_INT, ROOT,
                MPI_COMM_WORLD);

    // MPI_Barrier(MPI_COMM_WORLD);
    // print_board(local_board, total_rpp, rank);
    // MPI_Barrier(MPI_COMM_WORLD);

    // every process updates their rows T number of times
    for (int step = 0; step < T; step++)
    {
        // process sends its top row to the previous process & receives the bottom neighbor from the next process
        // MPI_Sendrecv(&(local_board[1][0]), N, MPI_INT, top_neighbor, 0, &(local_board[rows_per_process + 1][0]), N,
        //              MPI_INT, bottom_neighbor, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        // // process sends its bottom row to the next process & receives the top neighbor from the previous process
        // MPI_Sendrecv(&(local_board[rows_per_process][0]), N, MPI_INT, bottom_neighbor, 1, &(local_board[0][0]), N,
        //              MPI_INT, top_neighbor, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        // update_board(local_board, rows_per_process, rank, size);
    }

    // MPI_Barrier(MPI_COMM_WORLD);
    // print_board(local_board, total_rpp, rank);
    // MPI_Barrier(MPI_COMM_WORLD);

    // gather the final results of all the rows into root
    // MPI_Gather(&(local_board[1][0]), local_board_size, MPI_INT, &(board[0][0]), local_board_size, MPI_INT, ROOT,
    //            MPI_COMM_WORLD);

    free_board(local_board, total_rpp);
    // print the final board
    if (rank == ROOT)
    {
        print_board(board, M, ROOT);
        free_board(board, M);
    }

    MPI_Finalize();
    return 0;
}
