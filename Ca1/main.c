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
    struct city *next;
} cityNode;

typedef struct itemCategory {
    char name[50]; // E.g. Pet Supplies
    double totalSales;
    int order;
    struct itemCategory *next;
} itemCategoryNode;

// Structs Setup
struct city cities[500]; // Temporary Upper Limit (Linked List Later)
int cityCount = 0;
cityNode * head = NULL;

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
    //sortCities();

    // Print City Data
    printTopThreeCities();
    printBottomThreeCities();

    // Sort the Item Categories
    //sortItemCategories();

    // Print Item Categories Data
    //printTopThreeItemCategories();
    //printBottomThreeItemCategories();

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
    check = mkdir("reports", 0777);

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
        // New City
        // No Cities Currently Exist
        // Create head of the linked list.
        head = malloc(sizeof(cityNode));
        if (head == NULL) {
            // Malloc Returned Null
        }
        strcpy(head->name, cityName);
        head->totalSales = salesValue;
        head->next = NULL;
        cityCount++;
    }

    // Check if the city already matches one of those existing.
    else {
        // Iterate through the linked list.
        cityNode * current = head;

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
        current = head; // Reset Current

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
    int swapsMade;
    cityNode * current = head;

    do {
        swapsMade = 0;

        // Sort everything
        while (current != NULL) {
            
            if  (current->next != NULL) {
                // Check if its the head
                if (current == head) {

                    // Head
                    if ((current->next->totalSales) > current->totalSales) {
                        // Different method depending on 2, or 3 and above.
                        if (current->next->next != NULL) {
                            // 3 and above inside linked list.
                            cityNode * nodeA = current->next->next;
                            current->next->next = head;
                            current->next = nodeA;
                        }
                        else {
                            // 2 only
                            current->next->next = head;
                            head->next = NULL;
                            head = current->next;
                        }

                        swapsMade++;
                    }


                }

                else if (current->next->next != NULL) {
                    // Normal Sorting

                    if ((current->next->next->totalSales) > (current->next->totalSales)) {
                        // Swap next and next->next in the linked list
                        cityNode * nodeA = current->next;
                        cityNode * nodeB = current->next->next;
                        
                        current->next =  nodeB;
                        nodeA->next = nodeB->next;
                        nodeB->next = nodeA;
                        swapsMade++;
                    }
                }
            }

            current = current->next;
        }

        current = head; // Reset the head for second sort iteration.
    } while (swapsMade > 0);
}

// Sort Cities must be run first
void printTopThreeCities () {
    printf("\nTop Three Cities\n=======================================================================\n");
    cityNode * current = head;

    cityNode * first = malloc(sizeof(cityNode));
    cityNode * second = malloc(sizeof(cityNode));
    cityNode * third = malloc(sizeof(cityNode));

    first->totalSales = second->totalSales = third->totalSales = -DBL_MAX;

    //cityNode * current = head;
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
    printf("%*s", -50, first->name); // Name
    printf("\t%*.2lf \n", 15, first->totalSales); // Total Sales

    printf("%*s", -50, second->name); // Name
    printf("\t%*.2lf \n", 15, second->totalSales); // Total Sales

    printf("%*s", -50, third->name); // Name
    printf("\t%*.2lf \n", 15, third->totalSales); // Total Sales

    printf("=======================================================================\n");
}

// Sort Cities must be run first
void printBottomThreeCities () {
    printf("\nBottom Three Cities\n=======================================================================\n");

    cityNode * current = head;

    cityNode * first = malloc(sizeof(cityNode));
    cityNode * second = malloc(sizeof(cityNode));
    cityNode * third = malloc(sizeof(cityNode));

    first->totalSales = second->totalSales = third->totalSales = DBL_MAX;

    //cityNode * current = head;
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

        else if (current->totalSales > first->totalSales) {
            first = current;
        }
        
        current = current->next;
    }

    //Print
    printf("%*s", -50, first->name); // Name
    printf("\t%*.2lf \n", 15, first->totalSales); // Total Sales

    printf("%*s", -50, second->name); // Name
    printf("\t%*.2lf \n", 15, second->totalSales); // Total Sales

    printf("%*s", -50, third->name); // Name
    printf("\t%*.2lf \n", 15, third->totalSales); // Total Sales

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



