# Word Search Solver

A command-line word search solver written in C. It loads a square letter grid from a text file, asks for a word, and searches the grid in all eight directions (horizontal, vertical, and diagonal, forwards and backwards). If the word is found, it prints the grid with the position of each letter numbered along the path.

This was built for **CSE 031, Project 01**, which required solving the puzzle with pointer notation only and without library case-conversion functions.

## Features

- Case-insensitive search (`HelLo` matches `hello`)
- All eight directions, with backtracking when a path hits a dead end
- Prints the puzzle, then the search path as a numbered grid
- **Multiple occurrences:** if the word appears more than once with different starting letters, every occurrence is shown in the same grid
- Letters that belong to several paths show their positions joined together (for example `246` means that cell is the 2nd, 4th, and 6th letter)
- Handles a missing puzzle file and incorrect command-line usage

## Requirements

- A C compiler such as `gcc` or `clang`
- Standard C library only, no other dependencies

## Build and Run

```bash
gcc -Wall -o wordsearch wordsearch3.c
./wordsearch puzzle1.txt
```

The program then prompts for the word to search for:

```
Enter the word to search: HelLo
```

Running it with no arguments prints a usage message. Passing a file that doesn't exist prints `Cannot Open Puzzle File!`.

## Puzzle File Format

The first line is the grid size `N`, followed by `N` rows of `N` letters separated by spaces. For example, `puzzle1.txt` is a 5 × 5 grid:

```
5
W E B M O
I L H L L
M L Z E L
M Y E K O
A O A B A
```

Three sample puzzles are included: `puzzle1.txt` (5 × 5), `puzzle2.txt` (6 × 6), and `puzzle3.txt` (5 × 5).

## Example Runs

Output is shown below with the tab separators between columns replaced by spaces for readability.

**Searching `HelLo` in `puzzle1.txt`**

```
Enter the word to search: HelLo

Printing puzzle before search:
W E B M O
I L H L L
M L Z E L
M Y E K O
A O A B A

Word found!
Printing the search path:
0   0   0   0   5
0   0   1   4   0
0   0   0   2   3
0   0   0   0   0
0   0   0   0   0
```

Here `1` marks the `H`, `2` the `E`, and so on up to `5` for the final `O`. The grid contains more than one valid path for this word, and the program reports them together.

**Searching `bAnANa` in `puzzle2.txt`** finds a path that requires backtracking and shows overlapping positions as `246` and `35`.

**Searching `deTeR` in `puzzle3.txt`** finds two separate occurrences that start at different letters, and both are drawn in the output.

**Searching `color` in `puzzle3.txt`** prints `Word not found!`.

## How It Works

| Function | Purpose |
| --- | --- |
| `main()` | Provided by the assignment: reads the puzzle file, allocates the grid, prompts for the word, and calls the two functions below |
| `printPuzzle(char** arr)` | Prints the grid, with letters separated by spaces |
| `searchPuzzle(char** arr, char* word)` | Starts a search from every cell that matches the first letter, collects each distinct occurrence, merges them into one result grid, and prints the outcome |
| `aroundSearch(...)` | Recursive search that tries all eight neighbors for each next letter and records the path when the whole word is matched |
| `Upper(char)` | Converts a letter to uppercase manually, without `toupper` |
| `strLen(char*)` | Computes string length manually |
| `copyPath`, `isPathEmpty`, `printSinglePath` | Small helpers written for the multiple-path feature |

**Search strategy:** for each starting cell, `aroundSearch` matches the current letter, then recursively tries each of the 8 neighboring cells for the next letter, returning to try another direction if a branch fails. Each starting cell contributes at most one path, so a word can appear several times as long as the first letters are in different places.

**Assignment constraints:** the code uses pointer arithmetic (`*(*(arr + i) + j)`) instead of `[]` indexing when reading and writing the grid, and converts case by hand instead of using library functions.

## Repository Contents

| File | Description |
| --- | --- |
| `wordsearch3.c` | **Final version**, including multiple-occurrence support. Use this one |
| `wordsearch.c`, `wordsearch2.c`, `wordsearchtest.c` | Earlier development versions |
| `wordsearch (1).c` | The original starter code with `printPuzzle` and `searchPuzzle` left blank |
| `puzzle1.txt`, `puzzle2.txt`, `puzzle3.txt` | Sample puzzle grids |
| `Project01_Instructions.pdf` | The assignment description and sample runs |

The compiled executables in the folder (`wordsearch`, `wordsearch1`, `wordsearch2`, `wordsearch3`, `test`, `a.out`, `wordsearchtest`) were built on macOS (arm64) and won't run on Linux or Windows. Recompile from source on your own machine.

## Limitations

- The search word is stored in a 20-byte buffer provided by `main()`, so words of 20 or more characters will overflow it.
- The grid must be square and use single-character cells separated by spaces.
- Positions in the path are written as digits joined together, so for words longer than 9 letters a value like `12` is ambiguous (the 12th letter, or the 1st and 2nd).
- Each starting cell yields at most one path.
- `searchPuzzle` does not free the memory it allocates.
- The "not found" message prints with a trailing space (`Word not found! `), which differs slightly from the sample output in the assignment.

## Credits

Written for CSE 031. The source credits collaborators Angel Cisneros, Bryan Soto, and Adrian Bugarin.
