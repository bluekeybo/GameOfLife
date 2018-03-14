/*
This program implements the Game of Life by John Conway.
The rules are:
1. Any live cell with fewer than two live neighbours dies 
   (underpopulation).
2. Any live cell with two or three live neighbours lives to next 
   generation.
3. Any live cell with more than three live neighbours dies 
   (overpopulation).
4. Any dead cell with exactly three live neighbours comes alive.

See README.md for more information.
*/

/* This line used to enable nanoseconds */
#define _POSIX_C_SOURCE 199309L
/* Nanoseconds in a millisecond */
#define NANOMILLI 1000000
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

void usage(char *fileName);
bool * createGame(int isRand, int isInput, int *row, int *col, char *fileName);
bool * rleGame(int *row, int *col, char *fileName);
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
    char *fileName;

    if (argc < 3) {
        usage(argv[0]);
    }

    for (i = 1; i < argc; i++)
    {
        if(!strcmp(argv[i], "-random")) {
            rand = 1;
            if((i + 2) < argc ) {
                row = atoi(argv[i + 1]);
                col = atoi(argv[i + 2]);
            }
            else {
                usage(argv[0]);
            }
            
        }
        else if(!strcmp(argv[i], "-input")) {
            input = 1;
            if((i + 1) < argc ) {
                fileName = argv[i + 1];
            }
            else {
                usage(argv[0]);
            }
        }
        else if(!strcmp(argv[i], "-time")) {
            if((i + 1) < argc ) {
                timeN = atoi(argv[i + 1]) * NANOMILLI;
            }
            else {
                usage(argv[0]);
            }
        }
        else if(!strcmp(argv[i], "-gen")) {
            generations = atoi(argv[i + 1]);
        }
    }

    /* Making sure only one game option has been set */
    if(!(rand ^ input)) {
        usage(argv[0]);
    }

    sleepValue.tv_nsec = timeN;

    /* Checking if an rle file has been provided */
     if(input && strstr(fileName, ".rle")) {
         automaton = rleGame(&row, &col, fileName);
     }
     else {
         automaton = createGame(rand, input, &row, &col, fileName);
     }

    automatonPadded = (bool*)malloc((row + 2) * (col + 2) * sizeof(bool));
    if (automatonPadded == NULL) {
        fprintf(stderr, "Error! Memory allocation failed!\n");
        exit(1);
    }

    if(generations) {
        for (i = 0; i < generations + 1; i++) {
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


/* This is a usage function that shows correct program invocation.
*  Arguments: the filename.
*  Returns: none.
*/
void usage(char *fileName) {
    fprintf(stderr, "Usage for generating a random board:\n\
    %s -random rows columns [-time timeMilli] [-gen numberOfGen]\n\n\
Usage for supplying an input board:\n\
    %s -input inputGame [-time timeMilli] [-gen numberOfGen]\n\n\
Notes: default timeMilli is 200 (board updates every 200 milliseconds).\n\
      default number of generations is infinite.\n\
      To cancel an infinite program at any time just press CTRL + \\.\n", 
        fileName, fileName);
    exit(1);
}


/* This function creates the gameboard if rle file is used.
*  Arguments: rows, columns (set by the function to match automaton),
*       filename.
*  Returns: automaton array for the game board.
*/
bool * rleGame(int *row, int *col, char *fileName) {
    bool *automaton;
    FILE *fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t nread;
    int i = 0;
    char numbers[10];
    unsigned int val = 0;
    char c;
    unsigned iter = 0;
    int colCounter = 0;

    fp = fopen (fileName, "r");
    if(!fp) {
        fprintf(stderr, "Could not open file!\n");
        exit(1);
    }

    while ((nread = getline(&line, &len, fp)) != -1) {
        if(line[0] == '#') {
            continue;
        }
        else if(line[0] == 'x') {
            sscanf(line, "x = %d, y = %d", col, row);
            /* Addign extra row and column to avoid unintended wrap-around */
            (*row) += 2;
            (*col) += 2;
            automaton = (bool*)calloc((*row * *col), sizeof(bool));
            if (automaton == NULL) {
                fprintf(stderr, "Error! Memory allocation failed!\n");
                exit(1);
            }
            break;
        }      
    }

    colCounter = 0;
    
    /* Adding a blank line as the first line */
    iter = *col + 1;

    /* Numbers are separated from letters. A 0 is placed when a 1 is omitted. */
    while((c = fgetc(fp)) != EOF) {
        if(isdigit(c)) {
            numbers[i++] = c;
        }
        else {
            numbers[i] = '\0';
            val = atoi(numbers);

            /* Increment val if it's a zero. It represents an omitted 1 */
            if(!val) {
                val++;
            }
            if (c == 'o') {
                colCounter += val;
                val += iter;
                for (; iter < val; iter++) {
                    automaton[iter] = 1;
                }
            }
            else if (c == '$'){
                /* Skipping all the new lines */
                /* Adding +2 to skip the added empty cells */
                /* colCounter term is used in case $ assumes the rest of the
                    line is filled with zeros */
                iter += (*col * (val - 1)) + 2 + (*col - colCounter - 2);
                colCounter = 0;
            }
            else if (c == 'b') {
                colCounter += val;
                iter += val;
            }
            i = 0;
        }
    }

    fclose(fp);

    return automaton;
}


/* This function creates the gameboard for a custom input board file
*  Arguments: isRand, isInput (flags to check if random board or custom one).
*       rows, columns (set by the function to match automaton), filename.
*  Returns: automaton array for the game board.
*/
bool * createGame(int isRand, int isInput, int *row, int *col, char *fileName) {
    bool *automaton;
    FILE *fp;
    int c;
    int x = 0;
    int y = 0;
    int maxCol = 0;
    int i = 0;
    int mult = 0;

    if (isRand) {
        srand(time(0));
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
    else if (isInput) {
        x = 0;
        y = 0;
        maxCol = 0;

        /* Figure out number of rows and columns in file */
        fp = fopen (fileName, "r");
        if(!fp) {
            fprintf(stderr, "Could not open file!\n");
            exit(1);
        }
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
        if(!fp) {
            fprintf(stderr, "Could not open file!\n");
            exit(1);
        }
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
    return 0;
}


/* This function creates the next generation of the game. The current 
*       generation is in automatonPadded. The next generation is in 
*       automaton. The row and col are from automatonPadded (so row and col
*       are both 2 more than the non-padded row and col).
*  Arguments: current automaton, padded automaton, row, col.
*  Returns: none.
*/
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


/* This function adds padding to the current automaton board. The padding is
*       two extra rows and columns, having a value of the opposite row/col. 
*       This is used to create the wrap-around effect, where if a cell 
*       disappears on the edge, it reappears on the other side of the board.
*       The row and col are from automaton.
*  Arguments: automaton, padded automaton, row, col.
*  Returns: none.
*/
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


/* This function prints the board game to terminal. The row and col are from 
*       automaton.
*  Arguments: automaton, row, col.
*  Returns: none.
*/
void printGame(bool *automaton, int row, int col) {
    int i;
    int j;

    /* Clear screen */
    /* Could also be: printf("\e[1;1H\e[2J"); */
    printf("\x1B[1;1H\x1B[2J");

    for (i = 0; i < row; i++) {
        for (j = 0; j < col; j++) {
            printf("%c", automaton[i * col + j] ? 'o' : ' ');
        }
        printf("\n");
    }
}