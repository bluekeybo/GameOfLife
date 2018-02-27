# Conway's Game of Life
#### A terminal-based implementation in C

This program implements the Game of Life by John Conway in C.
#### The rules are:
1. Any live cell with fewer than two live neighbours dies 
   (underpopulation).
2. Any live cell with two or three live neighbours lives to next 
   generation.
3. Any live cell with more than three live neighbours dies 
   (overpopulation).
4. Any dead cell with exactly three live neighbours comes alive.

##### Game Board: 'o' represents an alive cell; ' ' represents a dead cell
```text
..........................
.            oooo        .
.      ooo   ooo      o  .
.           oooo       o .
.      ooooo   o     ooo .
.         oooooooo       .
..........................
```

Dots are used for creating the wrap around effect. Each dot is replaced by the opposite side's value. These extra values are not printed, they're just used for calculating the rules correctly. This allows the automata to develop as if the board size is infinite.

However, the input file provided with the *-input* option must adhere to the following convention, for easier data entering purposes:

##### Input file game board: '1' represents an alive cell; '0' represents a dead cell
```text
000000000000111100000000
000000111000111000000000
000000000001111000000000
000000111110001000000000
000000000111111110000000
```

Also the program internally uses this convention of 1s and 0s.

For a sample input file, see the **inputGame** file in the **_src_** directory.

#### Usage:
To compile, run:<br>
  `gcc gameOfLife.c -o <filename>`<br>
Generating a random board:<br>
  `<filename> -random rows columns [-time timeMilli] [-gen numberOfGen]`<br>
Supplying an input board:<br>
  `<filename> -input inputGame [-time timeMilli] [-gen numberOfGen]`<br>
Note: default timeMilli is 200.<br>
Note: default number of generations is infinite.

#### Example:
Compile: <br>
  `gcc gameOfLife.c -o gameOfLife`<br>
Run: <br>
  `./gameOfLife -input twoGosperGliderGuns -time 150`<br>
This will run the game using the input file provided as input and updating the board every 150 milliseconds. Since *-gen* option was not provided, the program will run indefinitely (until user stops it with CTRL + \\).

Here is the resulting simulation:<br>
![Two Gosper Glider Guns](https://github.com/bluekeybo/GameOfLife/blob/master/media/twoGosperGliderGuns.gif)
<img src="https://github.com/bluekeybo/GameOfLife/blob/master/media/twoGosperGliderGuns.gif" alt="Two Gosper Glider Guns" style="width: 200px;"/>