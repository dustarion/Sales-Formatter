//
//  main.c
//  Ca1
//
//  Created by Dalton Ng on 31/10/18.
//  Copyright © 2018 Dalton Prescott. All rights reserved.
//

// Import Relevant Libs
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <math.h>
#include <time.h>
#include <float.h>

// Function Prototypes
void printTime();

void printTotalSales(float);
void printAverageSales(int, float); 
void printAverageItemSales(int, float);

void readPurchaseRecordFile(FILE*);
void setupDirectory();
void updateSalesEntries (char*, char*);

void updateCity (char*, float);
void updateItemCategory (char*, float);

void printTopThreeCities ();
void printBottomThreeCities ();

void printTopThreeItemCategories ();
void printBottomThreeItemCategories ();

// Structs
typedef struct city {
	char name[50]; // E.g. Fort Worth
    double totalSales;
    int order;
    struct city *next;
} cityNode;

typedef struct itemCategory {
    char name[50]; // E.g. Pet Supplies
    double totalSales;
    int order;
    struct itemCategory *next;
} itemCategoryNode;

// Linked List Setup
int cityCount = 0;
cityNode * chead = NULL;

int itemCategoryCount = 0;
itemCategoryNode * ihead = NULL;

double totalSales = 0;
int totalTransactions = 0;

// Main Code
int main(int argc, const char * argv[]) {

    // Print Starting Time
    printTime();

    // Check if Correct number of parameters
    if ((argc < 2)||( argc > 2)) {
        printf("Please only input one argument.\nTry again.\n\n");
        exit(1);
    }

    // Setup Folders
    setupDirectory();

	// Read the txt File
    FILE *fp;

    // Ensure File Exists
    if((fp = fopen(argv[1], "r")) == NULL) {
        printf("Unable to open file %s.\nProgram Terminated\n", argv[1]);
        return 1;
    }
	readPurchaseRecordFile(fp);

    // Print the total sales of the year.
    printTotalSales(totalSales);

    // Print the average sales
    printAverageSales(cityCount, totalSales/cityCount);

    // Print City Data
    printTopThreeCities();
    printBottomThreeCities();

    // Print the average sales for item categories.
    printAverageItemSales(itemCategoryCount, totalSales/itemCategoryCount);

    // Print Item Categories Data
    printTopThreeItemCategories();
    printBottomThreeItemCategories();

    // Print Ending Time
    printTime();

    return 0;
}

// General Print Functions

void printTime() {
    time_t ltime;
    ltime = time(NULL);
    printf("\033[0;32m%s\033[0m\n", asctime(localtime(&ltime)));
}

void printTotalSales(float totalSales) {
    printf("Total Sales of the year is: \033[1;36m$%.2f\033[0m\n", totalSales);
}

void printAverageSales(int numberOfCities, float averageSales) {
    printf("\nThe average sales for \033[0;36m%d\033[0m transactions for \033[0;36m%d\033[0m cities are : \033[1;36m$%.2f\033[0m\n", totalTransactions, numberOfCities, averageSales);
}

void printAverageItemSales(int numberOfItemCategories, float averageSales) {
    printf("\nThe average sales from \033[0;36m%d\033[0m Item Categories are : \033[1;36m$%.2f\033[0m\n", numberOfItemCategories, averageSales);
}

// File IO Functions
void readPurchaseRecordFile(FILE *fp) {
    
    // Definitions
    const int SIZE = 255;
    char line[SIZE]; // String to store the current line.
    
    // Copy the physical file into my array.
    int i = 0;
    while(fgets(line,SIZE,fp) != NULL) {
        char originalLine[SIZE+1];
        strcpy(originalLine, line);

        char cityName[50]; // E.g. Fort Worth
        char itemCategoryName[50];
        float salesValue;
        strtok(line, "\t"); // Date
        strtok(NULL, "\t"); // Time
        strcpy(cityName, strtok(NULL, "\t")); // City
        strcpy(itemCategoryName, strtok(NULL, "\t")); // Item Category
        salesValue = atof(strtok(NULL, "\t")); // Sales Value
        strtok(NULL, "\n"); // Payment Type

        // Update approriately
        totalSales += salesValue;
        updateCity (cityName, salesValue);
        updateItemCategory (itemCategoryName, salesValue);

        // Update Files
        updateSalesEntries (cityName, originalLine);

        // Update Total Transaction Count
        totalTransactions++;

        i++;
    }
    fclose(fp);
}

void setupDirectory() {
    int check; 
    check = mkdir("reports", 0777);

    // Check if directory is created or not 
    if (check == -1) {
        //This could also be because reports directory already exists.
        //printf("Unable to create reports directory\nTerminating Program...\n\n\n");
        //exit(1); 
    }
}

void updateSalesEntries (char cityName[50], char line[255]) {
    
    char fileName[64];
    strcpy(fileName, "./reports/");
    strcat(fileName, cityName);
    strcat(fileName, ".txt"); 
    FILE * output;
    if ((output = fopen(fileName, "a")) == NULL) {
        printf("Cannot open file %s\n", fileName);
    }

    fprintf(output, "%s\n", line);
    fclose(output);
}

// Linked List Functions
void updateCity (char cityName[50], float salesValue) {
    if (cityCount == 0) {
        // New City
        // No Cities Currently Exist
        // Create head of the linked list.
        chead = malloc(sizeof(cityNode));
        if (chead == NULL) {
            // Malloc Returned Null
            printf("Unable to allocate memory.\n");
        }
        strcpy(chead->name, cityName);
        chead->totalSales = salesValue;
        chead->next = NULL;
        cityCount++;
    }

    // Check if the city already matches one of those existing.
    else {
        // Iterate through the linked list.
        cityNode * current = chead;

        while (current != NULL) {
            //printf("Comparing%s and %s, it is %d\n", cityName, current->name, strcmp(cityName, current->name));

            if (strcmp(cityName, current->name) == 0) {
                // City Exists.
                current->totalSales += salesValue;
                return;
            }

            current = current->next; // Move to the next value
        }

        // City does not Exist.
        current = chead; // Reset Current

        while (current->next != NULL) {
            current = current->next;
        }
        // Current should now be pointed at the last item in the linked list.
        
        // Add a new variable
        current->next = malloc(sizeof(cityNode));
        strcpy(current->next->name, cityName);
        current->next->totalSales = salesValue;
        current->next->next = NULL;

        // Update CityCount
        cityCount++;
    }
}

void updateItemCategory (char itemCategoryName[50], float salesValue) {
    if (itemCategoryCount == 0) {
        // New Item Category
        // No Item Categories Currently Exist
        // Create head of the linked list.
        ihead = malloc(sizeof(itemCategoryNode));
        if (ihead == NULL) {
            // Malloc Returned Null
            printf("Unable to allocate memory.\n");
        }
        strcpy(ihead->name, itemCategoryName);
        ihead->totalSales = salesValue;
        ihead->next = NULL;
        itemCategoryCount++;
    }

    // Check if the category already matches one of those existing.
    else {
        // Iterate through the linked list.
        itemCategoryNode * current = ihead;

        while (current != NULL) {

            if (strcmp(itemCategoryName, current->name) == 0) {
                // Category Exists.
                current->totalSales += salesValue;
                return;
            }

            current = current->next; // Move to the next value
        }

        // Category does not Exist.
        current = ihead; // Reset Current

        while (current->next != NULL) {
            current = current->next;
        }
        // Current should now be pointed at the last item in the linked list.
        
        // Add a new variable
        current->next = malloc(sizeof(itemCategoryNode));
        strcpy(current->next->name, itemCategoryName);
        current->next->totalSales = salesValue;
        current->next->next = NULL;

        // Update ItemCategoryCount
        itemCategoryCount++;
    }
}


// Results Printing Functions

void printTopThreeCities () {
    printf("\nTop Three Cities\n=======================================================================\n");

    cityNode * first = malloc(sizeof(cityNode));
    cityNode * second = malloc(sizeof(cityNode));
    cityNode * third = malloc(sizeof(cityNode));

    first->totalSales = second->totalSales = third->totalSales = -DBL_MAX;

    cityNode * current = chead;
    while (current != NULL) {
        if (current->totalSales > first->totalSales) {
            third = second;
            second = first;
            first = current;
        }

        else if (current->totalSales > second->totalSales) {
            third = second;
            second = current;
        }

        else if (current->totalSales > third->totalSales) {
            third = current;
        }
        
        current = current->next;
    }

    printf("\033[0;33m%*s\033[0m", -50, first->name); // Name
    printf("\t\033[1;36m%*.2lf\033[0m \n", 15, first->totalSales); // Total Sales

    printf("\033[0;33m%*s\033[0m", -50, second->name); // Name
    printf("\t\033[1;36m%*.2lf\033[0m \n", 15, second->totalSales); // Total Sales

    printf("\033[0;33m%*s\033[0m", -50, third->name); // Name
    printf("\t\033[1;36m%*.2lf\033[0m \n", 15, third->totalSales); // Total Sales

    printf("=======================================================================\n");
}

// Sort Cities must be run first
void printBottomThreeCities () {
    printf("\nBottom Three Cities\n=======================================================================\n");

    cityNode * first = malloc(sizeof(cityNode));
    cityNode * second = malloc(sizeof(cityNode));
    cityNode * third = malloc(sizeof(cityNode));

    first->totalSales = second->totalSales = third->totalSales = DBL_MAX;

    cityNode * current = chead;
    while (current != NULL) {
        if (current->totalSales < third->totalSales) {

            first = second;
            second = third;
            third = current;
        }

        else if (current->totalSales < second->totalSales) {
            first = second;
            second = current;
        }

        else if (current->totalSales < first->totalSales) {
            first = current;
        }
        
        current = current->next;
    }

    //Print
    printf("\033[0;33m%*s\033[0m", -50, first->name); // Name
    printf("\t\033[1;36m%*.2lf\033[0m \n", 15, first->totalSales); // Total Sales

    printf("\033[0;33m%*s\033[0m", -50, second->name); // Name
    printf("\t\033[1;36m%*.2lf\033[0m \n", 15, second->totalSales); // Total Sales

    printf("\033[0;33m%*s\033[0m", -50, third->name); // Name
    printf("\t\033[1;36m%*.2lf\033[0m \n", 15, third->totalSales); // Total Sales

    printf("=======================================================================\n");
}

// Sort Item Categories must be run first
void printTopThreeItemCategories () {
    printf("\nTop Three Item Categories\n=======================================================================\n");

    itemCategoryNode * first = malloc(sizeof(itemCategoryNode));
    itemCategoryNode * second = malloc(sizeof(itemCategoryNode));
    itemCategoryNode * third = malloc(sizeof(itemCategoryNode));

    first->totalSales = second->totalSales = third->totalSales = -DBL_MAX;

    itemCategoryNode * current = ihead;
    while (current != NULL) {
        if (current->totalSales > first->totalSales) {
            third = second;
            second = first;
            first = current;
        }

        else if (current->totalSales > second->totalSales) {
            third = second;
            second = current;
        }

        else if (current->totalSales > third->totalSales) {
            third = current;
        }
        
        current = current->next;
    }

    //Print
    printf("\033[0;33m%*s\033[0m", -50, first->name); // Name
    printf("\t\033[1;36m%*.2lf\033[0m \n", 15, first->totalSales); // Total Sales

    printf("\033[0;33m%*s\033[0m", -50, second->name); // Name
    printf("\t\033[1;36m%*.2lf\033[0m \n", 15, second->totalSales); // Total Sales

    printf("\033[0;33m%*s\033[0m", -50, third->name); // Name
    printf("\t\033[1;36m%*.2lf\033[0m \n", 15, third->totalSales); // Total Sales

    printf("=======================================================================\n");
}

void printBottomThreeItemCategories () {
    printf("\nBottom Three Item Categories\n=======================================================================\n");

    itemCategoryNode * first = malloc(sizeof(itemCategoryNode));
    itemCategoryNode * second = malloc(sizeof(itemCategoryNode));
    itemCategoryNode * third = malloc(sizeof(itemCategoryNode));

    first->totalSales = second->totalSales = third->totalSales = DBL_MAX;

    itemCategoryNode * current = ihead;
    while (current != NULL) {
        if (current->totalSales < third->totalSales) {

            first = second;
            second = third;
            third = current;
        }

        else if (current->totalSales < second->totalSales) {
            first = second;
            second = current;
        }

        else if (current->totalSales < first->totalSales) {
            first = current;
        }
        
        current = current->next;
    }

    //Print
    printf("\033[0;33m%*s\033[0m", -50, first->name); // Name
    printf("\t\033[1;36m%*.2lf\033[0m \n", 15, first->totalSales); // Total Sales

    printf("\033[0;33m%*s\033[0m", -50, second->name); // Name
    printf("\t\033[1;36m%*.2lf\033[0m \n", 15, second->totalSales); // Total Sales

    printf("\033[0;33m%*s\033[0m", -50, third->name); // Name
    printf("\t\033[1;36m%*.2lf\033[0m \n", 15, third->totalSales); // Total Sales

    printf("=======================================================================\n\n");
}