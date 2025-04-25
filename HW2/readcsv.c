/// 
/// File: readcsv.c
/// Usage: ./readcsv.c [file].csv
/// A program to print out multiple triangles of the form:
/// <pre>
///     Total population across 10 zipcodes is 447379.
///     The fewest people live in 91504, population 24939.
///     The most people live in 93535, population 72046
/// </pre>
/// 
/// @author jht3250 : John Treon
/// 
// // // // // // // // // // // // // // // // // // // // // // // // 

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// method readCSV
// @returns nothing
// reads in a csv file prints data about population, and zip codes.
void readCsv(char *filename[150]) {
    FILE *f;
    f = fopen(*filename, "r");

    if (f == NULL) {
        printf("file not found!");
        exit(0);
    }

    char line[150];
    char field[150];

    int maxPop = 0, maxZip = 0, minZip = 0, currZip = 0, totalZip = 0, totalPop = 0, currentPop = 0;
    int minPop = __INT_MAX__;
    
    char *tempZipChar; 
    char *tempPopChar; 
    fgets(line, 150, f); //skipping header
    while (fgets(line, 150, f)) {
        int index = 0;
        int counter = 0;
        for (int i = 0; line[i] != '\0'; i++) {
            if (line[i] != ',') {
                field[index] = line[i];
                index++;
            }
            else {
                if (counter == 0) { // zip codes
                    totalZip++;
                    currZip = strtol(field, &tempZipChar, 10);
                }
                if (counter == 1) { // populations
                    currentPop = strtol(field, &tempPopChar, 10);
                    if (currentPop > maxPop) { // checking for max
                        maxPop = currentPop;
                        maxZip = currZip;
                    } 
                    else if (currentPop < minPop) { // checking for min
                        minPop = currentPop; 
                        minZip = currZip;
                    }
                    totalPop += currentPop; 
                }
                ++counter;
                for (int j = 0; j <= index; j++) {
                    field[j] = '\0'; 
                }
                index = 0;
            } 
            
        }
    }

    // Total population across 10 zipcodes is 447379.
    printf("Total population across %d zipcodes is %d.\n", totalZip, totalPop);
    // The fewest people live in 91504, population 24939.
    printf("The fewest people live in %d, population %d.\n", minZip, minPop);
    // The most people live in 93535, population 72046.
    printf("The most people live in %d, population %d.\n", maxZip, maxPop);
    
    fclose(f);
}

int main(int argc, char *argv[]) {
    readCsv(&argv[1]);
    printf("\n"); 
}