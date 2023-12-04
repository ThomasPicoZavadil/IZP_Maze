/**********************************
        Projekt IZP - Maze
        Tomáš Zavadil
        xzavadt00
**********************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct
{
    int rows;
    int cols;
    unsigned char *cells;
} Map;

// funkce pro načtení dat ze souboru
Map *readfile(char *filename)
{
    Map *map = (Map *)malloc(sizeof(Map));

    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        free(map);
        exit(1);
    }

    if (fscanf(file, "%d %d", &map->rows, &map->cols) != 2)
    {
        free(map);
        fclose(file);
        exit(1);
    }

    map->cells = (unsigned char *)malloc(map->rows * map->cols * sizeof(unsigned char));
    if (map->cells == NULL)
    {
        free(map);
        fclose(file);
        exit(1);
    }

    for (int i = 0; i < map->rows * map->cols; i++)
    {
        if (fscanf(file, "%hhu", &map->cells[i]) != 1)
        {
            fclose(file);
            free(map->cells);
            free(map);
            exit(1);
        }
    }

    fclose(file);
    return map;
}

// funkce pro zjištění zda je stěna průchozí neprůchozí
bool isborder(Map *map, int r, int c, int border)
{
    return !!(map->cells[r * map->cols + c] & (border == 3 ? 4 : border));
}

bool isborder_test(Map *map, int i, int border)
{
    return !!(map->cells[i] & (border == 3 ? 4 : border));
}

// funkce pro zjištění prvního směru v labyrintu
// levá - 1, pravá - 2, horní/dolní - 3
int start_border(Map *map, int r, int c, int leftright)
{
    if (leftright == 0)
    {
        if (c == 1)
        {
            if (r % 2 == 1 && isborder(map, r - 1, c - 1, 1) == false)
            {
                return 2;
            }
            if (r % 2 == 0 && isborder(map, r - 1, c - 1, 1) == false)
            {
                return 3;
            }
        }
        if (c == map->cols)
        {
            if (r % 2 == 1 && isborder(map, r - 1, c - 1, 2) == false)
            {
                return 3;
            }
            if (r % 2 == 0 && isborder(map, r - 1, c - 1, 2) == false)
            {
                return 1;
            }
        }
        if (r == 1 && c % 2 == 1)
        {
            if (isborder(map, r - 1, c - 1, 3) == false)
            {
                return 1;
            }
        }
        if (r == map->rows)
        {
            if (isborder(map, r - 1, c - 1, 3) == false)
            {
                return 2;
            }
        }
        return 0;
    }
    else if (leftright == 1)
    {
        if (c == 1)
        {
            if (r % 2 == 1 && isborder(map, r - 1, c - 1, 1) == false)
            {
                return 3;
            }
            if (r % 2 == 0 && isborder(map, r - 1, c - 1, 1) == false)
            {
                return 2;
            }
            else
            {
                printf("Spatny vstup");
                exit(1);
            }
        }
        if (c == map->cols)
        {
            if (r % 2 == 1 && isborder(map, r - 1, c - 1, 2) == false)
            {
                return 1;
            }
            if (r % 2 == 0 && isborder(map, r - 1, c - 1, 2) == false)
            {
                return 3;
            }
            else
            {
                printf("Spatny vstup");
                exit(1);
            }
        }
        if (r == 1 && c % 2 == 1)
        {
            if (isborder(map, r - 1, c - 1, 3) == false)
            {
                return 2;
            }
            else
            {
                printf("Spatny vstup");
                exit(1);
            }
        }
        if (r == map->rows)
        {
            if (isborder(map, r - 1, c - 1, 3) == false)
            {
                return 1;
            }
            else
            {
                printf("Spatny vstup");
                exit(1);
            }
        }
        return 0;
    }
    return 0;
}

// funkce pro testování bludiště, zda je validní (nefunkční)
void test(Map *map)
{
    int cols_amount = 1;
    int rows_amount = 1;
    int i = 0;
    char symbol;
    while (i < map->cols * map->rows)
    {
        symbol = map->cells[i];
        if (!(symbol >= 0 && symbol <= 7))
        {
            printf("Invalid\n");
            exit(1);
        }
        if (cols_amount < map->cols)
        {
            if (isborder_test(map, i, 2) != isborder_test(map, i + 1, 1))
            {
                printf("Invalid\n");
                exit(1);
            }
        }
        if (rows_amount < map->rows)
        {
            if (rows_amount % 2 == 0 && (cols_amount != 1 && cols_amount != map->cols))
            {
                if (cols_amount % 2 == 1)
                {
                    if (isborder_test(map, i, 3) != isborder_test(map, i + map->cols, 3))
                    {
                        printf("Invalid\n");
                        exit(1);
                    }
                }
            }
            else if (rows_amount % 2 == 1)
            {
                if (cols_amount % 2 == 0)
                {
                    if (isborder_test(map, i, 3) != isborder_test(map, i + map->cols, 3))
                    {
                        printf("Invalid\n");
                        exit(1);
                    }
                }
            }
        }
        if (cols_amount == map->cols && (rows_amount != map->rows))
        {
            rows_amount++;
            cols_amount = 0;
        }
        cols_amount++;
        i++;
    }
    if (rows_amount != map->rows || cols_amount - 1 != map->cols)
    {
        printf("Invalid\n");
        exit(1);
    }
    else
    {
        printf("Valid");
    }
}

// funkce pro průchod bludištěm, pomocí pravidla pravé ruky
void r_path(Map *map, int r, int c, int border)
{
    int lastborder = border;
    while (true)
    {

        if (r < 1 || r > map->rows || c < 1 || c > map->cols)
        {
            break;
        }

        if (!isborder(map, r - 1, c - 1, border))
        {
            printf("%d, %d\n", r, c);
            if (border == 1)
            {
                c--;
                if ((r + c) % 2 == 0)
                {
                    border = 3;
                }
                else
                {
                    border = 1;
                }

                lastborder = 2;
            }
            else if (border == 2)
            {
                c++;
                if ((r + c) % 2 == 0)
                {
                    border = 2;
                }
                else
                {
                    border = 3;
                }

                lastborder = 1;
            }
            else if (border == 3)
            {
                if ((r + c) % 2 == 0)
                {
                    r--;
                }
                else
                {
                    r++;
                }

                if ((r + c) % 2 == 0)
                {
                    border = 1;
                }
                else
                {
                    border = 2;
                }

                lastborder = 3;
            }
        }
        else
        {
            if (lastborder == 1)
            {
                if ((r + c) % 2 == 1)
                {
                    lastborder = border;
                    border = 3;
                }
                else
                {
                    lastborder = border;
                    border = 2;
                }
            }
            else if (lastborder == 2)
            {
                if ((r + c) % 2 == 1)
                {
                    lastborder = border;
                    border = 1;
                }
                else
                {
                    lastborder = border;
                    border = 3;
                }
            }
            else if (lastborder == 3)
            {
                if ((r + c) % 2 == 1)
                {
                    lastborder = border;
                    border = 2;
                }
                else
                {
                    lastborder = border;
                    border = 1;
                }
            }
        }
    }
}

// funkce pro průchod bludištěm, pomocí pravidla levé ruky
void l_path(Map *map, int r, int c, int border)
{
    int lastborder = border;
    while (true)
    {

        if (r < 1 || r > map->rows || c < 1 || c > map->cols)
        {
            break;
        }

        if (!isborder(map, r - 1, c - 1, border))
        {
            printf("%d, %d\n", r, c);
            if (border == 1)
            {
                c--;
                if ((r + c) % 2 == 0)
                {
                    border = 1;
                }
                else
                {
                    border = 3;
                }

                lastborder = 2;
            }
            else if (border == 2)
            {
                c++;
                if ((r + c) % 2 == 0)
                {
                    border = 3;
                }
                else
                {
                    border = 2;
                }

                lastborder = 1;
            }
            else if (border == 3)
            {
                if ((r + c) % 2 == 0)
                {
                    r--;
                }
                else
                {
                    r++;
                }

                if ((r + c) % 2 == 0)
                {
                    border = 2;
                }
                else
                {
                    border = 1;
                }

                lastborder = 3;
            }
        }
        else
        {
            if (lastborder == 1)
            {
                if ((r + c) % 2 == 1)
                {
                    lastborder = border;
                    border = 2;
                }
                else
                {
                    lastborder = border;
                    border = 3;
                }
            }
            else if (lastborder == 2)
            {
                if ((r + c) % 2 == 1)
                {
                    lastborder = border;
                    border = 3;
                }
                else
                {
                    lastborder = border;
                    border = 1;
                }
            }
            else if (lastborder == 3)
            {
                if ((r + c) % 2 == 1)
                {
                    lastborder = border;
                    border = 1;
                }
                else
                {
                    lastborder = border;
                    border = 2;
                }
            }
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc < 1)
    {
        printf("Zadejte argumenty");
    }
    if (strcmp(argv[1], "--help") == 0)
    {
        printf("Program se musi spustit s argumentem:\n");
        printf("--test: Kontrola, ze soubor obsahuje spravnou definici bludiste\n");
        printf("--rpath R C soubor.txt: Hleda pruchod bludistem pomoci pravidla prave ruky ze vstupu na radku R a sloupci C\n");
        printf("--lpath R C soubor.txt: Hleda pruchod bludistem pomoci pravidla leve ze vstupu na radku R a sloupci C\n");
        exit(1);
    }
    if (strcmp(argv[1], "--test") == 0)
    {
        char *filename = argv[2];
        Map *map = readfile(filename);
        test(map);
        free(map->cells);
        free(map);
    }
    if (strcmp(argv[1], "--rpath") == 0)
    {
        char *filename = argv[4];
        Map *map = readfile(filename);
        int r = atoi(argv[2]);
        int c = atoi(argv[3]);
        int border = start_border(map, r, c, 0);
        r_path(map, r, c, border);
        free(map->cells);
        free(map);
    }
    if (strcmp(argv[1], "--lpath") == 0)
    {
        char *filename = argv[4];
        Map *map = readfile(filename);
        int r = atoi(argv[2]);
        int c = atoi(argv[3]);
        int border = start_border(map, r, c, 1);
        l_path(map, r, c, border);
        free(map->cells);
        free(map);
    }
    return 0;
}
