#ifndef Display_Products_H
#define Display_Products_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>

#define MAX_Product_ID 100
#define MAX_LINE_LENGTH 100

struct Product {
    char productID[MAX_LINE_LENGTH];
    char name[MAX_LINE_LENGTH];
    double quantity;
    int price;
    char inout[MAX_LINE_LENGTH];
};

void openFile(const char *filename);
void openFileFinder(const char *filename);
void displayMenuAndGetChoice(const char *filename);
void generateCSV(struct Product *summary, int count);
void generateSalesCSV(struct Product *summary, int count);
void Display(struct Product *products, int *row);

#endif /* FUNCTIONS_H */
