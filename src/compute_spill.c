#include <stdio.h>

#include "node.h"

int compute_spill(size_t nrows, size_t ncols, struct Node **grid)
{
    size_t row, col;

    for (row = 0; row < nrows; row++)
    {
        for (col = 0; col < ncols; col++)
            printf("%d", grid[row][col].type);
        printf("\n");
    }
    return 0;
}
