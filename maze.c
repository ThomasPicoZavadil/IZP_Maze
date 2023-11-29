#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int rows;
    int cols;
    unsigned char *cells;
} Map;

void readfile()
{

    Map map;

    FILE *file = fopen("bludiste.txt", "r");
    if (file == NULL)
    {
        fprintf(stderr, "Error opening the file.\n");
    }
    if (fscanf(file, "%d %d", &map.rows, &map.cols) != 2)
    {
        fprintf(stderr, "Error reading the dimensions.\n");
        fclose(file);
    }
    map.cells = (unsigned char *)malloc(map.rows * map.cols * sizeof(unsigned char));
    if (map.cells == NULL)
    {
        fprintf(stderr, "Memory allocation error.\n");
        fclose(file);
    }
    for (int i = 0; i < map.rows * map.cols; i++)
    {
        if (fscanf(file, "%hhu", &map.cells[i]) != 1)
        {
            fprintf(stderr, "Error reading the matrix elements.\n");
            fclose(file);
            free(map.cells);
        }
    }
    fclose(file);
}

int main()
{
    Map map;
    readfile();
    for (int i = 0; i < sizeof(map.cells); i++)
    {
        printf("%d", map.cells);
    }
    free(map.cells);
}
/*
#include <stdio.h>
#include <stdlib.h>

    int
    main()
{
    FILE *file = fopen("your_file.txt", "r");
    if (file == NULL)
    {
        fprintf(stderr, "Error opening the file.\n");
        return 1;
    }

    int rows, cols;

    // Read the first line
    if (fscanf(file, "%d %d", &rows, &cols) != 2)
    {
        fprintf(stderr, "Error reading the dimensions.\n");
        fclose(file);
        return 1;
    }

    // Allocate memory for the 1D array
    int *matrix = (int *)malloc(rows * cols * sizeof(int));
    if (matrix == NULL)
    {
        fprintf(stderr, "Memory allocation error.\n");
        fclose(file);
        return 1;
    }

    // Read the rest of the numbers
    for (int i = 0; i < rows * cols; i++)
    {
        if (fscanf(file, "%d", &matrix[i]) != 1)
        {
            fprintf(stderr, "Error reading the matrix elements.\n");
            fclose(file);
            free(matrix);
            return 1;
        }
    }

    // Close the file
    fclose(file);

    // Now you have the matrix in the 'matrix' variable
    // Access elements like matrix[i * cols + j]

    // Your code here...

    // Don't forget to free the allocated memory when done
    free(matrix);

    return 0;
}
*/