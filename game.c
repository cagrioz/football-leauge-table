// Doğu Çağrı Özarpacı - 2471589

/// Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Constants
#define WIN_POINTS 3
#define DRAW_POINTS 1

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
int SearchTeam(TEAM *, int, char *);
void capitalize(char *);
void DisplayMatchRecords(FILE *);

int main() {
    FILE * matchRecords;
    MATCH * scoreTable;
    TEAM * leagueTable;
    char willSearch[80];

    // Name of file that going to be used
    char tableFileName[] = "CNG140-P5-Dogu-Cagri-Ozarpaci.txt";

    // Create the file stream
    matchRecords = fopen(tableFileName, "r");

    // Check if score table file is exist
    if (!matchRecords) {
        printf("Score table file does NOT exist!\n");
        exit(EXIT_FAILURE);
    }

    printf("Score records file has been successully loaded!\nFollowing records have been loaded:\n\n");
    DisplayMatchRecords(matchRecords);

    // Get the number of matches played
    int numOfLines = getNumberOfLines(matchRecords);

    // Create an array of structures
    scoreTable = (MATCH *) malloc(sizeof(MATCH) * numOfLines);

    // Check if memory is allocated
    if (!scoreTable) {
        printf("Something went wrong while trying to allocate memory!\n");
        exit(EXIT_FAILURE);
    }

    // Load the score table into an array of structure
    LoadScoreTable(matchRecords, scoreTable);

    // Create the league table
    leagueTable = CreateLeagueTable(scoreTable, numOfLines);

    // Sort the league table
    OrderLeagueTable(leagueTable, numOfLines);
    
    // Display the league table
    DisplayLeagueTable(leagueTable, numOfLines);

    // Clear the buffer
    fflush(stdin);

    char wantSearch = 'S';
    int indexOf, rank;
    while (wantSearch == 'S' || wantSearch == 's') {
        printf("Press E for exit or S for search: ");
        scanf("\n%c", &wantSearch);
        fflush(stdin);

        if (wantSearch == 'S' || wantSearch == 's') {
            printf("Enter the name of the team: ");

            // Get a team name from user that going to be searched in league table
            scanf("\n%s", willSearch);
            fflush(stdin);

            // Search the team that entered, if it's exist then return the index
            indexOf = SearchTeam(leagueTable, numOfLines, willSearch);
            rank = indexOf + 1;

            // Capitalize the user's input to display
            capitalize(willSearch);

            // Check if the team user entered is exist in the league table
            if (indexOf != -1) {
                printf("%s's rank is %d!\n", willSearch, rank);
            } else {
                printf("That team is unknown! Please try again!\n");
            }

        } else  {
            break;
        }
    }

    printf("Wishing you a successful week!\n");

    // Close the files that openeed
    fclose(matchRecords);

    // Set pointers free
    free(scoreTable);
    free(leagueTable);

    return 0;
}

void DisplayMatchRecords(FILE * matchRecords) {
    printf("Formed Leauge Table is as follows:\n\n");
    printf("  Team1        Team2            Team1Score   Team2Score\n");
    printf("  -----------------------------------------------------\n");
    
    char firstTeam[80], secondTeam[80];
    int firstScore, secondScore;
    while (!feof(matchRecords)) {
        fscanf(matchRecords, "%s %s %d %d", firstTeam, secondTeam, &firstScore, &secondScore);
        printf("  %-12s %-12s %5d %12d\n", firstTeam, secondTeam, firstScore, secondScore);
    }

    printf("\n\n");
    rewind(matchRecords);
}

/**
 * This function counts the number of new lines (rows) in a file '\'
 * 
 * @param FILE fileStream
 *   File that number of new lines will be counted
 * 
 * @return int
 *   return number of rows
*/
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

/**
 * This function reads the the match datas in a file
 * 
 * @param FILE matchRecords
 *   File that has match datas
 * @param MATCH scoreTable
 *   Array of structure that will have our match datas
*/
void LoadScoreTable(FILE * matchRecords, MATCH * scoreTable) {
    int i = 0;
    while (!feof(matchRecords)) {
        fscanf(matchRecords, "%s %s %d %d\n", scoreTable[i].firstTeam, scoreTable[i].secondTeam, &scoreTable[i].firstScore, &scoreTable[i].secondScore);
        i++;
    }
}

/**
 * This function creates a league table according to match records that fetched from a file
 * 
 * @param FILE matchRecords
 *   File that has match datas
 * @param MATCH scoreTable
 *   Array of structure that will have our match datas
 * 
 * @return TEAM * {teamName, win, draw, loss, avg, points, goals} a structure for each team
*/
TEAM * CreateLeagueTable(MATCH * scoreTable, int size) {
    TEAM * leagueTable;
    int teamCount = size * 2;

    // Dynamic array creation
    leagueTable = (TEAM *) malloc(teamCount * sizeof(TEAM));

    // Check if memory is allocated
    if (!leagueTable) {
        printf("Something went wrong while trying to load the league table!\n");
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
        leagueTable[i].points  = (leagueTable[i].win * WIN_POINTS) + (leagueTable[i].draw * DRAW_POINTS);
        leagueTable[i + size].points  = (leagueTable[i + size].win * WIN_POINTS) + (leagueTable[i + size].draw * DRAW_POINTS);

        // Calculate the average
        leagueTable[i].average  = scoreTable[i].firstScore - scoreTable[i].secondScore;
        leagueTable[i + size].average = scoreTable[i].secondScore - scoreTable[i].firstScore;
    }

    return leagueTable;
}

/**
 * This function sorts the teams based on their stats
 * 
 * @param TEAM * leagueTable
 *   Arroy of structure that holds the teams' stats
 * @param MATCH size
 *   Number of lines or matches played between 2 teams (multiply 2 to get total number of teams)
*/
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

/**
 * Display league table which contains stats of each team
 * 
 * @param TEAM * leagueTable
 *   Arroy of structure that holds the teams' stats
 * @param MATCH size
 *   Number of lines or matches played between 2 teams (multiply 2 to get total number of teams)
*/
void DisplayLeagueTable(TEAM * leagueTable, int size) {
    size *= 2;

    printf("Formed Leauge Table is as follows:\n\n");
    printf("  Teams            Win  Draw    Loss    Points    Average    Goals\n");
    printf("  ----------------------------------------------------------------------\n");

    for (int i = 0; i < size; i++) {
        printf("  %-14s %3d %4d %7d %7d %9d %10d\n", leagueTable[i].teamName, leagueTable[i].win, leagueTable[i].draw, leagueTable[i].loss, leagueTable[i].points, leagueTable[i].average, leagueTable[i].goals);
    }

    printf("\n");
}

/**
 * Search for the team that is entered by the user
 * 
 * @param TEAM * leagueTable
 *   Arroy of structure that holds the teams' stats
 * @param MATCH size
 *   Number of lines or matches played between 2 teams (multiply 2 to get total number of teams)
 * @param char * willSearch
 *   A string obtained from user 
*/
int SearchTeam(TEAM * leagueTable, int size, char * willSearch) {
    size *= 2;

    for (int i = 0; i < size; i++) {
        if ((strcmp(strlwr(leagueTable[i].teamName), strlwr(willSearch))) == 0) {
            return i;
        }
    }

    return -1;
}

/**
 * Capitalizes a string
 * 
 * @param char * str
 *   String that going to be capitalized
*/
void capitalize(char *str) {
    *str = toupper(*str); 
}