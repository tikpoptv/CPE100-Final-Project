#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 100
#define MAX_PRODUCTS 100

struct Product {
    char date[MAX_LINE_LENGTH];
    char code[MAX_LINE_LENGTH];
    char name[MAX_LINE_LENGTH];
    int quantity;
    char transactionType[MAX_LINE_LENGTH];
};

void displayMenu() {
    printf("เลือกตัวเลือก:\n");
    printf("1. แสดงยอดคงเหลือของสินค้าตามชื่อ\n");
    printf("2. แสดงข้อมูลทั้งหมดของสินค้ารสที่เลือก\n");
    printf("3. อัพเดทข้อมูลสินค้า\n");
    printf("0. ปิดโปรแกรม\n");
}

void displayTotal(struct Product products[], int product_count) {
    printf("สินค้าทั้งหมดที่คงเหลือ:\n");
    for (int i = 0; i < product_count; i++) {
        printf("%s: %d\n", products[i].name, products[i].quantity);
    }
}

// Function to display balance based on product name
void displayBalance(struct Product products[], int product_count) {
    // Code for displaying balance remains the same as previously shown
    // ...
}

// Function to update product data based on user input
void updateProductData(struct Product products[], int product_count, char *selected_product) {
    // Code for updating product data remains the same as previously shown
    // ...
}

int main() {
    FILE *file = fopen("data.csv", "r");

    if (file == NULL) {
        printf("ไม่สามารถเปิดไฟล์\n");
        return 1;
    }

    char line[MAX_LINE_LENGTH];
    fgets(line, MAX_LINE_LENGTH, file);

    struct Product products[MAX_PRODUCTS];
    int product_count = 0;

    while (fgets(line, MAX_LINE_LENGTH, file)) {
        char *token = strtok(line, ",");
        int count = 0;

        while (token != NULL) {
            if (count == 0) {
                strcpy(products[product_count].date, token);
            } else if (count == 1) {
                strcpy(products[product_count].code, token);
            } else if (count == 2) {
                strcpy(products[product_count].name, token);
            } else if (count == 3) {
                products[product_count].quantity = atoi(token);
            } else if (count == 4) {
                strcpy(products[product_count].transactionType, token);
            }

            token = strtok(NULL, ",");
            count++;
        }

        if (count == 5) {
            if (strcmp(products[product_count].transactionType, "Import") == 0) {
                // Increase quantity for imported products
                for (int i = 0; i < product_count; i++) {
                    if (strcmp(products[i].name, products[product_count].name) == 0) {
                        products[i].quantity += products[product_count].quantity;
                        break;
                    }
                }
            } else if (strcmp(products[product_count].transactionType, "Export") == 0) {
                // Decrease quantity for exported products
                for (int i = 0; i < product_count; i++) {
                    if (strcmp(products[i].name, products[product_count].name) == 0) {
                        products[i].quantity -= products[product_count].quantity;
                        break;
                    }
                }
            } else {
                printf("Invalid transaction type\n");
            }

            product_count++;
        }
    }

    fclose(file);

    int choice;
    while (1) {
        displayMenu();
        printf("กรุณาเลือก: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                displayBalance(products, product_count);
                break;
            case 2:
                // ... (Existing code for displaying all data of a selected flavor remains unchanged)
                break;
            case 3:
                // ... (Existing code for updating product data remains unchanged)
                break;
            case 0:
                printf("ปิดโปรแกรม\n");
                return 0;
            default:
                printf("กรุณาเลือกตัวเลือกที่ถูกต้อง\n");
        }
    }

    return 0;
}
