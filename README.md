# Conway's Game of Life
#### A terminal-based implementation in C

This program implements [The Game of Life](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life) by John Conway, in C.
#### The rules are:
1. Any live cell with fewer than two live neighbours dies 
   (underpopulation).
2. Any live cell with two or three live neighbours lives to next 
   generation.
3. Any live cell with more than three live neighbours dies 
   (overpopulation).
4. Any dead cell with exactly three live neighbours comes alive.

##### Game Board
The program supports the **RLE** file format, which stands for [Run Length Encoded](http://www.conwaylife.com/w/index.php?title=Run_Length_Encoded). This means that you can load and run any game board from over a thousand patterns hosted on the [LifeWiki](http://www.conwaylife.com/wiki/Main_Page) website.

For some sample rle input files, see the **\*.rle** files in the `inputPatterns` directory.

##### Custom Game Board: '1' represents an alive cell; '0' represents a dead cell
```text
000000000000111100000000
000000111000111000000100
000000000001111000000010
000000111110001000001110
000000000111111110000000
```
If you wish to provide your own custom patterns in plaintext format, you can do so by following this convention. All the rows must be the same length. Use a **1** to represent an alive cell and a **0** to represent a dead cell.

For a sample input file, see the **twoGosperGliderGuns** file in the `inputPatterns` directory.

##### Printed Board: 'o' represents an alive cell; ' ' represents a dead cell
```text
..........................
.            oooo        .
.      ooo   ooo      o  .
.           oooo       o .
.      ooooo   o     ooo .
.         oooooooo       .
..........................
```
The printed board consists of an '**o**' to represent an alive cell and a '**_**' (space) to represent a dead cell. The dots shown above are used for creating the wrap around effect. Each dot is replaced by the opposite side's value. These extra values are not printed, they're just used for calculating the rules correctly. This allows the automata to develop as if the board is continuous and wraps around itself.

#### Usage:
To compile, run:<br>
  `gcc gameOfLife.c -o <filename>`<br>
Generating a random board:<br>
  `<filename> -random rows columns [-time timeMilli] [-gen numberOfGen]`<br>
Supplying an input board:<br>
  `<filename> -input inputPattern [-time timeMilli] [-gen numberOfGen]`<br>
Note: default timeMilli is 200.  
Note: default number of generations is infinite.  
Note: if the inputPattern file ends in *.rle* the program will use the **RLE** format.

#### Examples:
1. Two Gosper Glider Guns<br>
Compile: <br>
  `gcc gameOfLife.c -o gameOfLife`<br>
Run: <br>
  `./gameOfLife -input twoGosperGliderGuns -time 100`<br>
This will run the game using the input file provided as input and updating the board every 100 milliseconds. Since *-gen* option was not provided, the program will run indefinitely (until user stops it with <kbd>CTRL + \\</kbd>).

Here is the resulting simulation:<br>
<img src="https://github.com/bluekeybo/GameOfLife/blob/master/media/twoGosperGliderGuns.gif" alt="Two Gosper Glider Guns" width="500"/>

2. Two Gliders<br>
Compile: <br>
  `gcc gameOfLife.c -o gameOfLife`<br>
Run: <br>
  `./gameOfLife -input twoGliders -time 100`<br>
This will run the game the same way as in the previous example.

Here is the resulting simulation:<br>
<img src="https://github.com/bluekeybo/GameOfLife/blob/master/media/twoGliders.gif" alt="Two Gliders" width="500"/>

3. Random Board<br>
Compile: <br>
  `gcc gameOfLife.c -o gameOfLife`<br>
Run: <br>
  `./gameOfLife -random 30 100 -time 150`<br>
This will generate a random board (with random dead and alive cells). The board size is 30 rows by 100 columns and the speed will be 150 milliseconds. The game will run for only 1000 generations.

Here is the resulting simulation:<br>
<img src="https://github.com/bluekeybo/GameOfLife/blob/master/media/randomBoard.gif" alt="Random Board" width="500"/>

4. **RLE** Board<br>
Compile: <br>
  `gcc gameOfLife.c -o gameOfLife`<br>
Run: <br>
  `./gameOfLife -input lightspeedoscillator1.rle`<br>
This will generate the board using the **RLE** format. The pattern used is the [*Light Speed Oscillator 1*](http://www.conwaylife.com/wiki/Light_speed_oscillator_1).

Here is the resulting simulation:<br>
<img src="https://github.com/bluekeybo/GameOfLife/blob/master/media/lightSpeedOscillator1.gif" alt="RLE Format Board" width="500"/>  
  
For details on Code of Conduct, Issue and Pull requests and Contributing Guidelines, please see the `docs` directory.  

Enjoy!
