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
#include <math.h>

// Function Prototypes
//void printCurrentTime(void);
int readPurchaseRecordFile(void);
void printContentsOfPurchases(int); // Remove Later
float getTotalSales(int);
void printTotalSales(float);
void updateCities(int);

// Purchase Record
struct purchaseRecord {
    char  date[50];         // E.g. 2012-01-01
    char  time[50];         // E.g. 09:00
    char  city[50];         // E.g. Fort Worth
    char  itemCategory[50]; // E.g. Women's Clothing
    float salesValue;       // E.g. 153.57
    char  paymentType[50];  // E.g. Visa
};

struct city {
	char name[50]; // E.g. Fort Worth
	int counter; // Track the current sales array position
	float sales[100]; // Temporary Upper Limit (Linked List Later)
    float totalSales;
};

struct purchaseRecord purchases[100];
int purchaseCount = 0;

struct city cities[100]; // Temporary Upper Limit (Linked List Later)
int cityCount = 0;

//2012-01-01    09:00    Fort Worth    Women's Clothing    153.57    Visa

int main(int argc, const char * argv[]) {

	// Read the txt File
	int lineCount = readPurchaseRecordFile();
	//printContentsOfPurchases(lineCount);

	// Print the total sales of the year.
    float totalSales = getTotalSales(lineCount);
	printTotalSales(totalSales);

	// Print the total number of cities
	updateCities(lineCount);
	printf("There are a total of %d cities.\n", cityCount);

    float avgSales = totalSales/cityCount;
    printf("The average sales from these %d cities are : %.2f\n", cityCount, avgSales);

    return 0;
}

int readPurchaseRecordFile() {
	    // Definitions
    const int SIZE = 255;
    char line[SIZE]; // String to store the current line.
    char *token;


    // Open the file
    FILE *fp = fopen("small_purchases.txt", "r"); // r stands for read mode, w for write, a for append.

    // FILE *output = fopen("output.txt", "w");

    int i = 0;
    
    // Copy the physical file into my array.
    while(fgets(line,SIZE,fp) !=NULL) {

        strcpy(purchases[i].date, strtok(line, "\t")); // Date
        strcpy(purchases[i].time, strtok(NULL, "\t")); // Time
        strcpy(purchases[i].city, strtok(NULL, "\t")); // City
        strcpy(purchases[i].itemCategory, strtok(NULL, "\t")); // Item Category
        purchases[i].salesValue = atof(strtok(NULL, "\t")); // Sales Value
        strcpy(purchases[i].paymentType, strtok(NULL, "\n")); // Payment Type
        
        i++;
    }
    i--;
    fclose(fp);

    return i;
}

void printContentsOfPurchases(int lineCount) {
	for (int a = 0; a <= lineCount; a++) {
    	printf("%s\n", purchases[a].date);
    	printf("%s\n", purchases[a].time);
    	printf("%s\n", purchases[a].city);
    	printf("%s\n", purchases[a].itemCategory);
    	printf("%f\n", purchases[a].salesValue);
    	printf("%s\n", purchases[a].paymentType);
    }
}

float getTotalSales(int lineCount) {
	float totalSales = 0.0;
	for (int a = 0; a <= lineCount; a++) {
		totalSales += purchases[a].salesValue;
	}
	return totalSales;
}

void printTotalSales(float totalSales) {
	printf("Total Sales of the year is: %.2f\n", totalSales);
}

int cityExists(char cityName[50], int limit) {
	for (int a = 0; a <= limit; a++) {
		if (strcmp(cityName, cities[a].name) == 0) {
			return 1;
		}
	}
	return 0;
}

void updateCitySales(char cityName[50], float salesValue) {
    for (int a = 0; a <= cityCount; a++) {
        if (strcmp(cityName, cities[a].name) == 0) {
            cities[a].sales[cities[a].counter] = salesValue;
            cities[a].totalSales += salesValue;
            cities[a].counter++;
            return;
        }
    }
}

void updateCities(int lineCount) {
    for (int a = 0; a <= lineCount; a++) {
        if (cityExists(purchases[a].city, lineCount) == 0) {
            strcpy(cities[cityCount].name, purchases[a].city);
            cityCount++;
        }

        updateCitySales(purchases[a].city, purchases[a].salesValue);
    }
}

/*
float printTopThree () {
    return totalSales
}*/






