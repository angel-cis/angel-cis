#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// For character handling functions
#include <ctype.h> 

// Declarations of the two functions you will implement
// Feel free to declare any helper functions or global variables
void printPuzzle(char** arr);
void searchPuzzle(char** arr, char* word);
int bSize;
int found = 0;

// Helper function to convert character to uppercase
char toUpper(char c) {
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
} Coordinate;

// Recursive function to search around a position
int aroundSearch(char** arr, char* word, int wordIdx, int row, int col, Coordinate* path, int pathLen, int** resultPath) {
    int wordLength = strLen(word);
    
    // Base case: found complete word
    if (wordIdx == wordLength) {
        // Fill result path
        for (int i = 0; i < pathLen; i++) {
            int r = (path + i)->row;
            int c = (path + i)->col;
            
            // Handle multiple occurrences at same position
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
    
    // Check bounds
    if (row < 0 || row >= bSize || col < 0 || col >= bSize) {
        return 0;
    }
    
    // Check if current letters matches
    if (toUpper(*(*(arr + row) + col)) != toUpper(*(word + wordIdx))) {
        return 0;
    }
    
    // Add current position to path
    (path + pathLen)->row = row;
    (path + pathLen)->col = col;
    
    // Try all 8 directions
    int directions[8][2] = {{0,1}, {1,1}, {1,0}, {1,-1}, {0,-1}, {-1,-1}, {-1,0}, {-1,1}};
    
    for (int i = 0; i < 8; i++) {
        int newRow = row + *(*(directions + i) + 0);
        int newCol = col + *(*(directions + i) + 1);
        
        if (aroundSearch(arr, word, wordIdx + 1, newRow, newCol, path, pathLen + 1, resultPath)) {
            return 1;
        }
    }
    
    return 0;
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
    int wordLength = strLen(word);
    int i, j, k, l;
    int found = 0;
    
    // Allocate result path array (initialized to 0)
    int** resultPath = (int**)malloc(bSize * sizeof(int*));
    for (i = 0; i < bSize; i++) {
        *(resultPath + i) = (int*)malloc(bSize * sizeof(int));
        for (j = 0; j < bSize; j++) {
            *(*(resultPath + i) + j) = 0;
        }
    }
    
    // Allocate coordinate path array
    Coordinate* path = (Coordinate*)malloc(wordLength * sizeof(Coordinate));
    
    // Try starting from each position in the puzzle
    for (i = 0; i < bSize && !found; i++) {
        for (j = 0; j < bSize && !found; j++) {
            // Check if first character matches
            if (toUpper(*(*(arr + i) + j)) == toUpper(*(word + 0))) {
                // Try to find word starting from this position
                if (aroundSearch(arr, word, 0, i, j, path, 0, resultPath)) {
                    found = 1;
                }
            }
        }
    }
    
    if (found) {
        printf("\nWord found!\n");
        printf("Printing the search path:\n");
        for (i = 0; i < bSize; i++) {
            for (j = 0; j < bSize; j++) {
                printf("%d ", *(*(resultPath + i) + j));
            }
            printf("\n");
        }
    } else {
        printf("\nWord not found!\n");
    }
    
    // Free allocated memory
    for (i = 0; i < bSize; i++) {
        free(*(resultPath + i));
    }
    free(resultPath);
    free(path);
}