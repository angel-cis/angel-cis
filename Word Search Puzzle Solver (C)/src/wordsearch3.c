#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Colaborator: Angel Cisneros, Bryan Soto, Adrian Bugarin 

// Declarations of the two functions you will implement
// Feel free to declare any helper functions or global variables

// Declare functions and global variables
void printPuzzle(char** arr);
void searchPuzzle(char** arr, char* word);
int bSize;

// Helper functions: 

// Chacracter convert function to uppercase
char Upper(char c) {
    if (c >= 'a' && c <= 'z') {
        return c - ('a' - 'A');
    }
    return c;
}

// Helper function to get string length
int strLen(char* str) {
    int len = 0;
    while (*(str + len) != '\0') {
        len++;
    }
    return len;
}

// Coordinate structure to track positions
typedef struct {
    int row;
    int col;
} 
Coordinate;

//Recursive function to search for a word in 8 directions
int aroundSearch (char** arr, char* word, int wordIdx, int row, int col, Coordinate* path, int pathLen, int** resultPath) {
    int wordLength = strLen(word);

    //Base case: found complete word
    if (wordIdx == wordLength) {
        //Fill result path
        for (int i = 0; i < pathLen; i++) {
            int r = (path + i)->row;
            int c = (path + i)->col;

            //Handle multiple occurrences at same position
            if (*(*(resultPath + r) + c) != 0) {
                int temp = *(*(resultPath + r) + c);
                int multiplier = 10;
                while (temp >= 10) {
                    multiplier = multiplier * 10;
                    temp = temp / 10;
                }
                *(*(resultPath + r) + c) = *(*(resultPath + r) + c) * 10 + (i + 1);
            } else {
                *(*(resultPath + r) + c) = i + 1;
            }
        }
        return 1;
    }

    // Check boundaries
    if (row < 0 || row >= bSize || col < 0 || col >= bSize) {
        return 0;
    }

    //Check if letters matches
    if (Upper(*(*(arr + row) + col)) != Upper(*(word + wordIdx))) {
        return 0;
    }

    //Add current position to path
    (path + pathLen)->row = row;
    (path + pathLen)->col = col;

    //Try 8 directions
    int directions[8][2] = {{0,1}, {1,1}, {1,0}, {1,-1}, {0,-1}, {-1,-1}, {-1,0}, {-1,1}};

    // Explore each direction    
    for (int d = 0; d < 8; d++) {
        int newrow = row + *(*(directions + d) + 0);
        int newcol = col + *(*(directions + d) + 1);

        // Recursive call
        if (aroundSearch(arr, word, wordIdx + 1, newrow, newcol, path, pathLen + 1, resultPath)) {
            return 1;
        }
    }

    return 0;
}

//Extra Credit: Fucntion to copy path arrays
void copyPath(int** dest, int** src) {
    for (int i = 0; i < bSize; i++) {
        for (int j = 0; j < bSize; j++) {
            *(*(dest + i) + j) = *(*(src + i) + j);
        }
    }
}

// Extra Credit: Function to check if path is empty
int isPathEmpty(int** path) {
    for (int i = 0; i < bSize; i ++) {
        for (int j = 0; j < bSize; j++) {
            if (*(*(path + i) + j) != 0) {
                return 0;
            }
        }
    }
    return 1;
}

//Extra Credit: Function to print a single path
void printSinglePath(int** path) {
    for (int i = 0; i < bSize; i++) {
        for (int j = 0; j < bSize; j++) {
            printf("%d ", *(*(path + i) + j));
        }
        printf("\n");
    }
}

// Main function, DO NOT MODIFY 	
int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <puzzle file name>\n", argv[0]);
        return 2;
    }
    int i, j;
    FILE *fptr;

    // Open file for reading puzzle
    fptr = fopen(argv[1], "r");
    if (fptr == NULL) {
        printf("Cannot Open Puzzle File!\n");
        return 0;
    }

    // Read the size of the puzzle block
    fscanf(fptr, "%d\n", &bSize);
    
    // Allocate space for the puzzle block and the word to be searched
    char **block = (char**)malloc(bSize * sizeof(char*));
    char *word = (char*)malloc(20 * sizeof(char));

    // Read puzzle block into 2D arrays
    for(i = 0; i < bSize; i++) {
        *(block + i) = (char*)malloc(bSize * sizeof(char));
        for (j = 0; j < bSize - 1; ++j) {
            fscanf(fptr, "%c ", *(block + i) + j);            
        }
        fscanf(fptr, "%c \n", *(block + i) + j);
    }
    fclose(fptr);

    printf("Enter the word to search: ");
    scanf("%s", word);
    
    // Print out original puzzle grid
    printf("\nPrinting puzzle before search:\n");
    printPuzzle(block);
    
    // Call searchPuzzle to the word in the puzzle
    searchPuzzle(block, word);
    
    return 0;
}

void printPuzzle(char** arr) {
	// This function will print out the complete puzzle grid (arr). 
    // It must produce the output in the SAME format as the samples 
    // in the instructions.
    // Your implementation here...
    int i, j;
    for (i = 0; i < bSize; i++) {
        for (j = 0; j < bSize; j++) {
            printf("%c ", *(*(arr + i) + j));
        }
        printf("\n");
    }

}

void searchPuzzle(char** arr, char* word) {
    // This function checks if arr contains the search word. If the 
    // word appears in arr, it will print out a message and the path 
    // as shown in the sample runs. If not found, it will print a 
    // different message as shown in the sample runs.
    // Your implementation here...

    int wordLength = strLen(word); // Get length of the word to search
    int i, j, k, l; // i and j for rows and columns, k is for paths, l for columns in the path
    int found = 0; //Number of found paths
    int*** allPaths = (int***)malloc(bSize * bSize * sizeof(int**)); //Stores all found paths

    // Non Extra Credit Version: 
    // int** resultPath = (int**)malloc(bSize * sizeof(int*));
    // for (i = 0; i < bSize; i++) {
    //     *(resultPath + i) = (int*)malloc(bSize * sizeof(int));
    //     for (j = 0; j < bSize; j++) {
    //         *(*(resultPath + i) + j) = 0;
    //     }
    // }

    //Alocate the coordinate path array
    Coordinate* path = (Coordinate*)malloc(wordLength * sizeof(Coordinate));

    //Extra Credit Version:
    //Starting from each position in the puzzle
    for (i = 0; i < bSize; i++) {
        for (j = 0; j < bSize; j++) {
            //Check if the first character matches
            if (Upper(*(*(arr + i) + j)) == Upper(*(word + 0))) {
                // Allocate a temporary result path for this search
                int** tempPath = (int**)malloc(bSize * sizeof(int*));
                //Initialize tempPath to 0
                for (k = 0; k < bSize; k++) {
                    *(tempPath + k) = (int*)malloc(bSize * sizeof(int));
                    for (l = 0; l < bSize; l++) {
                        *(*(tempPath + k) + l) = 0;
                    }
                }

                //Try to find word from a position
                if (aroundSearch(arr, word, 0, i, j, path, 0, tempPath)) {
                    //If found, copy to resultPath and set found flag
                    int isDuplicate = 0;
                    for (k = 0; k < found; k++) {
                        if (*(*(*(allPaths + k) + i) + j) == 1) {
                            isDuplicate = 1;
                            break;
                        }
                    }

                    //If not duplicate, store the path
                    if (!isDuplicate) {
                        //Copy to resultPath
                        *(allPaths + found) = tempPath;
                        found++;
                    } else {
                        //Free duplicate path
                        for (k = 0; k < bSize; k++) {
                            free (*(tempPath + k));
                        }
                        free (tempPath);
                    }
                } else {
                    //Free unused path
                    for (k = 0; k < bSize; k++) {
                        free (*(tempPath + k));
                    }
                    free (tempPath);
                }
            }
        }
    }

    //Combine all found paths into a single resultPath
    int** resultPath = (int**)malloc(bSize * sizeof(int*));
    for (i = 0; i < bSize; i++) {
        *(resultPath + i) = (int*)calloc(bSize, sizeof(int));
    }

    //Merge all paths
    for (k = 0; k < found; k++) {
        for (i = 0; i < bSize; i++) {
            for (j = 0; j < bSize; j++) {
                int cellValue = *(*(*(allPaths + k) + i ) + j);
                if (cellValue != 0) {
                    if (*(*(resultPath + i) + j) != 0) {
                        int existing = *(*(resultPath + i) + j);
                        *(*(resultPath + i) + j) = existing * 10 + cellValue;
                    } else {
                        *(*(resultPath + i) + j) = cellValue;
                    }
                }
            }
        }
    }

    // Print results
    if(found > 0) {
        printf ("\nWord found!\n");
        printf("Printing the search path:\n");

        //Print the search path
        for (i = 0; i < bSize; i++) {
            for (j = 0; j < bSize; j++) {
                printf("%d", *(*(resultPath + i) + j));
                if (j < bSize - 1) {
                    printf("\t");
                }
            }
            printf("\n");
            }
    } else {
        printf("\nWord not found! \n");
    }

    // Prints 1 path only
    // if (found) {
    //     printf("\nWord found!\n");
    //     printf("Printing the search path:\n");
    //     for (i = 0; i < bSize; i++) {
    //         for (j = 0; j < bSize; j++) {
    //             printf("%d ", *(*(resultPath + i) + j));
    //         }
    //         printf("\n");
    //     }
    // } else {
    //     printf("\nWord not found!\n");
    // }
    
}
