#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
 * This porgram implements the Game of Life by John Conway.
 * The rules are:
 * Any live cell with fewer than two live neighbours dies (underpopulation).
 * Any live cell with two or three live neighbours lives to next generation.
 * Any live cell with more than three live neighbours dies (overpopulation).
 * Any dead cell with exactly three live neighbours comes alive.
 */

/* Game Board: '*' represents alive cell; ' ' represents dead cell
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
'1' represents alive cell; '0' represents dead cell.
For a sample input file, see the "inputGame" file.
*/

void usage(char *fileName);
int * createGame(int isRand, int isInput, int *row, int *col, char **argv);
void nextGen(int automaton[], int *row, int *col);
void addPadding(int automaton[], int *row, int *col);
void printGame(int automaton[], int *row, int *col);


int main(int argc, char *argv[]) {
    int i;
    int rand = 0;
    int input = 0;
    int row;
    int col;
    int *automaton;

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
    }

    automaton = createGame(rand, input, &row, &col, argv);

    printGame(automaton, &row, &col);

    free(automaton);

    return 0;
}

void usage(char *fileName) {
    fprintf(stderr, "usage: %s [-random rows columns] [-input inputGame]\n",
         fileName);
    exit(1);
}

int * createGame(int isRand, int isInput, int *row, int *col, char **argv) {
    int *automaton;
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

        automaton = (int*)calloc(mult, sizeof(int));
        if (automaton == NULL) {
            fprintf(stderr, "Error! Memory allocation failed!\n");
            exit(1);
        }
        for (i = 0; i < mult; i++) {
            if (rand() % 2) {
                automaton[i] = '*';
            }
            else {
                automaton[i] = ' ';
            }
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

        automaton = (int*)calloc(mult, sizeof(int));
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
                automaton[x] = '*';
                x++;
            }
            else if (c == '0') {
                automaton[x] = ' ';
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

void nextGen(int *automaton, int *row, int *col) {


}

void addPadding(int automaton[], int *row, int *col) {


}

void printGame(int *automaton, int *row, int *col) {
    int i;
    int j;
    int index;

    /* Clear screen */
    /* Could also be: printf("\e[1;1H\e[2J"); */
    printf("\x1B[1;1H\x1B[2J");

    for (i = 0; i < *row; i++) {
        for (j = 0; j < *col; j++) {
            index = i * *col + j;
            printf("%c", automaton[index]);
        }
        printf("\n");
    }
}