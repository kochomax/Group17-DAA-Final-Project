#ifndef HANOI_H
#define HANOI_H

#define MAX_HANOI_MOVES 255

// Stores one movement of a disk
typedef struct
{
    int disk;
    int from;
    int to;
} HanoiMove;

// Calculate the total number of moves required
int hanoiTotalMoves(int disks);

// Generate and store all moves recursively
void generateHanoiMoves(
    int disks,
    int source,
    int auxiliary,
    int destination,
    HanoiMove moves[],
    int *count
);

#endif