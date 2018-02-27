/*
 * This porgram implements the Game of Life by John Conway.
 * The rules are:
 * 1. Any live cell with fewer than two live neighbours dies 
 *    (underpopulation).
 * 2. Any live cell with two or three live neighbours lives to next 
 *    generation.
 * 3. Any live cell with more than three live neighbours dies 
 *    (overpopulation).
 * 4. Any dead cell with exactly three live neighbours comes alive.

Game Board: '*' represents alive cell; ' ' represents dead cell
..........................
.            ****        .
.      ***   ***         .
.           ****         .
.      *****   *         .
.         ********       .
..........................

Dots are used for creating the wrap around effect. Each dot is replaced by
the opposite side's value. These extra values are not printed, they're just 
used for calculating the rules correctly.

However, the input file provided with the -input option must follow the
following convention, for easier data entering purposes:

Input file game board: '1' represents alive cell; '0' represents dead cell
..........................
.000000000000111100000000.
.000000111000111000000000.
.000000000001111000000000.
.000000111110001000000000.
.000000000111111110000000.
..........................

Also the program internally uses this convention of 1s and 0s.
For a sample input file, see the "inputGame" file.

Usage:
Usage for generating a random board:
    <filename> -random rows columns [-time timeMilli] [-gen numberOfGen]
Usage for supplying an input board:
    <filename> -input inputGame [-time timeMilli] [-gen numberOfGen]
Note: default timeMilli is 200.
Note: default number of generations is infinite.
*/

/* This line used to enable nanoseconds */
#define _POSIX_C_SOURCE 199309L
/* Nanoseconds in a millisecond */
#define NANOMILLI 1000000

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

void usage(char *fileName);
bool * createGame(int isRand, int isInput, int *row, int *col, char **argv);
void nextGen(bool automaton[], bool automatonPadded[], int row, int col);
void addPadding(bool automaton[], bool automatonPadded[], int row, int col);
void printGame(bool automaton[], int row, int col);

int main(int argc, char *argv[]) {
    /* Default time per generation is 200 milliseconds */
    long timeN = 200 * NANOMILLI;
    struct timespec sleepValue = {0};
    int generations = 0;
    int i;
    int rand = 0;
    int input = 0;
    int row;
    int col;
    bool *automaton;
    bool *automatonPadded;

    if (argc < 3) {
        usage(argv[0]);
    }

    for (i = 1; i < argc; i++)
    {
        if(!strcmp(argv[i], "-random")) {
            rand = 1;
        }
        else if(!strcmp(argv[i], "-input")) {
            input = 1;
        }
        else if(!strcmp(argv[i], "-time")) {
            timeN = atoi(argv[i + 1]) * NANOMILLI;
        }
        else if(!strcmp(argv[i], "-gen")) {
            generations = atoi(argv[i + 1]);
        }
    }

    sleepValue.tv_nsec = timeN;

    automaton = createGame(rand, input, &row, &col, argv);

    automatonPadded = (bool*)malloc((row + 2) * (col + 2) * sizeof(bool));
    if (automatonPadded == NULL) {
        fprintf(stderr, "Error! Memory allocation failed!\n");
        exit(1);
    }

    if(generations) {
        for (i = 0; i < generations; i++) {
            printGame(automaton, row, col);
            addPadding(automaton, automatonPadded, row, col);
            nextGen(automaton, automatonPadded, row + 2, col + 2);
            nanosleep(&sleepValue, NULL);
        }
    }
    else {
        while (1) {
        printGame(automaton, row, col);
        addPadding(automaton, automatonPadded, row, col);
        nextGen(automaton, automatonPadded, row + 2, col + 2);
        nanosleep(&sleepValue, NULL);
        }
    }

    free(automaton);
    free(automatonPadded);

    return 0;
}

void usage(char *fileName) {
    fprintf(stderr, "Usage for generating a random board:\n\
    %s -random rows columns [-time timeMilli] [-gen numberOfGen]\n\n\
Usage for supplying an input board:\n\
    %s -input inputGame [-time timeMilli] [-gen numberOfGen]\n\n\
Note: default timeMilli is 200.\n\
Note: default number of generations is infinite.\n", fileName, fileName);
    exit(1);
}

bool * createGame(int isRand, int isInput, int *row, int *col, char **argv) {
    bool *automaton;
    char *fileName;
    FILE *fp;
    int c;
    int x = 0;
    int y = 0;
    int maxCol = 0;
    int i = 0;
    int mult = 0;

    if (isRand && !isInput) {
        srand(time(0));
        *row = atoi(argv[2]);
        *col = atoi(argv[3]);
        mult = *row * *col;

        automaton = (bool*)malloc(mult * sizeof(bool));
        if (automaton == NULL) {
            fprintf(stderr, "Error! Memory allocation failed!\n");
            exit(1);
        }
        for (i = 0; i < mult; i++) {
            automaton[i] = rand() % 2;
        }
        return automaton;
    }
    else if (!isRand && isInput) {
        fileName = argv[2];

        x = 0;
        y = 0;
        maxCol = 0;

        /* Figure out number of rows and columns in file */
        fp = fopen (fileName, "r");
        while((c = fgetc(fp)) != EOF) {
            if(c == '\n') {
                x++;
                /* maxCol used to avoid blank line at end of file issue */
                maxCol = y;
                y = 0;
            }
            else {
                y++;
            }
        }
        fclose(fp);

        /* Avoids issue where file ends in blank line.
         * File ending in blank line ends in EOF, not in newline.
         * Assigns row correctly */
        if(y != 0) {
            x++;
        }

        *row = x;        
        *col = maxCol;
        if((*row < 2) || (*col < 2)) {
            fprintf(stderr, "Input file formatting error!\n");
            exit(1);
        }
        mult = *row * *col;

        automaton = (bool*)malloc(mult * sizeof(bool));
        if (automaton == NULL) {
            fprintf(stderr, "Error! Memory allocation failed!\n");
            exit(1);
        }

        /* Put file contents into automaton array */
        x = 0;
        fp = fopen (fileName, "r");
        while(!feof(fp)) {
            c = fgetc(fp);
            if(c == '1') {
                automaton[x] = 1;
                x++;
            }
            else if (c == '0') {
                automaton[x] = 0;
                x++;
            }
        }
        fclose(fp);
        return automaton;
    }
    else {
        usage(argv[0]);
    }
    return 0;
}

void nextGen(bool *automaton, bool *automatonPadded, int row, int col) {
    int i;
    int j;
    int sumAlive;

    for (i = 0; i < row - 2; i++) {
        for (j = 0; j < col - 2; j++) {
            /* Compute sum of alive cells surrounding current cell */
            sumAlive = automatonPadded[col * i + j] + 
                       automatonPadded[col * i + j + 1] + 
                       automatonPadded[col * i + j + 2] + 
                       automatonPadded[col * (i + 1) + j] + 
                       automatonPadded[col * (i + 1) + j + 2] + 
                       automatonPadded[col * (i + 2) + j] + 
                       automatonPadded[col * (i + 2) + j + 1] + 
                       automatonPadded[col * (i + 2) + j + 2];
            /* Rules 1, 2, 3 */
            if(automaton[i * (col - 2) + j]) {
                if((sumAlive < 2) || (sumAlive > 3)) {
                    automaton[i * (col - 2) + j] = 0;
                }
            }
            else {
                /* Rule 4 */
                if(sumAlive == 3) {                    
                    automaton[i * (col - 2) + j] = 1;
                }
            }
            sumAlive = 0;
        }
    }
}

void addPadding(bool automaton[], bool automatonPadded[], int row, int col) {
    int i = 0;
    int j = 0;

    for (i = 0; i < col; i++) {
        /* Copying bottom row into top empty line */
        automatonPadded[i + 1] = automaton[col * (row - 1) + i];

        /* Copying top row into bottom empty line */
        automatonPadded[(col + 2) * (row + 1) + i + 1] = automaton[i];
    }

    for (i = 0; i < row; i++) {
        /* Copying right column into left empty line */
        automatonPadded[(col + 2) * (i + 1)] = 
            automaton[(col * i) + (col - 1) ];
        
        /* Copying left column into right empty line */
        automatonPadded[(col + 2) * (i + 2) - 1] = automaton[col * i];
    }

    /* Copying bottom-right corner into top-left empty corner */
    automatonPadded[0] = automaton[(col * row) - 1];

    /* Copying top-left corner into bottom-right empty corner */
    automatonPadded[(col + 2) * (row + 2) - 1] = automaton[0];

    /* Copying bottom-left corner into top-right empty corner */
    automatonPadded[(col + 2) - 1] = automaton[col * (row - 1)];

    /* Copying top-right corner into bottom-left empty corner */
    automatonPadded[(col + 2) * (row + 2 - 1)] = automaton[col - 1];

    /* Copying the original automaton into the center of the padded one */
    for (i = 0; i < row; i++) {
        for (j = 0; j < col; j++) {
            automatonPadded[(col + 2) * (i + 1) + j + 1] = 
                automaton[i * col + j];
        }
    }
}

void printGame(bool *automaton, int row, int col) {
    int i;
    int j;

    /* Clear screen */
    /* Could also be: printf("\e[1;1H\e[2J"); */
    printf("\x1B[1;1H\x1B[2J");

    for (i = 0; i < row; i++) {
        for (j = 0; j < col; j++) {
            printf("%c", automaton[i * col + j] ? '*' : ' ');
        }
        printf("\n");
    }
}