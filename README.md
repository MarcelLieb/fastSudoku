# fastSudoku
A Sudoku Solver with speed and scalability in mind



To use the solver execute it in a commandline enviorment with a text file containing the Sudoku as an argument.

The solver will print all possible solutions to the Sudoku on the command line.

To solve the Sudoku a Branch and Bound algorithm is used, but also a Sudoku specific algorithm (based on how a human would solve a Sudoku) with Branch and Bound as Fallback.
Both algorithms will be executed and the computation time of both of the algorithms will be printed for comparision(For a better comparision comment out printMatrix() in BranchAndBound).

The Sudoku textfile must me formatted as follows:

* Each Sudoku row must be in a new seperate line
* The first row must be in the first line
* To indicate empty spaces use x, 0, # or *
* All characters except 0, 1, ... , 9, x, #, * or new lines will be ignored

This is optimized for quick and dirty copypasting.
