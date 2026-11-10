# Average Score Calculator (MIPS Assembly)

A MIPS assembly program that reads a list of assignment scores, sorts them in descending order, drops the lowest scores, and reports the average of what remains. It is written to run in the [MARS](https://dpetersanderson.github.io/) MIPS simulator.

This was built for **CSE 031, Project 02**. It implements selection sort and a recursive summation in assembly, and includes the bonus input-validation tasks. A reference implementation in C (`avgScore.c`) shows the intended behavior.

## What It Does

1. Asks for the number of assignments (1 to 25) and re-prompts until the input is valid.
2. Reads each score and stores it in the `orig` array.
3. Sorts a copy of the scores in descending order into the `sorted` array using **selection sort**.
4. Prints the original scores and the sorted scores.
5. Asks how many of the lowest scores to drop (0 to the number of scores) and re-prompts until the input is valid.
6. Computes the sum of the remaining scores with a **recursive** function, divides by the number of remaining scores, and prints the average rounded down.
7. If every score is dropped, prints `All scores dropped!` instead of dividing by zero.

## Example Runs

**Dropping the two lowest scores**

```
Enter the number of assignments (between 1 and 25): 5
Enter score: 2
Enter score: 22
Enter score: 11
Enter score: 7
Enter score: 19
Original scores: 2 22 11 7 19 
Sorted scores (in descending order): 22 19 11 7 2 
Enter the number of (lowest) scores to drop: 2
Average (rounded down) with dropped scores removed: 17
```

The sorted scores are `22 19 11 7 2`, dropping the lowest two leaves `22 + 19 + 11 = 52`, and `52 / 3` rounds down to `17`.

**Invalid input and dropping everything**

```
Enter the number of assignments (between 1 and 25): 0
Enter the number of assignments (between 1 and 25): -3
Enter the number of assignments (between 1 and 25): 26
Enter the number of assignments (between 1 and 25): 5
Enter score: 2
Enter score: 22
Enter score: 11
Enter score: 7
Enter score: 19
Original scores: 2 22 11 7 19 
Sorted scores (in descending order): 22 19 11 7 2 
Enter the number of (lowest) scores to drop: -1
Enter the number of (lowest) scores to drop: 6
Enter the number of (lowest) scores to drop: 5
All scores dropped!
```

Seven test cases, including these two, are recorded in `testRuns.txt`.

## Running the Program

You need [MARS](https://dpetersanderson.github.io/) and a Java runtime.

**In the MARS application**

1. Open MARS and choose **File → Open**, then select `avgScore.s`.
2. Choose **Run → Assemble** (or press `F3`).
3. Choose **Run → Go** (or press `F5`) and type your inputs in the console at the bottom.

**From the command line**

```bash
java -jar Mars.jar avgScore.s
```

When the program ends, MARS prints its own `-- program is finished running --` line.

## How It Works

| Routine | Purpose |
| --- | --- |
| `main` | Handles input and validation, calls the routines below, and prints the final result |
| `printArray` | Prints an array's elements on one line, each followed by a space, then a newline. Arguments: `$a0` = array address, `$a1` = length |
| `selSort` | Copies `orig` into `sorted`, then sorts `sorted` in descending order with selection sort. Argument: `$a0` = length |
| `calcSum` | Recursively returns the sum of `arr[0..len-1]`. Arguments: `$a0` = array address, `$a1` = length. Result in `$v0` |

**Selection sort:** for each position `i`, the inner loop finds the index of the largest remaining element, and that element is swapped into position `i`.

**Recursive sum:** `calcSum(arr, len)` returns `0` when `len <= 0`. Otherwise it saves `$ra`, `$a0`, and `$a1` on the stack, calls `calcSum(arr, len - 1)`, then restores its saved values and adds `arr[len - 1]` to the returned sum.

**Dropping the lowest scores:** because `sorted` is in descending order, the lowest scores are at the end. Dropping `d` of them means summing only the first `numScores - d` elements, so no elements are ever removed from the array.

**Register use in `main`:**

| Register | Holds |
| --- | --- |
| `$s0` | Number of scores |
| `$s1` | Address of `orig` |
| `$s2` | Address of `sorted` |
| `$a3` | Number of scores to drop |

**Data segment:** `orig` and `sorted` each reserve 100 bytes (25 words), plus the prompt and message strings.

## Project Files

| File | Description |
| --- | --- |
| `avgScore.s` | **Final solution** with all three functions and the bonus input handling. Use this one |
| `avgScore_1.s` | The starter template with the three functions left empty |
| `avgScore.c` | Reference implementation in C |
| `testRuns.txt` | Seven recorded test cases |
| `Project02_Instructions.pdf` | The assignment description and sample runs |

The `avgScore` executable and `avgScore.dSYM` folder are macOS build artifacts from compiling the C reference and aren't needed to run the assembly program.

## Limitations

- Input must be whole numbers. MARS reports an error if something else is typed when an integer is requested.
- The average uses integer division, which rounds toward zero. That matches "rounded down" for non-negative scores, but a negative average would round up toward zero.
- `All scores dropped!` is printed without a trailing newline, so the MARS termination message follows it on the next line.
- The array is fixed at 25 scores.

## Credits

Written for CSE 031, Project 02.
