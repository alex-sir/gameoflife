/**
 *  Alex Carbajal
 *  CPT_S 411, Fall 2024
 *  Homework 2: Conway's Game of Life
 */

#ifndef BOARD
#define BOARD

// matrix grid (MxN)
#define M 8  // # of rows
#define N 8  // # of columns
#define T 10 // # of time steps in simulation

// 2 possible states for a cell
#define ALIVE 1
#define DEAD 0

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/**
 * @brief initialize a matrix board with random values of 0 or 1
 *
 * @param board matrix board to initialize with random values of 0 or 1
 */
extern void init_board(int board[][N]);

/**
 * @brief print a matrix board
 *
 * @param board matrix board to print out
 * @param M_local number of rows of the matrix board to print out
 */
extern void print_board(int board[][N], int M_local);

/**
 * @brief return the top (or previous) neighbor block of rows/process
 *
 * @param rank rank of the process to find the top neighbor of
 * @return int rank of the top neighbor
 */
extern int get_top_neighbor(int rank);

/**
 * @brief return the bottom (or next) neighbor block of rows/process
 *
 * @param rank rank of the process to find the bottom neighbor of
 * @param size total number of processes executing the program
 * @return int rank of the bottom neighbor
 */
extern int get_bottom_neighbor(int rank, int size);

/**
 * @brief count the number of live neighbor cells that a cell in a matrix board has
 *
 * @param board matrix board
 * @param row row of the cell to check neighbors of
 * @param col column of the cell to check neighbors of
 * @param M_local number of rows of the matrix board
 * @param rank rank of the process to have the number of live neighbors of one of its cells counted
 * @param size total number of processes executing the program
 * @return int number of live neighbors the cell has
 */
extern int count_neighbors(int board[][N], int row, int col, int M_local, int rank, int size);

/**
 * @brief fully update a matrix board by checking all cells
 *
 * @param board matrix board to check and update
 * @param M_local number of rows of the matrix board
 * @param rank rank of the process to update its cells
 * @param size total number of processes executing the program
 */
extern void update_board(int board[][N], int M_local, int rank, int size);

#endif
