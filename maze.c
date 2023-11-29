#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct
{
    int rows;
    int cols;
    unsigned char *cells;
} Map;

Map *readfile() // dodělat název souboru ze vstupu
{
    Map *map = (Map *)malloc(sizeof(Map));

    FILE *file = fopen("bludiste.txt", "r");
    if (file == NULL)
    {
        // // fprintf(stderr, "Error opening the file.\n");
        exit(1);
    }

    if (fscanf(file, "%d %d", &map->rows, &map->cols) != 2)
    {
        // // fprintf(stderr, "Error reading the dimensions.\n");
        fclose(file);
        exit(1);
    }

    map->cells = (unsigned char *)malloc(map->rows * map->cols * sizeof(unsigned char));
    if (map->cells == NULL)
    {
        // // fprintf(stderr, "Memory allocation error.\n");
        fclose(file);
        exit(1);
    }

    for (int i = 0; i < map->rows * map->cols; i++)
    {
        if (fscanf(file, "%hhu", &map->cells[i]) != 1)
        {
            // // fprintf(stderr, "Error reading the matrix elements.\n");
            fclose(file);
            free(map->cells);
            exit(1);
        }
    }

    fclose(file);
    return map;
}

bool isborder(Map *map, int r, int c, int border)
{
    return !!(map->cells[r * map->cols + c] & (border == 3 ? 4 : border));
}

// levá - 1, pravá - 2, horní/dolní - 3
int start_border(Map *map, int r, int c, int leftright)
{
    if (leftright == 0)
    {
        if (c == 1)
        {
            if (r % 2 == 1 && isborder(map, r - 1, c - 1, 1) == false)
            {
                // // fprintf(stdout, "PICA1\n");
                printf("%d, %d\n", r, c);
                return 2;
            }
            if (r % 2 == 0 && isborder(map, r - 1, c - 1, 1) == false)
            {
                // // fprintf(stdout, "PICA2\n");
                printf("%d, %d\n", r, c);
                return 3;
            }
        }
        if (c == map->cols)
        {
            if (r % 2 == 1 && isborder(map, r - 1, c - 1, 2) == false)
            {
                // // fprintf(stdout, "PICA3\n");
                printf("%d, %d\n", r, c);
                return 3;
            }
            if (r % 2 == 0 && isborder(map, r - 1, c - 1, 2) == false)
            {
                // // fprintf(stdout, "PICA4\n");
                printf("%d, %d\n", r, c);
                return 1;
            }
        }
        if (r == 1 && c % 2 == 1)
        {
            if (isborder(map, r - 1, c - 1, 3) == false)
            {
                // // fprintf(stdout, "PICA5\n");
                printf("%d, %d\n", r, c);
                return 1;
            }
        }
        if (r == map->rows)
        {
            if (r % 2 == 0 && c % 2 == 1 && isborder(map, r - 1, c - 1, 3) == false)
            {
                // // fprintf(stdout, "PICA6\n");
                printf("%d, %d\n", r, c);
                return 2;
            }
            if (r % 2 == 1 && c % 2 == 0 && isborder(map, r - 1, c - 1, 3) == false)
            {
                // // fprintf(stdout, "PICA7\n");
                printf("%d, %d\n", r, c);
                return 2;
            }
        }
        return 0;
    }
    return 0;
}

void r_path(Map *map, int r, int c, int border)
{
    int lastborder = border;
    while (true)
    {
        if (isborder(map, r - 1, c - 1, border) == false)
        {
            if (border == 1)
            {
                // // fprintf(stdout, "BEER1\n");
                c--;
                if ((r + c) % 2 == 0)
                {
                    // fprintf(stdout, "BEER2\n");
                    border = 3;
                    lastborder = border;
                    if (r < 1 || r > map->rows || c < 1 || c > map->cols)
                    {
                        break;
                    }
                    printf("%d, %d\n", r, c);
                }
                else
                {
                    // fprintf(stdout, "BEER3\n");
                    border = 1;
                    lastborder = border;
                    if (r < 1 || r > map->rows || c < 1 || c > map->cols)
                    {
                        break;
                    }
                    printf("%d, %d\n", r, c);
                }
            }
            if (border == 2)
            {
                // // fprintf(stdout, "BEER4\n");
                c++;
                if ((r + c) % 2 == 0)
                {
                    border = 2;
                    lastborder = border;
                    // fprintf(stdout, "BEER5\n");
                    if (r < 1 || r > map->rows || c < 1 || c > map->cols)
                    {
                        break;
                    }
                    printf("%d, %d\n", r, c);
                }
                else
                {
                    border = 3;
                    lastborder = border;
                    // fprintf(stdout, "BEER6\n");
                    if (r < 1 || r > map->rows || c < 1 || c > map->cols)
                    {
                        break;
                    }
                    printf("%d, %d\n", r, c);
                }
            }
            if (border == 3)
            {
                // // fprintf(stdout, "BEER7\n");
                if ((r + c) % 2 == 0)
                {
                    // // fprintf(stdout, "BEER8\n");
                    r--;
                }
                else
                {
                    // // fprintf(stdout, "BEER9\n");
                    r++;
                }
                if ((r + c) % 2 == 0)
                {
                    // fprintf(stdout, "BEER_10\n");
                    border = 1;
                    lastborder = border;
                    if (r < 1 || r > map->rows || c < 1 || c > map->cols)
                    {
                        break;
                    }
                    printf("%d, %d\n", r, c);
                }
                else
                {
                    // fprintf(stdout, "BEER_11\n");
                    border = 2;
                    lastborder = border;
                    if (r < 1 || r > map->rows || c < 1 || c > map->cols)
                    {
                        break;
                    }
                    printf("%d, %d\n", r, c);
                }
            }
        }
        else
        {
            if (lastborder == 1)
            {
                if ((r + c) % 2 == 1)
                {
                    // fprintf(stdout, "Vlevo stena 1\n");
                    border = 3;
                    lastborder = border;
                }
                else
                {
                    // fprintf(stdout, "Vlevo stena 2\n");
                    border = 2;
                    lastborder = border;
                }
            }
            if (lastborder == 2)
            {
                if ((r + c) % 2 == 1)
                {
                    // fprintf(stdout, "Vpravo stena 1\n");
                    border = 1;
                    lastborder = border;
                }
                else
                {
                    // fprintf(stdout, "Vpravo stena 2\n");
                    border = 3;
                    lastborder = border;
                }
            }
            if (lastborder == 3)
            {
                if ((r + c) % 2 == 1)
                {
                    // fprintf(stdout, "Nahore/dole stena 1\n");
                    border = 2;
                    lastborder = border;
                }
                else
                {
                    // fprintf(stdout, "Nahore/dole stena 2\n");
                    border = 1;
                    lastborder = border;
                }
            }
        }
    }
}

int main(int argc, char *argv[])
{
    Map *map = readfile();
    /* for (int i = 0; i < map->cols * map->rows; i++)
     {
         printf("%d ", map->cells[i]);
     }*/
    if (argc > 5)
    {
        // // fprintf(stderr, "Picovina");
        return 1;
    }
    int r = atoi(argv[1]);
    int c = atoi(argv[2]);
    int border = start_border(map, r, c, 0);

    r_path(map, r, c, border);
    /*
        for (int i = 0; i < map->rows; i++)
        {
            for (int j = 0; j < map->cols; j++)
            {
                printf("%hhu ", map->cells[i * map->cols + j]);
            }
            printf("\n");
        }
    */
    free(map->cells);

    return 0;
}
