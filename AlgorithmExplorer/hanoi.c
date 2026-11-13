#include "hanoi.h"

// Calculate the minimum number of moves required
// Time complexity: O(2^n)
int hanoiTotalMoves(int disks)
{
    if (disks == 0)
    {
        return 0;
    }

    return 2 * hanoiTotalMoves(disks - 1) + 1;
}

// Generate all Tower of Hanoi moves recursively
// Time complexity: O(2^n)
void generateHanoiMoves(
    int disks,
    int source,
    int auxiliary,
    int destination,
    HanoiMove moves[],
    int *count)
{
    if (disks == 0)
    {
        return;
    }

    generateHanoiMoves(
        disks - 1,
        source,
        destination,
        auxiliary,
        moves,
        count
    );

    moves[*count].disk = disks;
    moves[*count].from = source;
    moves[*count].to = destination;

    (*count)++;

    generateHanoiMoves(
        disks - 1,
        auxiliary,
        source,
        destination,
        moves,
        count
    );
}