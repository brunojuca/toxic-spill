#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "node.h"
#include "utils.h"

#define MAX_IT 10000
#define ACCURACY 1E-4

struct Node **compute_spill(size_t nrows, size_t ncols, struct Node **grid)
{
    int i, row, col;
    struct Node *newdata = (struct Node *)malloc(nrows * ncols * sizeof(struct Node));
    struct Node **auxgrid = (struct Node **)malloc(nrows * sizeof(struct Node *)), **tmp;

    if (newdata == NULL)
        throwMallocError();

    if (auxgrid == NULL)
    {
        free(newdata);
        throwMallocError();
    }

    for (i = 0; i < nrows; i++)
        auxgrid[i] = &newdata[i * ncols];

    for (row = 0; row < nrows; row++)
    {
        for (col = 0; col < ncols; col++)
        {
            auxgrid[row][col].type = grid[row][col].type;
            auxgrid[row][col].concentration = grid[row][col].concentration;
        }
    }

    printf("\nInitial island concentration: \n\n");
    print_grid(nrows, ncols, auxgrid);
    printf("\n---------------------\n\n");

    double c_sum, dc, max_dc;
    for (i = 0; i < MAX_IT; i++)
    {
        max_dc = 0;
        for (row = 0; row < nrows; row++)
        {
            for (col = 0; col < ncols; col++)
            {
                if (grid[row][col].type == GROUND)
                {
                    c_sum = 0;

                    if ((row - 1) >= 0)
                        c_sum += grid[row - 1][col].concentration;
                    if ((row + 1) < nrows)
                        c_sum += grid[row + 1][col].concentration;
                    if ((col - 1) >= 0)
                        c_sum += grid[row][col - 1].concentration;
                    if ((col + 1) < ncols)
                        c_sum += grid[row][col + 1].concentration;

                    auxgrid[row][col].concentration = c_sum / 4;

                    dc = fabs(auxgrid[row][col].concentration - grid[row][col].concentration) / auxgrid[row][col].concentration;
                    if (dc > max_dc)
                        max_dc = dc;
                }
            }
        }
        tmp = grid;
        grid = auxgrid;
        auxgrid = tmp;

        printf("Iteration %d: \n\n", i + 1);
        print_grid(nrows, ncols, grid);
        generate_image(nrows, ncols, grid, i + 1);
        printf("\nmax_dc: %lf\n", max_dc);
        printf("\n---------------------\n\n");

        if (max_dc < ACCURACY)
        {
            printf("Simulation stopped by convergence\n");
            printf("Iterations: %d\n", i + 1);
            printf("max_dc: %lf\n", max_dc);
            break;
        }
    }

    free(newdata);
    free(auxgrid);
    return grid;
}

int print_grid(size_t nrows, size_t ncols, struct Node **grid)
{
    int row, col;

    for (row = 0; row < nrows; row++)
    {
        for (col = 0; col < ncols; col++)
            printf("%lf ", grid[row][col].concentration);
        printf("\n");
    }

    return 0;
}

int generate_image(size_t nrows, size_t ncols, struct Node **grid, int it)
{
    size_t row, col, i;
    int r, g, b;
    char filename[BUFF_FILENAME_SIZE];
    double thresholds[] = {0.3, 0.2, 0.15, 0.07, 0.05, 0.02, 0.01, 0.005, 0.002, 0.0005};

    sprintf(filename, "output/%05d.ppm", it);
    FILE *fp = fopen(filename, "w");
    if (!fp)
    {
        perror("Error opening image file");
        exit(1);
    }

    fprintf(fp, "P3\n%d %d\n255\n", (int)ncols, (int)nrows);

    for (row = 0; row < nrows; row++)
    {
        for (col = 0; col < ncols; col++)
        {
            if (grid[row][col].type == SOURCE)
            {
                r = g = b = 0;
            }
            else if (grid[row][col].type == WATER)
            {
                r = 0;
                g = 100;
                b = 240;
            }
            else if (grid[row][col].concentration == 0.0)
            {
                r = 240;
                g = 240;
                b = 240;
            }
            else
            {
                r = 240;
                b = 0;
                for (i = 0; i <= 10; i++)
                {
                    if (grid[row][col].concentration > thresholds[i] || i == 10)
                    {
                        g = i * 24;
                        break;
                    }
                }
            }
            fprintf(fp, "%d %d %d  ", r, g, b);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
}
