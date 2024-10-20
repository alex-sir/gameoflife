/**
 *  Alex Carbajal
 *  CPT_S 411, Fall 2024
 *  Homework 2: Conway's Game of Life
 */

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

    // track the time it takes for the program to fully run
    double startwtime, endwtime;
    startwtime = MPI_Wtime();

    int board[M][N];
    // setup the initial board
    if (rank == ROOT)
    {
        init_board(board);
        printf("*** INITIAL BOARD ***\n");
        print_board(board, M);
        printf("\n");
    }

    int rows_per_process = M / size;
    int total_rpp = rows_per_process + 2; // +2 for the top & bottom neighbor rows
    int local_board[total_rpp][N];
    int local_board_size = rows_per_process * N;
    int top_neighbor = get_top_neighbor(rank);
    int bottom_neighbor = get_bottom_neighbor(rank, size);

    // send each process their rows
    MPI_Scatter(board, local_board_size, MPI_INT, local_board[1], local_board_size, MPI_INT, ROOT, MPI_COMM_WORLD);

    // every process updates their rows T number of times
    for (int step = 0; step < T; step++)
    {
        // process sends its top row to the previous process & receives the bottom neighbor from the next process
        MPI_Sendrecv(local_board[1], N, MPI_INT, top_neighbor, 0, local_board[rows_per_process + 1], N, MPI_INT,
                     bottom_neighbor, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        // process sends its bottom row to the next process & receives the top neighbor from the previous process
        MPI_Sendrecv(local_board[rows_per_process], N, MPI_INT, bottom_neighbor, 1, local_board[0], N, MPI_INT,
                     top_neighbor, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        update_board(local_board, rows_per_process, rank, size);
    }

    // gather the final results of all the rows into root
    MPI_Gather(local_board[1], local_board_size, MPI_INT, board, local_board_size, MPI_INT, ROOT, MPI_COMM_WORLD);

    // print the final board
    if (rank == ROOT)
    {
        endwtime = MPI_Wtime();
        printf("*** FINAL BOARD ***\n");
        print_board(board, M);
        printf("Wall clock time: %fs\n", endwtime - startwtime);
    }

    MPI_Finalize();
    return 0;
}
