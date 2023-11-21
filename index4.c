#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 100
#define MAX_PRODUCTS 100

struct Product {
    char name[MAX_LINE_LENGTH];
    int quantity;
};

void displayMenu() {
    printf("Select an option:\n");
    printf("1. Display total remaining\n");
    printf("2. Display all data of selected flavor\n");
    printf("3. Update product data\n");
    printf("0. Exit program\n");
}

void displayTotal(struct Product products[], int product_count) {
    printf("All remaining products:\n");
    for (int i = 0; i < product_count; i++) {
        printf("%s: %d\n", products[i].name, products[i].quantity);
    }
}

void displayAllDataOfFlavor(struct Product products[], int product_count, char *selected_flavor) {
    printf("All data of flavor %s:\n", selected_flavor);
    int found = 0;
    for (int i = 0; i < product_count; i++) {
        if (strcmp(products[i].name, selected_flavor) == 0) {
            printf("Quantity imported: %d\n", products[i].quantity);
            found = 1;
        }
    }
    if (!found) {
        printf("No data found for flavor %s\n", selected_flavor);
    }
}

void updateProductData(struct Product products[], int product_count, char *selected_product) {
    printf("1. Import product\n");
    printf("2. Export product\n");
    printf("Please select an option: ");
    int update_choice;
    scanf("%d", &update_choice);

    switch (update_choice) {
        case 1:
            printf("Enter quantity to import for %s: ", selected_product);
            int quantity_import;
            scanf("%d", &quantity_import);
            for (int i = 0; i < product_count; i++) {
                if (strcmp(products[i].name, selected_product) == 0) {
                    products[i].quantity += quantity_import;
                    printf("Product imported successfully for %s\n", selected_product);
                    return;
                }
            }
            printf("Product %s not found\n", selected_product);
            break;
        case 2:
            printf("Enter quantity to export for %s: ", selected_product);
            int quantity_export;
            scanf("%d", &quantity_export);
            for (int i = 0; i < product_count; i++) {
                if (strcmp(products[i].name, selected_product) == 0) {
                    if (products[i].quantity >= quantity_export) {
                        products[i].quantity -= quantity_export;
                        printf("Product exported successfully for %s\n", selected_product);
                        return;
                    } else {
                        printf("Insufficient quantity for export\n");
                        return;
                    }
                }
            }
            printf("Product %s not found\n", selected_product);
            break;
        default:
            printf("Please select a valid option\n");
            break;
    }
}

void saveToCSV(struct Product products[], int product_count) {
    FILE *file = fopen("data.csv", "w");
    
    if (file == NULL) {
        printf("Cannot open file\n");
        return;
    }
    
    fprintf(file, "Import Date,Product Name,Quantity\n");
    for (int i = 0; i < product_count; i++) {
        fprintf(file, "2023-11-21,%s,%d\n", products[i].name, products[i].quantity);
    }
    
    fclose(file);
    printf("Data saved successfully\n");
}

// The rest of the code reads CSV data, manages product data, and interacts with the user through a menu system.

// (โค้ดอื่นๆ ที่มีอยู่)
int main() {
    FILE *file = fopen("data.csv", "r");

    if (file == NULL) {
        printf("Unable to open file\n");
        return 1;
    }

    char line[MAX_LINE_LENGTH];
    fgets(line, MAX_LINE_LENGTH, file);

    struct Product products[MAX_PRODUCTS];
    int product_count = 0;

    while (fgets(line, MAX_LINE_LENGTH, file)) {
        char *token = strtok(line, ",");
        char *product_name;
        int quantity = -1;
        int count = 0;

        while (token != NULL) {
            if (count == 1) {
                product_name = token;
            } else if (count == 2) {
                quantity = atoi(token);
            }

            token = strtok(NULL, ",");
            count++;
        }

        if (quantity != -1) {
            int found = 0;
            for (int i = 0; i < product_count; i++) {
                if (strcmp(products[i].name, product_name) == 0) {
                    products[i].quantity += quantity;
                    found = 1;
                    break;
                }
            }

            if (!found) {
                strcpy(products[product_count].name, product_name);
                products[product_count].quantity = quantity;
                product_count++;
            }
        }
    }

    fclose(file);

    // Menu system for interacting with product data
    int choice;
    char selected_product[MAX_LINE_LENGTH];
    while (1) {
        displayMenu();
        printf("Please select: ");
        scanf("%d", &choice);

        switch (choice) {
            // Cases for different menu options (already explained in the previous code snippet)
            // ...
            // (Code for handling different menu choices)
            // ...
            default:
                printf("Please select a valid option\n");
                break;
        }
    }

    return 0;
}

