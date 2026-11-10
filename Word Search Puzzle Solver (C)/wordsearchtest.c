#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printPuzzle(char** arr);
void searchPuzzle(char** arr, char* word);
int bSize;

char toUpper(char c) {
    if (c >= 'a' && c <= 'z') {
        return c - ('a' - 'A');
    }
    return c;
}

int strLen(char* str) {
    int len = 0;
    while (*(str + len) != '\0') {
        len++;
    }
    return len;
}

typedef struct {
    int row;
    int col;
} Coordinate;

int aroundSearch(char** arr, char* word, int wordIdx, int row, int col, 
                 Coordinate* path, int pathLen, int** resultPath) {
    int wordLength = strLen(word);
    
    if (wordIdx == wordLength) {
        // Fill result path
        for (int i = 0; i < pathLen; i++) {
            int r = (path + i)->row;
            int c = (path + i)->col;
            
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
    
    if (row < 0 || row >= bSize || col < 0 || col >= bSize) {
        return 0;
    }
    
    if (toUpper(*(*(arr + row) + col)) != toUpper(*(word + wordIdx))) {
        return 0;
    }
    
    (path + pathLen)->row = row;
    (path + pathLen)->col = col;
    
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

void copyPath(int** dest, int** src) {
    for (int i = 0; i < bSize; i++) {
        for (int j = 0; j < bSize; j++) {
            *(*(dest + i) + j) = *(*(src + i) + j);
        }
    }
}

int isPathEmpty(int** path) {
    for (int i = 0; i < bSize; i++) {
        for (int j = 0; j < bSize; j++) {
            if (*(*(path + i) + j) != 0) {
                return 0;
            }
        }
    }
    return 1;
}

void printSinglePath(int** path) {
    for (int i = 0; i < bSize; i++) {
        for (int j = 0; j < bSize; j++) {
            printf("%d ", *(*(path + i) + j));
        }
        printf("\n");
    }
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <puzzle file name>\n", argv[0]);
        return 2;
    }
    int i, j;
    FILE *fptr;

    fptr = fopen(argv[1], "r");
    if (fptr == NULL) {
        printf("Cannot Open Puzzle File!\n");
        return 0;
    }

    fscanf(fptr, "%d\n", &bSize);
    
    char **block = (char**)malloc(bSize * sizeof(char*));
    char *word = (char*)malloc(20 * sizeof(char));

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
    
    printf("\nPrinting puzzle before search:\n");
    printPuzzle(block);
    
    searchPuzzle(block, word);
    
    return 0;
}

void printPuzzle(char** arr) {
    int i, j;
    for (i = 0; i < bSize; i++) {
        for (j = 0; j < bSize; j++) {
            printf("%c ", *(*(arr + i) + j));
        }
        printf("\n");
    }
}

void searchPuzzle(char** arr, char* word) {
    int wordLength = strLen(word);
    int i, j, k, l;
    
    int*** allPaths = (int***)malloc(100 * sizeof(int**));
    int pathCount = 0;
    
    // Allocate coordinate path array
    Coordinate* path = (Coordinate*)malloc(wordLength * sizeof(Coordinate));
    
    // Try starting from each position in the puzzle
    for (i = 0; i < bSize; i++) {
        for (j = 0; j < bSize; j++) {
            // Check if first character matches
            if (toUpper(*(*(arr + i) + j)) == toUpper(*(word + 0))) {
                // Allocate a temporary result path for this search
                int** tempPath = (int**)malloc(bSize * sizeof(int*));
                for (k = 0; k < bSize; k++) {
                    *(tempPath + k) = (int*)malloc(bSize * sizeof(int));
                    for (l = 0; l < bSize; l++) {
                        *(*(tempPath + k) + l) = 0;
                    }
                }
                
                // Try to find word starting from this position
                if (aroundSearch(arr, word, 0, i, j, path, 0, tempPath)) {
                    // Check if this is a new occurrence (different starting position)
                    int isDuplicate = 0;
                    for (k = 0; k < pathCount; k++) {
                        // Check if first position matches any existing path
                        if (*(*(*(allPaths + k) + i) + j) == 1) {
                            isDuplicate = 1;
                            break;
                        }
                    }
                    
                    if (!isDuplicate) {
                        // Store this path
                        *(allPaths + pathCount) = tempPath;
                        pathCount++;
                    } else {
                        // Free the duplicate path
                        for (k = 0; k < bSize; k++) {
                            free(*(tempPath + k));
                        }
                        free(tempPath);
                    }
                } else {
                    // Free the unused path
                    for (k = 0; k < bSize; k++) {
                        free(*(tempPath + k));
                    }
                    free(tempPath);
                }
            }
        }
    }
    
    if (pathCount > 0) {
        printf("\nWord found!\n");
        
        // If multiple paths with different starting positions, print them separately
        if (pathCount > 1) {
            for (i = 0; i < pathCount; i++) {
                printf("Printing the search path:\n");
                printSinglePath(*(allPaths + i));
                if (i < pathCount - 1) {
                    printf("\n");
                }
            }
        } else {
            // Single path
            printf("Printing the search path:\n");
            printSinglePath(*(allPaths + 0));
        }
    } else {
        printf("\nWord not found!\n");
    }
    
    // Free allocated memory
    for (i = 0; i < pathCount; i++) {
        for (j = 0; j < bSize; j++) {
            free(*(*(allPaths + i) + j));
        }
        free(*(allPaths + i));
    }
    free(allPaths);
    free(path);
}