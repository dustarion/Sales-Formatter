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

// Function Prototypes
void printTime();

void printTotalSales(float);
void printAverageSales(int, float); 

void readPurchaseRecordFile(FILE*);
void setupDirectory();
void updateSalesEntries (char*, char*);

void updateCity (char*, float);
void updateItemCategory (char*, float);

void sortCities ();
void printTopThreeCities ();
void printBottomThreeCities ();

void sortItemCategories ();
void printTopThreeItemCategories ();
void printBottomThreeItemCategories ();

// Structs
typedef struct city {
	char name[50]; // E.g. Fort Worth
    double totalSales;
    int order;
} cityNode;

typedef struct itemCategory {
    char name[50]; // E.g. Pet Supplies
    double totalSales;
    int order;
} itemCategoryNode;

// Structs Setup
struct city cities[500]; // Temporary Upper Limit (Linked List Later)
int cityCount = 0;

struct itemCategory itemCategories[500]; // Temporary Upper Limit (Linked List Later)
int itemCategoryCount = 0;

double totalSales = 0;
int totalTransactions = 0;

// Main Code
int main(int argc, const char * argv[]) {

    // Print Starting Time
    printTime();

    // Check if Correct number of parameters
    if ((argc < 2)||( argc > 2)) {
        printf("Wrong number of parameters!\n");
        exit(1);
    }

    // Setup Folders
    setupDirectory();

	// Read the txt File
    FILE *fp = fopen(argv[1], "r");
	readPurchaseRecordFile(fp);

    // Print the total sales of the year.
    printTotalSales(totalSales);

    // Print the average sales
    printAverageSales(cityCount, totalSales/cityCount);

    // Sort the cities
    sortCities();

    // Print City Data
    printTopThreeCities();
    printBottomThreeCities();

    // Sort the Item Categories
    sortItemCategories();

    // Print Item Categories Data
    printTopThreeItemCategories();
    printBottomThreeItemCategories();

    // Print Ending Time
    printTime();

    return 0;
}

// Print Functions

void printTime() {
    time_t ltime;
    ltime = time(NULL);
    printf("%s\n", asctime(localtime(&ltime)));
}

void printTotalSales(float totalSales) {
    printf("\nTotal Sales of the year is: $%.2f\n", totalSales);
}

void printAverageSales(int numberOfCities, float averageSales) {
    printf("\nThe average sales for %d transactions for %d cities are : $%.2f\n", totalTransactions, numberOfCities, averageSales);
}

// General Functions
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
    check = mkdir("reports", 0700); 

    // Check if directory is created or not 
    if (check == -1) {
        //This could also be because reports directory already exists.
        //printf("Unable to create reports directory\nTerminating Program...\n\n\n");
        //exit(1); 
    }
}

void updateSalesEntries (char cityName[50], char line[255]) {
    /*char fileName[64];
    strcpy(fileName, "./reports/");
    strcat(fileName, cityName);
    strcat(fileName, ".txt"); 
    FILE * output;
    if ((output = fopen(fileName, "a")) == NULL) {
        printf("Cannot open file %s\n", fileName);
    }

    fprintf(output, "%s\n", line);
    fclose(output);*/
}

void updateCity (char cityName[50], float salesValue) {

    if (cityCount == 0) {
        //New City
        strcpy(cities[cityCount].name ,cityName);
        cities[cityCount].totalSales = salesValue;
        cityCount++;
        // No Cities Currently Exist
        // Create head of the linked list.



    }

    // Check if the city already matches one of those existing.
    else {
        for (int i = 0; i < cityCount; i++) {
            if (strcmp(cityName, cities[i].name) == 0) {
                // City Exists.
                cities[i].totalSales += salesValue;
                return;
            }
        }

        // City does not Exist.
        strcpy(cities[cityCount].name ,cityName);
        cities[cityCount].totalSales = salesValue;
        cityCount++;
    }
}

void updateItemCategory (char itemCategoryName[50], float salesValue) {
    if (itemCategoryCount == 0) {
        //New item category
        strcpy(itemCategories[itemCategoryCount].name ,itemCategoryName);
        itemCategories[itemCategoryCount].totalSales = salesValue;
        itemCategoryCount++;
    }

    // Check if the item category already matches one of those existing.
    else {
        for (int i = 0; i < itemCategoryCount; i++) {
            if (strcmp(itemCategoryName, itemCategories[i].name) == 0) {
                // City Exists.
                itemCategories[i].totalSales += salesValue;
                return;
            }
        }

        // Item category does not Exist.
        strcpy(itemCategories[itemCategoryCount].name ,itemCategoryName);
        itemCategories[itemCategoryCount].totalSales = salesValue;
        itemCategoryCount++;
    }
}

void sortCities () {

    // Reset the order variables
    for (int i = 0; i < cityCount; i++) {
        cities[i].order = 0;
    }
 
    int order = 1;
    while (order <= cityCount) {
        double highestSale = -100.0;
        for (int i = 0; i < cityCount; i++) {
            if (cities[i].order == 0) {
                if (cities[i].totalSales > highestSale) {
                    highestSale = cities[i].totalSales; // Update Highest Sales
                }
            }
        }

        for (int i = 0; i < cityCount; i++) {
            if (cities[i].totalSales == highestSale) {
                cities[i].order = order;
                order++;
            }
        }
    }
}

// Sort Cities must be run first
void printTopThreeCities () {
    printf("\nTop Three Cities\n=======================================================================\n");

    for (int order = 1; order <= 3; order++) {
        for (int i = 0; i < cityCount; i++) {
            if (cities[i].order == order) {
                printf("%*s", -50, cities[i].name); // City Name
                printf("\t%*.2lf\n", 15, cities[i].totalSales);
            }
        }
    }

    printf("=======================================================================\n");
}

// Sort Cities must be run first
void printBottomThreeCities () {
    printf("\nBottom Three Cities\n=======================================================================\n");

    for (int order = cityCount-2; order <= cityCount; order++) {
        for (int i = 0; i < cityCount; i++) {
            if (cities[i].order == order) {
                printf("%*s", -50, cities[i].name); // City Name
                printf("\t%*.2lf\n", 15, cities[i].totalSales);
            }
        }
    }

    printf("=======================================================================\n");
}

void sortItemCategories () {

    // Reset the order variables
    for (int i = 0; i < itemCategoryCount; i++) {
        itemCategories[i].order = 0;
    }
 
    int order = 1;
    while (order <= itemCategoryCount) {
        double highestSale = -100.0;
        for (int i = 0; i < itemCategoryCount; i++) {
            if (itemCategories[i].order == 0) {
                if (itemCategories[i].totalSales > highestSale) {
                    highestSale = itemCategories[i].totalSales; // Update Highest Sales
                }
            }
        }

        for (int i = 0; i < itemCategoryCount; i++) {
            if (itemCategories[i].totalSales == highestSale) {
                itemCategories[i].order = order;
                order++;
            }
        }
    }
}

// Sort Item Categories must be run first
void printTopThreeItemCategories () {
    printf("\nTop Three Item Categories\n=======================================================================\n");

    for (int order = 1; order <= 3; order++) {
        for (int i = 0; i < itemCategoryCount; i++) {
            if (itemCategories[i].order == order) {
                printf("%*s", -50, itemCategories[i].name); // Item Category Name
                printf("\t%*.2lf \n", 15, itemCategories[i].totalSales);
            }
        }
    }

    printf("=======================================================================\n");
}

// Sort Cities must be run first
void printBottomThreeItemCategories () {
    printf("\nBottom Three Item Categories\n=======================================================================\n");

    for (int order = itemCategoryCount-2; order <= itemCategoryCount; order++) {
        for (int i = 0; i < itemCategoryCount; i++) {
            if (itemCategories[i].order == order) {
                printf("%*s", -50, itemCategories[i].name); // City Name
                printf("\t%*.2lf \n", 15, itemCategories[i].totalSales);
            }
        }
    }

    printf("=======================================================================\n");
}



