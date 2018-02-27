# Conway's Game of Life
#### A terminal-based implementation in C
---
This program implements the Game of Life by John Conway in C.
The rules are:
1. Any live cell with fewer than two live neighbours dies 
   (underpopulation).
2. Any live cell with two or three live neighbours lives to next 
   generation.
3. Any live cell with more than three live neighbours dies 
   (overpopulation).
4. Any dead cell with exactly three live neighbours comes alive.

##### Game Board: 'o' represents alive cell; ' ' represents dead cell
```text
..........................
.            oooo        .
.      ooo   ooo      o  .
.           oooo       o .
.      ooooo   o     ooo .
.         oooooooo       .
..........................
```

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