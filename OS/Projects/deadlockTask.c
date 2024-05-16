//Lana Amjad Hamayel
//1200209
//section 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define P_NUM 100
#define R_NUM 100

#define LINE_SIZE 1000

int numberOfProcesses = 0;
int numberOfResources = 0;

int allocation[P_NUM][R_NUM];
int request[P_NUM][R_NUM];
int available[R_NUM];
int finish[P_NUM];

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function prototypes
void readingCSV_files(const char *filename, int matrix[P_NUM][R_NUM], int dimension);

void readAvailableFile(const char *filename, int matrix[R_NUM]);

void printMatrices();

void detectDeadlock();

int checkingMatrices();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main() {
     readingCSV_files("Allocation.csv", allocation, 0);
     int value1 = numberOfProcesses;
     int value2 = numberOfResources;
     readingCSV_files("Request.csv", request, 0);
     int value3 = numberOfProcesses;
     int value4 = numberOfResources;
     readAvailableFile("available.csv", available);
     int value5 = numberOfResources;

    if (value1 != value3 || value2 != value4 || value2 != value5 || value4 != value5 || numberOfResources <= 0 || numberOfProcesses <= 0 ||
        !checkingMatrices()) {
        printf("-->> It appears that some data is missing, Please rectify this.\n");
        exit(0);
    }
    printMatrices();//calling printMatrices() function
    detectDeadlock();//calling detectDeadlock() function
    printf("-------------------------------------------------------------------\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////
//A function which used to read the csv files
void readingCSV_files(const char *filename, int matrix[P_NUM][R_NUM], int dimension) {
    FILE *file = fopen(filename, "r");
    if (!file) {//checking if the file is exited or not
        printf("Could not open file %s for reading.\n", filename);
        exit(0);
    }

    char lineInsideFile[LINE_SIZE];
    int row = 0, col;

    while (fgets(lineInsideFile, sizeof(lineInsideFile), file)) {
        if (row == 0) { // Skip first(header) line
            row++;
            continue;
        }

        char *token = strtok(lineInsideFile,
                             ",");//cutting the line using strtok function from string.h library,depending on char ","
        col = 0;
        while (token != NULL) {
            if (!dimension || col > 0) {
                matrix[row - 1][col - 1] = atoi(token);
            }
            token = strtok(NULL, ",");
            col++;
        }
        if (row == 1) {
            if (!dimension) {
                numberOfResources = col - 1;
            }
        }
        row++;
    }
    fclose(file);
    row -= 1;
    numberOfProcesses = row;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//A function which used to read the Available.csv file
void readAvailableFile(const char *filename, int matrix[R_NUM]) {
    int col = 0;
    FILE *file = fopen(filename, "r");
    if (!file) {//checking if the file is exited or not
        printf("Could not open file %s for reading.\n", filename);
        exit(1);
    }

    char lineInsideFile[LINE_SIZE];
    int row = 0;

    while (fgets(lineInsideFile, sizeof(lineInsideFile), file)) {
        if (row == 1) { // Process the second line
            char *token = strtok(lineInsideFile, ",");
            col = 0;

            while (token != NULL && col < R_NUM) {
                matrix[col] = atoi(token);
                token = strtok(NULL, ",");
                col++;
            }

            // Break after executing the second line
            break;
        }

        row++;
    }
    fclose(file);
    numberOfResources=col;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//A function which is used to print the read matrices
void printMatrices() {

    printf("\tAllocation Matrix:\n");
    for (int k = 0; k < numberOfResources; k++) {
        printf("\tR%d", k + 1);
    }
    printf("\n");

    for (int i = 0; i < numberOfProcesses; i++) {
        printf("p%d\t", i + 1);
        for (int j = 0; j < numberOfResources; j++) {
            printf("%d\t", allocation[i][j]);
        }
        printf("\n");
    }
    printf("---------------------------------------------\n");
    printf("\n");
    printf("\tRequest Matrix:\n");
    for (int k = 0; k < numberOfResources; k++) {
        printf("\tR%d", k + 1);
    }
    printf("\n");
    for (int i = 0; i < numberOfProcesses; i++) {
        printf("p%d\t", i + 1);
        for (int j = 0; j < numberOfResources; j++) {
            printf("%d\t", request[i][j]);
        }
        printf("\n");
    }
    printf("---------------------------------------------\n");
    printf("\n");
    printf("\tAvailable Matrix:\n");
    for (int k = 0; k < numberOfResources; k++) {
        printf("\tR%d", k + 1);
    }
    printf("\n");
    printf("p%d\t", 1);
    for (int i = 0; i < numberOfResources; i++) {
        printf("%d\t", available[i]);
    }
    printf("\n");
    printf("---------------------------------------------\n");

    printf("\n\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//A function which used to checking the occurrence of deadlock or not
void detectDeadlock() {
    int k = 0;
    for (int i = 0; i < numberOfProcesses; ++i) {
        if (!finish[i]) {
            int safe = 1;
            for (int j = 0; j < numberOfResources; ++j) {
                if (request[i][j] > available[j]) {
                    safe = 0;
                    break;
                }
            }

            if (safe) {
                // pass resources to the process
                for (int j = 0; j < numberOfResources; ++j) {
                    available[j] += allocation[i][j];
                }
                finish[i] = 1;

                printf("In safe sequence, %d) Process [P%d] is executing successfully.\n", ++k, i + 1);
                i = -1;  // Reset i to start from the beginning

            }
        }
    }

    int deadlockDetected = 0;
    for (int i = 0; i < numberOfProcesses; i++) {
        if (!finish[i]) {
            deadlockDetected = 1;
            printf("Process [P%d] is deadlocked\n", i + 1);
        }
    }

    if (!deadlockDetected) {
        printf("\n\nNo Deadlock Detection. System is in a safe state!!!\n");
        for (int i = 0; i < numberOfProcesses; ++i) {
            if (finish[i]) {
                printf("Process [P%d] had executed successfully.\n", i + 1);
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//A function to checking the consistent
int checkingMatrices() {
    for (int i = 0; i < numberOfProcesses; i++) {
        for (int j = 0; j < numberOfResources; ++j) {
            if (allocation[i][j] < 0 || request[i][j] < 0) {
                return 0;
            }
        }
    }
    return 1;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////