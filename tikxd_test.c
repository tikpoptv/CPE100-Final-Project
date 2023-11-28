#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_LINE_LENGTH 100
#define MAX_PRODUCTS 100

struct Product {
    double time;
    char productID[MAX_LINE_LENGTH];
    char name[MAX_LINE_LENGTH];
    double quantity;
    int price;
};

void displayMenuWithTime() {
    displayCurrentTime();
    printf("<== Select features: ==>\n");
    printf("1. Display Products\n");
    printf("2. Update product\n");
    printf("3. Open Summary File\n");
    printf("4. Open Supply File\n");
    printf("5. End of the Day\n");
    printf("0. Close Program\n");
}

void readdaily(struct Product *products, int *column) {
    FILE* file = fopen("Ice Cream Shop(Daily).csv", "r");
    if (!file) {
        printf("Can't open file\n");
        return;
    }
    char line[MAX_LINE_LENGTH];
    fgets(line, MAX_LINE_LENGTH, file);
    *column = 0;
    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        char* value = strtok(line, ",");
        int cnt = 0;
        while (value != NULL) {
            if ((cnt % 5) == 0)
                products[*column].time = atof(value);
            else if ((cnt % 5) == 1)
                strcpy(products[*column].productID, value);
            else if ((cnt % 5) == 2)
                strcpy(products[*column].name, value);
            else if ((cnt % 5) == 3)
                products[*column].quantity = atof(value);
            else if ((cnt % 5) == 4)
                products[*column].price = atoi(value);
            cnt++;
            value = strtok(NULL, ",");
        }
        (*column)++;
    }
    fclose(file);
}

void appendpro(struct Product *products) {
    FILE* file = fopen("Ice Cream Shop(Daily).csv", "a");
    int want;
    printf("How many items : ");
    scanf("%d", &want);
    for (int i = 1; i <= want; i++) {
        struct Product update;
        printf("Time : ");
        scanf("%lf", &update.time);
        printf("ProductID : ");
        scanf("%s", update.productID);
        printf("Name : ");
        scanf("%s", update.name);
        printf("Amount : ");
        scanf("%lf", &update.quantity);
        printf("Price : ");
        scanf("%d", &update.price);
        fprintf(file, "\n%.2lf,%s,%s,%.2lf,%d", update.time, update.productID, update.name, update.quantity, update.price);
        printf("Successfully Updated!");
    }
    fclose(file);
}

void summary(struct Product *products, int *column) {
    struct Product sum[6];
    char *str[] = {"Chocolate", "Vanilla", "Cone", "Cup", "Strawberry", "Cookies & Cream"};
    for (int i = 0; i < 6; i++) {
        sum[i].price = 0;
        sum[i].quantity = 0;
        strcpy(sum[i].name, str[i]);
    }
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < *column; j++) {
            if (strcmp(sum[i].name, products[j].name) == 0) {
                sum[i].price += products[j].price;
                sum[i].quantity += products[j].quantity;
            }
        }
    }
    printf("Name  Price  Amount\n");
    for (int i = 0; i < 6; i++) {
        printf("%s  %d  %lf\n", sum[i].name, sum[i].price, sum[i].quantity);
    }
}

void displayCurrentTime() {
    time_t currentTime;
    struct tm *localTime;

    currentTime = time(NULL);
    localTime = localtime(&currentTime);

    printf("Current Time: %02d:%02d:%02d\n",
           localTime->tm_hour, localTime->tm_min, localTime->tm_sec);
}

int main() {
    struct Product products[MAX_PRODUCTS];
    int column = 0;
    displayMenuWithTime();
    readdaily(products, &column);
    int what;
    scanf("%d", &what);
    if (what == 2) {
        appendpro(products);
    } else if (what == 1) {
        summary(products, &column);
    }
    return 0;
}
