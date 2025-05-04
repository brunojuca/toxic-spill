#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"
#include "node.h"
#include "compute_spill.h"

int main(int argc, char const *argv[])
{
    char filename[BUFF_FILENAME_SIZE], line[BUFF_INT_LINE_SIZE], *endptr, ch;
    FILE *file;
    struct Node *data, **grid;
    size_t nrows, ncols, i, row, col;

    if (argc > 1)
        snprintf(filename, sizeof(filename), "%s", argv[1]);
    else
    {
        printf("Enter the input filename: ");
        if (fgets(filename, sizeof(filename), stdin) == NULL)
            throwInputError();

        size_t len = strlen(filename);
        if (len > 0 && filename[len - 1] == '\n')
            filename[len - 1] = '\0';
    }

    file = fopen(filename, "r");

    if (!file)
    {
        perror("Failed to open file");
        return 1;
    }

    if (fgets(line, sizeof(char) * BUFF_INT_LINE_SIZE, file) == NULL)
        throwInputError();
    nrows = strtol(line, &endptr, 10);
    if (fgets(line, sizeof(char) * BUFF_INT_LINE_SIZE, file) == NULL)
        throwInputError();
    ncols = strtol(line, &endptr, 10);

    data = (struct Node *)malloc(nrows * ncols * sizeof(struct Node));
    grid = (struct Node **)malloc(nrows * sizeof(struct Node *));

    if (data == NULL)
        throwMallocError();

    if (grid == NULL)
    {
        free(data);
        throwMallocError();
    }

    for (i = 0; i < nrows; i++)
        grid[i] = &data[i * ncols];

    for (row = 0; row < nrows; row++)
        for (col = 0; col < ncols; col++)
        {
            do
            {
                ch = fgetc(file);
                if (ch == EOF)
                {
                    free(grid);
                    free(data);
                    fclose(file);
                    throwInputError();
                }
            } while (ch == '\n' || ch == '\r' || ch == ' ');
            grid[row][col].type = ch - '0';
            grid[row][col].concentration = grid[row][col].type == SOURCE ? 1.0 : 0.0;
        }

    grid = compute_spill(nrows, ncols, grid);

    free(data);
    free(grid);
    fclose(file);
    return 0;
}
