// Author: Joseph Chung
// z5308483
// Date: 
// UNSW Freefall assignment.
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// #defines
#define SIZE 15
#define EMPTY 0
#define STONE 1
#define MARCHING 2
#define MOVE 1
#define LASER 2
#define DOWN 3
#define FLIP 4

// prototypes
void printMap(int map[SIZE][SIZE], int playerX);
void blocks(int value, int stone_row, int stone_col, int stone_num, int map[SIZE][SIZE]);
void playerCommand(int playerX, int map[SIZE][SIZE]);
int gameWon(int playerX, int map[SIZE][SIZE]);
int gameLost(int playerX, int map[SIZE][SIZE]);
void tntExplosion(int playerX, int i, int map[SIZE][SIZE]);


int main (void) {
    // This line creates our 2D array called "map" and sets all of the blocks 
    // in the map to EMPTY.
    int map[SIZE][SIZE] = {EMPTY};
    
    // This line creates out playerX variable. The player starts in the middle 
    // of the map, at position 7.
    int playerX = SIZE / 2;
    
    //Ask for how many lines
    printf("How many lines of stone? ");
    
    // Scan in the number of lines of blocks.
    int stone_lines;
    scanf("%d", &stone_lines);   
    printf("Enter lines of stone:\n");
    
    // Scan in the lines of blocks.
    int stone_row;
    int stone_col;
    int stone_num;
    int value;
    
    int lines = 0;
    while (lines < stone_lines) {
        scanf("%d %d %d %d", &stone_row, &stone_col, &stone_num, &value);
        // checks that the entered inputs are within the grid
        if (0 < stone_num && (stone_num + stone_col) <= SIZE) {
            if (stone_row >= 0 && stone_col >= 0) {
                if (stone_row <= SIZE && stone_col <= SIZE) {
                    blocks(value, stone_row, stone_col, stone_num, map);        
                }
            }
        }                       
        lines++;    
    }
    // if entered inputs are not in grid, print all those within grid only
    printMap(map, playerX);
    
    // Scan in commands until EOF.
    playerCommand(playerX, map);
    
   
    return 0;
}        

// Checks if game is won each time after laser
int gameWon(int playerX, int map[SIZE][SIZE]) {
    int i = 0;
    int num = 0;
    int emptycounter = 0;
    while (i < SIZE) {
        int j = 0;
        while (j < SIZE) {
            if (map[i][j] == EMPTY) {
                // everytime there is an empty value, it adds one to the count
                emptycounter++;
                // 15 x 15 = 225, meaning if whole grid 0s the game won
                if (emptycounter == 225) {
                    printf("Game Won! \n");
                    // change return value to 1 to end the game
                    num = 1;
                }         
            }    
            j++;
        }
        i++;
    }
    return num;   
}

// Checks if game is lost each time before shift down
int gameLost(int playerX, int map[SIZE][SIZE]) {
    int i = 0;
    int num = 0;
    int gridcounter = 0;
    while (i < SIZE) {
        int j = 0;
        while (j < SIZE) {
            if (gridcounter == 0) {
                if (map[SIZE - 1][j] == STONE) {
                    printMap(map, playerX);
                    printf("Game Lost! \n");
                    num = 1;
                    gridcounter++;
                }
            }    
            j++;
        }
        i++;
    }
    return num;
}                    

// Explodes the TNT creating all STONES within radius to EMPTY
void tntExplosion(int playerX, int i, int map[SIZE][SIZE]) {

    int radius = map[i][playerX];
    map[i][playerX] = 0;
    int xdiff;
    int ydiff;
    
    int row = 0;
    while (row < SIZE) {
        int col = 0;
        while (col < SIZE) {
            // set xdiff and ydiff as difference in x and y 
            // i.e. (x1 - x2) and (y1 - y2)
            xdiff = col - playerX;
            // square the differences by multiplying them with themselves
            xdiff *= xdiff;
            ydiff = row - i;
            ydiff *= ydiff;
            
            // initialise sum of the differences
            int sum = xdiff + ydiff;
            // use math function to sqrt the sum
            int distance = sqrt(sum);
            // if the distance from centre is less than radius change to EMPTY
            if (distance < radius) {
                map[row][col] = EMPTY;
            }
            col++;
        }        
        row++;
    }            
}    
    
    
                         
// Prints out stones/marching blocks into the map
// The function is in two lines because of line length limit
void blocks(int value, int stone_row, int stone_col, int stone_num, 
int map[SIZE][SIZE]) {
    int countCol = stone_col;
    if (0 <= (countCol + stone_num - 1)) {
        if ((countCol + stone_num - 1) < SIZE) {
            while (countCol < (stone_col + stone_num)) {
                map[stone_row][countCol] = value;
                countCol++;
            }
        }
    }        
}

// Prints out the contents of the map array. Then print out the player line
// depends on the playerX variable.
void printMap(int map[SIZE][SIZE], int playerX) {
    int i;
    int j; 
    i = 0;
    while (i < SIZE) {
        j = 0;
        while (j < SIZE) {
            printf("%d ", map[i][j]);
            j++;
        }
        printf("\n");
        i++;
    } 
    // Print the player line.
    i = 0;
    while (i < playerX) {
        printf("  ");
        i++;
    }   
    printf("P\n");
}

// Scans input for commands for playerX            
void playerCommand(int playerX, int map[SIZE][SIZE]) {          
    int command;
    int direction;
    int i;
    int j;
    int reverserow;
    int reversecol;
    int flipcounter = 0;
    int marchingcounter = 0;
    
    // scanf command/input  
    while (scanf("%d", &command) != EOF) {
        // if command is 1, then scanf again for direction    
        if (command == MOVE) {
            scanf(" %d", &direction);
            if (direction == 1) {
                playerX = playerX + 1;
            } else if (direction == -1) {
                playerX = playerX - 1;
            }
            printMap(map, playerX);
        // if command is 2, then fire laser                            
        } else if (command == LASER) {
            i = SIZE - 1;
            int countstone = 0;
            // destroy up to 3 blocks
            while (i >= 0 && countstone < 3) {
                if (map[i][playerX] == STONE) {
                    map[i][playerX] = EMPTY;
                    countstone++;
                // if laser hits value between 3-9 then go to tntExplosion    
                } else if (map[i][playerX] > 2 && map[i][playerX] < 10) {
                    countstone = 3;
                    
                    tntExplosion(playerX, i, map);
                }
                i--;    
            }
            printMap(map, playerX);
            // Calls gameWon to check whether game is won
            if (gameWon(playerX, map) == 1) {
                return;
            }
        // if command is 3, then shift grid down            
        } else if (command == DOWN) {
            marchingcounter++;
            // first call gameLost to check whether game is lost
            if (gameLost(playerX, map) == 1) {
                return;
            }
            // go through array   
            i = SIZE - 1;
            int row = SIZE - 1;
            while (i >= 0 && row >= 0) {
                j = SIZE - 1;
                int col = 0;
                while (j >= 0 && col < SIZE) {
                    // if there is a value then shift it down
                    if (map[i][j] == STONE) {
                        // change value in original position to 0 
                        map[i][j] = EMPTY;
                        // change bottom row to that value
                        map[i + 1][j] = STONE;
                    } else if (map[i][j] == MARCHING) {
                        
                        if (marchingcounter == 1) {
                            map[i][j] = EMPTY;
                            map[i + 1][j] = MARCHING;
                        } else if (marchingcounter == 2) {
                            if (map[i][SIZE - 1] != MARCHING) {
                                map[i][j] = EMPTY;
                                map[i][j + 1] = MARCHING;
                            }    
                        } else if (marchingcounter == 3) {
                            map[i][j] = EMPTY;
                            map[i + 1][j] = MARCHING;
                        } else if (marchingcounter == 4) {
                            if (map[i][0] != MARCHING) {
                                map[i][j] = EMPTY;
                                map[i][j - 1] = MARCHING;
                                //marchingcounter = 0;
                            }                 
                        }
                    }                              
                    j--;
                    col++;
                }
                i--;
                row--;    
            }
            printMap(map, playerX);    
        // if command is 4, then flip the grid       
        } else if (command == FLIP) {
            // first initalise the whole reverse grid as 0
            int reverse[SIZE][SIZE] = {0};
            i = 0;
            reverserow = SIZE - 1;
            // checking that the flip is only done once
            if (flipcounter == 0) {
                // go through original 2D while going through reverse array 
                // backwards simultaneously 
                while (i <= SIZE - 1 && reverserow >= 0) {
                    j = 0;
                    reversecol = 0;
                    while (j < SIZE - 1 && reversecol < SIZE - 1) {
                        // fill reverse array with map array 
                        reverse[reverserow][reversecol] = map[i][j];    
                        reversecol++;
                        j++;
                    }   
                    reverserow--;
                    i++;
                }
                // change original array with 0s
                i = 0;
                while (i < SIZE) {
                    j = 0;
                    while (j < SIZE) {
                        map[i][j] = 0;    
                        j++;
                    }   
                    i++;
                }
                // copy reverse array into original array
                i = 0;
                reverserow = 0;
                // go through reverse array and original array together
                // in the same direction
                while (i <= SIZE - 1 && reverserow <= SIZE - 1) {
                    j = 0;
                    reversecol = 0;
                    while (j < SIZE - 1 && reversecol < SIZE - 1) {
                        map[i][j] = reverse[reverserow][reversecol];    
                        reversecol++;
                        j++;
                    }   
                    reverserow++;
                    i++;
                }
                flipcounter++;    
            }
            // print original array                          
            printMap(map, playerX);
        } 
    }             
}

