#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAP_MIN_WIDTH 2
#define MAP_MAX_WIDTH 32
#define MAP_MIN_HEIGHT 2
#define MAP_MAX_HEIGHT 32
#define MAX_TREASURES_RATIO 0.25

void initializeMap(int ***map, int width, int height);
int isCodeValid(const char *code);
void generateTreasures(int **map, int width, int height, int numTreasures, char **treasureCodes);
void freeMap(int **map, int height);
void digAtSpot(int **map, int width, int height, char **treasureCodes);
void enterCheatMode(int **map, int width, int height, char **treasureCodes);

int main() {
    int width, height, numTreasures;
    int **map;
    char **treasureCodes;
    int choice;
    int i,j;

    printf("Enter map width: ");
    scanf("%d", &width);
    if (width < MAP_MIN_WIDTH || width > MAP_MAX_WIDTH) {
        printf("Map width out of range. Please enter a width between %d and %d.\n", MAP_MIN_WIDTH, MAP_MAX_WIDTH);
        return 1;
    }

    printf("Enter map height: ");
    scanf("%d", &height);
    if (height < MAP_MIN_HEIGHT || height > MAP_MAX_HEIGHT) {
        printf("Map height out of range. Please enter a height between %d and %d.\n", MAP_MIN_HEIGHT, MAP_MAX_HEIGHT);
        return 1;
    }

    printf("Enter number of treasures to add: ");
    scanf("%d", &numTreasures);
    if (numTreasures < 1 || numTreasures > width * height * MAX_TREASURES_RATIO) {
        printf("Invalid number of treasures. Please enter a number between 1 and %d.\n", (int)(width * height * MAX_TREASURES_RATIO));
        return 1;
    }

    initializeMap(&map, width, height);

    treasureCodes = malloc(numTreasures * sizeof(char *));
    for (i = 0; i < numTreasures; i++) {
        treasureCodes[i] = malloc(5 * sizeof(char));
    }

    generateTreasures(map, width, height, numTreasures, treasureCodes);

    do {
        printf("\n1 - Dig at a spot  | 2 - Exit Game      | 3 - Cheat!\n");
        printf("-> ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Map:\n");
                for (i = 0; i < height; i++) {
                    printf("%2d| ", i);
                    for (j = 0; j < width; j++) {
                        printf(" ");
                    }
                    printf("\n");
                }
                digAtSpot(map, width, height, treasureCodes);
                break;
            case 2:
                printf("Exiting the game.\n");
                break;
            case 3:
                enterCheatMode(map, width, height, treasureCodes);
                break;
            default:
                printf("Invalid choice. Please choose a valid option.\n");
        }
    } while (choice != 2);

    freeMap(map, height);

    for (i = 0; i < numTreasures; i++) {
        free(treasureCodes[i]);
    }
    free(treasureCodes);

    return 0;
}

void initializeMap(int ***map, int width, int height) {
    int i, j;
    *map = malloc(height * sizeof(int *));
    for (i = 0; i < height; i++) {
        (*map)[i] = malloc(width * sizeof(int));
        for (j = 0; j < width; j++) {
            (*map)[i][j] = 0;
        }
    }
}

int isCodeValid(const char *code) {
    int i;
    if (strlen(code) != 4 || code[0] != 'T')
        return 0;
    for (i = 1; i < 4; i++) {
        if (code[i] < '0' || code[i] > '9')
            return 0;
    }
    return 1;
}

void generateTreasures(int **map, int width, int height, int numTreasures, char **treasureCodes) {
    int i, j;
    srand(time(0));
    
    printf("Map:\n");
    printf("   ");
    for (j = 0; j < width; j++) {
        printf("%d", j / 10); 
    }
    printf("\n");
    printf("   ");
    for (j = 0; j < width; j++) {
        printf("%d", j % 10); 
    }
    printf("\n");
    printf("   ");
    for (j = 0; j < width; j++) {
        printf("-");
    }
    printf("\n");
    for (i = 0; i < height; i++) {
        printf("%2d| ", i); 
        for (j = 0; j < width; j++) {
            printf(" "); 
        }
        printf
("\n");
    }

    for (i = 0; i < numTreasures; i++) {
        int codeExists; 
        do {
            int x = rand() % width;
            int y = rand() % height;
            codeExists = 0; 
            if (map[y][x] == 0) {
                map[y][x] = i + 1;
                printf("Enter treasure #%d code (e.g. T001): ", i + 1);
                scanf("%s", treasureCodes[i]);
                if (!isCodeValid(treasureCodes[i])) {
                    printf("Treasure codes must be a 'T' followed by 3 digits\n");
                } else {
                    for (j = 0; j < i; j++) {
                        if (strcmp(treasureCodes[i], treasureCodes[j]) == 0) {
                            codeExists = 1;
                            printf("Treasure code already in use.\n");
                            break;
                        }
                    }
                }
            }
            else {
                codeExists = 1; 
            }
        } while (!isCodeValid(treasureCodes[i]) || codeExists);
    }
}

void freeMap(int **map, int height) {
    int i;
    for (i = 0; i < height; i++) {
        free(map[i]);
    }
    free(map);
}

void digAtSpot(int **map, int width, int height, char **treasureCodes) {
    int digX, digY;
    int i,j;

    printf("Where would you like to dig?\n");
    
    printf("x = ");
    scanf("%d", &digX);
    printf("y = ");
    scanf("%d", &digY);

    
    if (digX < 0 || digX >= width || digY < 0 || digY >= height) {
        printf("Invalid coordinates. Please enter valid coordinates within the map.\n");
        return;
    }

    
    if (map[digY][digX] == 0) {
        printf("You found nothing there!\n");
        map[digY][digX] = -1; 
    } else if (map[digY][digX] == -1) {
        printf("You already dug there and found nothing!\n");
    } else {
        printf("Congratulations, you found a treasure! (code: %s)\n", treasureCodes[map[digY][digX] - 1]);
        map[digY][digX] = -1; 
    }

    printf("Map:\n");
    printf("   ");
    for (j = 0; j < width; j++) {
        printf("%d", j / 10); 
    }
    printf("\n");
    printf("   ");
    for (j = 0; j < width; j++) {
        printf("%d", j % 10); 
    }
    printf("\n");
    printf("   ");
    for (j = 0; j < width; j++) {
        printf("-");
    }
    printf("\n");
    for (i = 0; i < height; i++) {
        printf("%2d| ", i); 
        for (j = 0; j < width; j++) {
            if (map[i][j] == 0) {
                printf(" ");  
            } else if (map[i][j] == -1) {
                printf("x");
            } else {
                printf("$");
            }
        }
        printf("\n");
    }
}


void enterCheatMode(int **map, int width, int height, char **treasureCodes) {
    int i,j;
    printf("Map (Cheat mode):\n");
    printf("   ");
    for (j = 0; j < width; j++) {
        printf("%2d", j);
    }
    printf("\n");
    printf("   ");
    for (j = 0; j < width; j++) {
        printf("--");
    }
    printf("\n");
    for (i = 0; i < height; i++) {
        printf("%2d| ", i);
        for (j = 0; j < width; j++) {
            if (map[i][j] == 0) {
                printf("  ");  
            } else {
                printf(" !"); 
            }
        }
        printf("\n");
    }
    printf("\nWhere would you like to dig?\n");
}