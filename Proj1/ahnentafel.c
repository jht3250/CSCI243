/// 
/// File: ahnentafel.c
/// Usage: ./ahnentafel 
/// A program to take user input regarding family relations and output data:
/// 
/// @author jht3250 : John Treon
/// 
// // // // // // // // // // // // // // // // // // // // // // // // 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//converts passed in decimal integer "num" to a char binary representation 
char * decToBin(int num) {
    int binary[32];
    int i = 0;
    char * chBin = malloc(32); // allocate mem for binary string

    while (num > 0) {
        binary[i] = num % 2; //calculate binary digits
        num = num / 2;
        i++;
    }
    int k = 0;
    printf("ahnentafel number in binary: "); 
    for (int j = i - 1; j >= 0; j--) {
        printf("%d", binary[j]); 
        if (binary[j] == 0) {
            chBin[k] = '0'; //store binary digits in char array
        }
        else if (binary[j] == 1) {
            chBin[k] = '1';
        }
        k++;
    }
    printf("%c", '\n');
    return chBin; // return binary char array
}

//converts passed in char binary representation to a decimal integer value
int binToDec(char *num) {
    char *binary = num;
    char *endptr;

    return strtol(binary, &endptr, 2); // convert binary string to decimal with strtol
}

char * relation(char *str) {
    char *relation;
    relation = strtok( str, " ,." ); // first call
    char * binary = malloc(sizeof(relation) + sizeof(char)); // allocate memory for binary string
    binary[0] = '1'; // first digit is always 1 for self
    int index = 1;
    char mothers[9] = "mother's"; // used to limit the length of comparison because of weird multiple null-terminating bug
    char mother[8] = "mother\n";
    char father[8] = "father\n";
    char fathers[9] = "father's";
    char self[5] = "self\n";
    while ( relation != NULL ) { // continue until end of str
        if ((strcmp(relation, mothers) == 0) || (strcmp(relation, mother) == 0)) {
            binary[index] = '1';
        }
        if ((strcmp(relation, fathers) == 0) || (strcmp(relation, father) == 0)) {
            binary[index] = '0';
        }
        if((strcmp(relation, self)) == 0) {
            return binary;
        }
        index++;
        relation = strtok( NULL, " ,." ); // subsequent call
    }
    return binary; // return binary char array
}

// converts char binary representation to familial relation
char * binToRel(char* binary) {
    if (sizeof(binary) == 1 && binary[0] == 1) { // if the input is just the individual "self"
        return "self";
    } 
    char * relations = malloc(9 * 32); // allocate memory for relations string
    for (int i = 1; i < strlen(binary); i++) {
        if (i == strlen(binary) - 1 || binary[i+1] == '\n') {
            if (binary[i] == '0') {
                strcat(relations, "father\n");
            }
            else if (binary[i] == '1') {
                strcat(relations, "mother\n");
            }
        }
        else {
            if (binary[i] == '0') {
                strcat(relations, "father's ");
            }
            else if (binary[i] == '1') {
                strcat(relations, "mother's ");
            }
        }
    }
    return relations; // return relations
}

int running = 1; // allows while to run

int main() {
    while (running) { // main logic
        int option; // user input for menu
        int *input = malloc(sizeof(int)); // integer input for option 2
        int c; // placeholder char for flushing stdin
        int ahnDec; // decimal ahnentafel number for option 3
        char * ahnBin; // binary ahnentafel number for option 4
        char * family; // relational representation for 2 and 3.
        
        printf("\n1) description\n2) ahnentafel number (base 10)\n3) ahnentafel number (base 2)\n4) relation (e.g. mother's mother's father)\n5) exit\n\n> ");
        scanf("%d", &option); //take user option input
        while((c = getchar()) != '\n'); //flush stdin
        switch (option) { // user choice
            case 1: // description
                printf("The Ahnentafel number is used to determine the relation\nbetween an individual and each of his/her direct ancestors.\n");
                break;
            case 2: // base-10 input
                printf("Enter the ahnentafel number in base-10: ");
                scanf("%d", input); // take base-10 input
                while((c = getchar()) != '\n');
                char *decBStr; // binary string that will come from converting from decimal
                decBStr = decToBin(input[0]);
                family = binToRel(decBStr); // relational
                printf("family relation: %s", family);
                printf("generations back: %ld\n", strlen(decBStr) - 1);
                break;
            case 3: // base-2 binary input
                printf("Enter the ahnentafel number in binary: ");
                char binStr[100];
                fgets(binStr, 100, stdin); // take binary char input
                ahnDec = binToDec(binStr);
                printf("base-10 value: %d\n", ahnDec);
                family = binToRel(binStr);
                printf("family relation: %s", family);
                printf("generations back: %ld\n", strlen(binStr) - 2);
                break;
            case 4: // relational input
                printf("Enter family relation (e.g.) \"father's mother\": ");
                char string[100];
                fgets(string, 100, stdin); // take relational input
                ahnBin = relation(string);
                printf("ahnentafel number in binary: %s\n", ahnBin);
                ahnDec = binToDec(ahnBin);
                printf("base-10 value: %d\n", ahnDec);
                printf("generations back: %ld\n", strlen(ahnBin) - 1);
                break;
            case 5: // exit
                printf("Goodbye.\n");
                exit(EXIT_SUCCESS);
        }
    }
}