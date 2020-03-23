/*
Garrett Hope | Daniel Hanna
CSC 345-01
Project 2
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>

typedef struct
{
    int row;
    int col;
    int (*sudoku_board)[9];
} parameters;

void *checkRow(void *params)
{
    parameters *data = (parameters *)params;
    int startRow = data->row;
    int startCol = data->col;
    for (int i = startRow; i < 9; ++i)
    {
        int row[10] = {0};
        for (int j = startCol; j < 9; ++j)
        {
            int val = data->sudoku_board[i][j];
            if (row[val] != 0)
            {
                return (void *)0;
            }
            else
            {
                row[val] = 1;
            }
        }
    }
    return (void *)1;
}

void *checkCol(void *params)
{
    parameters *data = (parameters *)params;
    int startRow = data->row;
    int startCol = data->col;
    for (int i = startCol; i < 9; ++i)
    {
        int col[10] = {0};
        for (int j = startRow; j < 9; ++j)
        {
            int val = data->sudoku_board[j][i];
            if (col[val] != 0)
            {
                return (void *)0;
            }
            else
            {
                col[val] = 1;
            }
        }
    }
    return (void *)1;
}

void *check3x3(void *params)
{
    parameters *data = (parameters *)params;
    int startRow = data->row;
    int startCol = data->col;
    int saved[10] = {0};
    for (int i = startRow; i < startRow + 3; ++i)
    {
        for (int j = startCol; j < startCol + 3; ++j)
        {
            int val = data->sudoku_board[i][j];
            if (saved[val] != 0)
            {
                return (void *)0;
            }
            else
            {
                saved[val] = 1;
            }
        }
    }
    return (void *)1;
}

void solve1(int sudoku_board[9][9])
{ /* create 11 threads for first method */

    parameters *rowCol = (parameters *)malloc(sizeof(parameters));
    rowCol->row = 0;
    rowCol->col = 0;
    rowCol->sudoku_board = sudoku_board;

    parameters *first3x3 = (parameters *)malloc(sizeof(parameters));
    first3x3->row = 0;
    first3x3->col = 0;
    first3x3->sudoku_board = sudoku_board;

    parameters *second3x3 = (parameters *)malloc(sizeof(parameters));
    second3x3->row = 0;
    second3x3->col = 3;
    second3x3->sudoku_board = sudoku_board;

    parameters *third3x3 = (parameters *)malloc(sizeof(parameters));
    third3x3->row = 0;
    third3x3->col = 6;
    third3x3->sudoku_board = sudoku_board;

    parameters *fourth3x3 = (parameters *)malloc(sizeof(parameters));
    fourth3x3->row = 3;
    fourth3x3->col = 0;
    fourth3x3->sudoku_board = sudoku_board;

    parameters *fifth3x3 = (parameters *)malloc(sizeof(parameters));
    fifth3x3->row = 3;
    fifth3x3->col = 3;
    fifth3x3->sudoku_board = sudoku_board;

    parameters *sixth3x3 = (parameters *)malloc(sizeof(parameters));
    sixth3x3->row = 3;
    sixth3x3->col = 6;
    sixth3x3->sudoku_board = sudoku_board;

    parameters *seventh3x3 = (parameters *)malloc(sizeof(parameters));
    seventh3x3->row = 6;
    seventh3x3->col = 0;
    seventh3x3->sudoku_board = sudoku_board;

    parameters *eighth3x3 = (parameters *)malloc(sizeof(parameters));
    eighth3x3->row = 6;
    eighth3x3->col = 3;
    eighth3x3->sudoku_board = sudoku_board;

    parameters *ninth3x3 = (parameters *)malloc(sizeof(parameters));
    ninth3x3->row = 6;
    ninth3x3->col = 6;
    ninth3x3->sudoku_board = sudoku_board;

    pthread_t tid1, tid2, tid3, tid4, tid5, tid6, tid7, tid8, tid9, tid10, tid11;

    void *retrows;
    void *retcols;
    void *square1;
    void *square2;
    void *square3;
    void *square4;
    void *square5;
    void *square6;
    void *square7;
    void *square8;
    void *square9;

    pthread_create(&tid1, NULL, checkRow, (void *)rowCol);
    pthread_join(tid1, &retrows);

    pthread_create(&tid2, NULL, checkCol, (void *)rowCol);
    pthread_join(tid2, &retcols);

    pthread_create(&tid3, NULL, check3x3, (void *)first3x3);
    pthread_join(tid3, &square1);

    pthread_create(&tid4, NULL, check3x3, (void *)second3x3);
    pthread_join(tid4, &square2);

    pthread_create(&tid5, NULL, check3x3, (void *)third3x3);
    pthread_join(tid5, &square3);

    pthread_create(&tid6, NULL, check3x3, (void *)fourth3x3);
    pthread_join(tid6, &square4);

    pthread_create(&tid7, NULL, check3x3, (void *)fifth3x3);
    pthread_join(tid7, &square5);

    pthread_create(&tid8, NULL, check3x3, (void *)sixth3x3);
    pthread_join(tid8, &square6);

    pthread_create(&tid9, NULL, check3x3, (void *)seventh3x3);
    pthread_join(tid9, &square7);

    pthread_create(&tid10, NULL, check3x3, (void *)eighth3x3);
    pthread_join(tid10, &square8);

    pthread_create(&tid11, NULL, check3x3, (void *)ninth3x3);
    pthread_join(tid11, &square9);

    if ((int)retrows == 1 &&
        (int)retcols == 1 &&
        (int)square1 == 1 &&
        (int)square2 == 1 &&
        (int)square3 == 1 &&
        (int)square4 == 1 &&
        (int)square5 == 1 &&
        (int)square6 == 1 &&
        (int)square7 == 1 &&
        (int)square8 == 1 &&
        (int)square9 == 1)
    {
        printf("SOLUTION: YES");
    }
    else
    {
        printf("SOULTION: NO");
    }
}

void *checkRow2(void *params)
{
    parameters *data = (parameters *)params;
    int startRow = data->row;
    int startCol = data->col;
    int i = startRow;
    int row[10] = {0};
    for (int j = startCol; j < 9; ++j)
    {
        int val = data->sudoku_board[i][j];
        if (row[val] != 0)
        {
            return (void *)0;
        }
        else
        {
            row[val] = 1;
        }
    }
    return (void *)1;
}

void *checkCol2(void *params)
{
    parameters *data = (parameters *)params;
    int startRow = data->row;
    int startCol = data->col;
    int i = startCol;
    int col[10] = {0};
    for (int j = startRow; j < 9; ++j)
    {
        int val = data->sudoku_board[j][i];
        if (col[val] != 0)
        {
            return (void *)0;
        }
        else
        {
            col[val] = 1;
        }
    }
    return (void *)1;
}

void solve2(int sudoku_board[9][9])
{

    parameters *row0 = (parameters *)malloc(sizeof(parameters));
    row0->row = 0;
    row0->col = 0;
    row0->sudoku_board = sudoku_board;

    parameters *row1 = (parameters *)malloc(sizeof(parameters));
    row1->row = 1;
    row1->col = 0;
    row1->sudoku_board = sudoku_board;

    parameters *row2 = (parameters *)malloc(sizeof(parameters));
    row2->row = 2;
    row2->col = 0;
    row2->sudoku_board = sudoku_board;

    parameters *row3 = (parameters *)malloc(sizeof(parameters));
    row3->row = 3;
    row3->col = 0;
    row3->sudoku_board = sudoku_board;

    parameters *row4 = (parameters *)malloc(sizeof(parameters));
    row4->row = 4;
    row4->col = 0;
    row4->sudoku_board = sudoku_board;

    parameters *row5 = (parameters *)malloc(sizeof(parameters));
    row5->row = 5;
    row5->col = 0;
    row5->sudoku_board = sudoku_board;

    parameters *row6 = (parameters *)malloc(sizeof(parameters));
    row6->row = 6;
    row6->col = 0;
    row6->sudoku_board = sudoku_board;

    parameters *row7 = (parameters *)malloc(sizeof(parameters));
    row7->row = 7;
    row7->col = 0;
    row7->sudoku_board = sudoku_board;

    parameters *row8 = (parameters *)malloc(sizeof(parameters));
    row8->row = 8;
    row8->col = 0;
    row8->sudoku_board = sudoku_board;

    parameters *col0 = (parameters *)malloc(sizeof(parameters));
    col0->row = 0;
    col0->col = 0;
    col0->sudoku_board = sudoku_board;

    parameters *col1 = (parameters *)malloc(sizeof(parameters));
    col1->row = 0;
    col1->col = 1;
    col1->sudoku_board = sudoku_board;

    parameters *col2 = (parameters *)malloc(sizeof(parameters));
    col2->row = 0;
    col2->col = 2;
    col2->sudoku_board = sudoku_board;

    parameters *col3 = (parameters *)malloc(sizeof(parameters));
    col3->row = 0;
    col3->col = 3;
    col3->sudoku_board = sudoku_board;

    parameters *col4 = (parameters *)malloc(sizeof(parameters));
    col4->row = 0;
    col4->col = 4;
    col4->sudoku_board = sudoku_board;

    parameters *col5 = (parameters *)malloc(sizeof(parameters));
    col5->row = 0;
    col5->col = 5;
    col5->sudoku_board = sudoku_board;

    parameters *col6 = (parameters *)malloc(sizeof(parameters));
    col6->row = 0;
    col6->col = 6;
    col6->sudoku_board = sudoku_board;

    parameters *col7 = (parameters *)malloc(sizeof(parameters));
    col7->row = 0;
    col7->col = 7;
    col7->sudoku_board = sudoku_board;

    parameters *col8 = (parameters *)malloc(sizeof(parameters));
    col8->row = 0;
    col8->col = 8;
    col8->sudoku_board = sudoku_board;

    parameters *first3x3 = (parameters *)malloc(sizeof(parameters));
    first3x3->row = 0;
    first3x3->col = 0;
    first3x3->sudoku_board = sudoku_board;

    parameters *second3x3 = (parameters *)malloc(sizeof(parameters));
    second3x3->row = 0;
    second3x3->col = 3;
    second3x3->sudoku_board = sudoku_board;

    parameters *third3x3 = (parameters *)malloc(sizeof(parameters));
    third3x3->row = 0;
    third3x3->col = 6;
    third3x3->sudoku_board = sudoku_board;

    parameters *fourth3x3 = (parameters *)malloc(sizeof(parameters));
    fourth3x3->row = 3;
    fourth3x3->col = 0;
    fourth3x3->sudoku_board = sudoku_board;

    parameters *fifth3x3 = (parameters *)malloc(sizeof(parameters));
    fifth3x3->row = 3;
    fifth3x3->col = 3;
    fifth3x3->sudoku_board = sudoku_board;

    parameters *sixth3x3 = (parameters *)malloc(sizeof(parameters));
    sixth3x3->row = 3;
    sixth3x3->col = 6;
    sixth3x3->sudoku_board = sudoku_board;

    parameters *seventh3x3 = (parameters *)malloc(sizeof(parameters));
    seventh3x3->row = 6;
    seventh3x3->col = 0;
    seventh3x3->sudoku_board = sudoku_board;

    parameters *eighth3x3 = (parameters *)malloc(sizeof(parameters));
    eighth3x3->row = 6;
    eighth3x3->col = 3;
    eighth3x3->sudoku_board = sudoku_board;

    parameters *ninth3x3 = (parameters *)malloc(sizeof(parameters));
    ninth3x3->row = 6;
    ninth3x3->col = 6;
    ninth3x3->sudoku_board = sudoku_board;

    pthread_t tid1, tid2, tid3, tid4, tid5, tid6, tid7, tid8, tid9, tid10, tid11, tid12, tid13, tid14, tid15, tid16, tid17, tid18, tid19, tid20, tid21, tid22, tid23, tid24, tid25, tid26, tid27;

    void *retrow0;
    void *retrow1;
    void *retrow2;
    void *retrow3;
    void *retrow4;
    void *retrow5;
    void *retrow6;
    void *retrow7;
    void *retrow8;
    void *retcol0;
    void *retcol1;
    void *retcol2;
    void *retcol3;
    void *retcol4;
    void *retcol5;
    void *retcol6;
    void *retcol7;
    void *retcol8;
    void *square1;
    void *square2;
    void *square3;
    void *square4;
    void *square5;
    void *square6;
    void *square7;
    void *square8;
    void *square9;
    pthread_create(&tid1, NULL, checkRow2, (void *)row0);
    pthread_join(tid1, &retrow0);

    pthread_create(&tid2, NULL, checkRow2, (void *)row1);
    pthread_join(tid2, &retrow1);

    pthread_create(&tid3, NULL, checkRow2, (void *)row2);
    pthread_join(tid3, &retrow2);

    pthread_create(&tid4, NULL, checkRow2, (void *)row3);
    pthread_join(tid4, &retrow3);

    pthread_create(&tid5, NULL, checkRow2, (void *)row4);
    pthread_join(tid5, &retrow4);

    pthread_create(&tid6, NULL, checkRow2, (void *)row5);
    pthread_join(tid6, &retrow5);

    pthread_create(&tid7, NULL, checkRow2, (void *)row6);
    pthread_join(tid7, &retrow6);

    pthread_create(&tid8, NULL, checkRow2, (void *)row7);
    pthread_join(tid8, &retrow7);

    pthread_create(&tid9, NULL, checkRow2, (void *)row8);
    pthread_join(tid9, &retrow8);

    pthread_create(&tid10, NULL, checkCol2, (void *)col0);
    pthread_join(tid10, &retcol0);

    pthread_create(&tid11, NULL, checkCol2, (void *)col1);
    pthread_join(tid11, &retcol1);

    pthread_create(&tid12, NULL, checkCol2, (void *)col2);
    pthread_join(tid12, &retcol2);

    pthread_create(&tid13, NULL, checkCol2, (void *)col3);
    pthread_join(tid13, &retcol3);

    pthread_create(&tid14, NULL, checkCol2, (void *)col4);
    pthread_join(tid14, &retcol4);

    pthread_create(&tid15, NULL, checkCol2, (void *)col5);
    pthread_join(tid15, &retcol5);

    pthread_create(&tid16, NULL, checkCol2, (void *)col6);
    pthread_join(tid16, &retcol6);

    pthread_create(&tid17, NULL, checkCol2, (void *)col7);
    pthread_join(tid17, &retcol7);

    pthread_create(&tid18, NULL, checkCol2, (void *)col8);
    pthread_join(tid18, &retcol8);

    pthread_create(&tid19, NULL, check3x3, (void *)first3x3);
    pthread_join(tid19, &square1);

    pthread_create(&tid20, NULL, check3x3, (void *)second3x3);
    pthread_join(tid20, &square2);

    pthread_create(&tid21, NULL, check3x3, (void *)third3x3);
    pthread_join(tid21, &square3);

    pthread_create(&tid22, NULL, check3x3, (void *)fourth3x3);
    pthread_join(tid22, &square4);

    pthread_create(&tid23, NULL, check3x3, (void *)fifth3x3);
    pthread_join(tid23, &square5);

    pthread_create(&tid24, NULL, check3x3, (void *)sixth3x3);
    pthread_join(tid24, &square6);

    pthread_create(&tid25, NULL, check3x3, (void *)seventh3x3);
    pthread_join(tid25, &square7);

    pthread_create(&tid26, NULL, check3x3, (void *)eighth3x3);
    pthread_join(tid26, &square8);

    pthread_create(&tid27, NULL, check3x3, (void *)ninth3x3);
    pthread_join(tid27, &square9);

    if ((int)retrow0 == 1 &&
        (int)retrow1 == 1 &&
        (int)retrow2 == 1 &&
        (int)retrow3 == 1 &&
        (int)retrow4 == 1 &&
        (int)retrow5 == 1 &&
        (int)retrow6 == 1 &&
        (int)retrow7 == 1 &&
        (int)retrow8 == 1 &&
        (int)retcol0 == 1 &&
        (int)retcol1 == 1 &&
        (int)retcol2 == 1 &&
        (int)retcol3 == 1 &&
        (int)retcol4 == 1 &&
        (int)retcol5 == 1 &&
        (int)retcol6 == 1 &&
        (int)retcol7 == 1 &&
        (int)retcol8 == 1 &&
        (int)square1 == 1 &&
        (int)square2 == 1 &&
        (int)square3 == 1 &&
        (int)square4 == 1 &&
        (int)square5 == 1 &&
        (int)square6 == 1 &&
        (int)square7 == 1 &&
        (int)square8 == 1 &&
        (int)square9 == 1)
    {
        printf("SOLUTION: YES");
    }
    else
    {
        printf("SOULTION: NO");
    }
}

int main(int argc, char *argv[])
{
    double time_spent = 0.0;
    clock_t start, end;
    start = clock();

    int sudoku_board[9][9];
    FILE *fp;
    fp = fopen("input.txt", "r");
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            fscanf(fp, "%d", &sudoku_board[i][j]);
        }
    }


    printf("BOARD STATE IN %s\n", "input.txt");
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            printf("%d ", sudoku_board[i][j]);
        }
        printf("\n");
    }

    int method = atoi(argv[1]);
    if (method == 1)
    {
        solve1(sudoku_board);
    }
    else if (method == 2)
    {
        solve2(sudoku_board);
    }
    else
    {
        printf("You did not select a valid method, please enter '1' or '2'\n");
        exit(1);
    }
    end = clock();
    time_spent += ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("(%f) seconds\n", time_spent);

    return 0;
}