/// Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structures
struct Match {
    char firstTeam[80];
    char secondTeam[80];
    int firstScore;
    int secondScore;
};

typedef struct Match MATCH;

struct Team {
    char teamName[80];
    int win;
    int draw;
    int loss;
    int points;
    int average;
    int goals; // score
};

typedef struct Team TEAM;

// Function Prototypes
void LoadScoreTable(FILE *, MATCH *);
int getNumberOfLines(FILE *);
TEAM * CreateLeagueTable(MATCH *, int);
void OrderLeagueTable(TEAM *, int);
void DisplayLeagueTable(TEAM *, int);

int main() {
    FILE * matchRecords;
    MATCH * scoreTable;
    TEAM * leagueTable;

    char tableFileName[] = "scoretable.txt";

    matchRecords = fopen(tableFileName, "r");

    // Check if score table file is exist
    if (!matchRecords) {
        printf("Score table file does NOT exist!\n");
        exit(EXIT_FAILURE);
    }

    // Get the number of matches played
    int numOfLines = getNumberOfLines(matchRecords);

    // Create an array of structures
    scoreTable = (MATCH *) malloc(sizeof(MATCH) * numOfLines);

    // Check if memory is allocated
    if (!scoreTable) {
        printf("Something went wrong while trying to allocate memory\n");
        exit(EXIT_FAILURE);
    }

    // Load the score table into an array of structure
    LoadScoreTable(matchRecords, scoreTable);

    // Create the league table
    leagueTable = CreateLeagueTable(scoreTable, numOfLines);

    OrderLeagueTable(leagueTable, numOfLines);
    
    DisplayLeagueTable(leagueTable, numOfLines);

    // Close the files that openeed
    fclose(matchRecords);

    // TODO: Set pointers free
    free(scoreTable);
    free(leagueTable);

    return 0;
}

int getNumberOfLines(FILE * fileStream) {
    char el;
    int numOfLines = 1;

    // Count the rows/newlines
    while ( (el = fgetc(fileStream)) != EOF ) {
        if (el == '\n') {
            numOfLines++;
        }
    }

    // bring the cursor to the beginning of the file again
    rewind(fileStream);

    // Return the number of rows
    return numOfLines;
}

void LoadScoreTable(FILE * matchRecords, MATCH * scoreTable) {
    int i = 0;
    while (!feof(matchRecords)) {
        fscanf(matchRecords, "%s %s %d %d\n", scoreTable[i].firstTeam, scoreTable[i].secondTeam, &scoreTable[i].firstScore, &scoreTable[i].secondScore);
        i++;
    }
}

TEAM * CreateLeagueTable(MATCH * scoreTable, int size) {
    TEAM * leagueTable;
    int teamCount = size * 2;

    // Dynamic array creation
    leagueTable = (TEAM *) malloc(teamCount * sizeof(TEAM));

    // Check if memory is allocated
    if (!leagueTable) {
        printf("Something went wrong while trying to allocate memory\n");
        exit(EXIT_FAILURE);
    }

    // Set default values of the team stats
    for (int i = 0; i < teamCount; i++) {
        leagueTable[i].win = 0;
        leagueTable[i].draw = 0;
        leagueTable[i].loss = 0;
        leagueTable[i].points = 0;
        leagueTable[i].average = 0;
        leagueTable[i].goals = 0;
    }

    //TEAM tempFirst, tempSecond;
    for (int i = 0; i < size; i++) {
        // Put each team to the leage table
        strcpy(leagueTable[i].teamName, scoreTable[i].firstTeam);
        strcpy(leagueTable[i + size].teamName, scoreTable[i].secondTeam);

        // Copy the goals from score table to the league table
        leagueTable[i].goals  = scoreTable[i].firstScore;
        leagueTable[i + size].goals = scoreTable[i].secondScore;

        // Compare the goals made and determine the win - loss - draw situations
        if (leagueTable[i].goals > leagueTable[i + size].goals) {
            leagueTable[i].win++;
            leagueTable[i + size].loss++;
        } else if (leagueTable[i].goals < leagueTable[i + size].goals) {
            leagueTable[i].loss++;
            leagueTable[i + size].win++;
        } else {
            leagueTable[i].draw++;
            leagueTable[i + size].draw++;
        }

        // Calculation of the points
        leagueTable[i].points  = (leagueTable[i].win * 3) + (leagueTable[i].draw * 1);
        leagueTable[i + size].points  = (leagueTable[i + size].win * 3) + (leagueTable[i + size].draw * 1);

        // Calculate the average
        leagueTable[i].average  = scoreTable[i].firstScore - scoreTable[i].secondScore;
        leagueTable[i + size].average = scoreTable[i].secondScore - scoreTable[i].firstScore;
    }

    return leagueTable;
}

void OrderLeagueTable(TEAM * leagueTable, int size) {
    size *= 2;

    TEAM tempTeam;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {

            // Sort depending on the points
            if (leagueTable[j].points < leagueTable[i].points) {
                tempTeam = leagueTable[i];
                leagueTable[i] = leagueTable[j];
                leagueTable[j] = tempTeam;
            }

            // If the points are the same then compare the averages
            if (leagueTable[j].points == leagueTable[i].points) {
                if (leagueTable[j].average < leagueTable[i].average) {
                    tempTeam = leagueTable[i];
                    leagueTable[i] = leagueTable[j];
                    leagueTable[j] = tempTeam;
                }

                // If the averages are the same then compare the goals
                if (leagueTable[j].average == leagueTable[i].average) {
                    if (leagueTable[j].goals < leagueTable[i].goals) {
                        tempTeam = leagueTable[i];
                        leagueTable[i] = leagueTable[j];
                        leagueTable[j] = tempTeam;
                    }
                }

            }
    
        }
    }
}

void DisplayLeagueTable(TEAM * leagueTable, int size) {
    size *= 2;

    printf("  Teams            Win    Draw    Loss    Points    Average    Goals\n");
    printf("  ----------------------------------------------------------------------\n");
    for (int i = 0; i < size; i++) {
        printf("  %-14s %3d %4d %7d %7d %9d %10d\n", leagueTable[i].teamName, leagueTable[i].win, leagueTable[i].draw, leagueTable[i].loss, leagueTable[i].points, leagueTable[i].average, leagueTable[i].goals);
    }
}