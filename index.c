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
    printf("เลือกตัวเลือก:\n");
    printf("1. แสดงยอดคงเหลือทั้งหมด\n");
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

void displayAllDataOfFlavor(struct Product products[], int product_count, char *selected_flavor) {
    printf("ข้อมูลทั้งหมดของสินค้ารส %s:\n", selected_flavor);
    int found = 0;
    for (int i = 0; i < product_count; i++) {
        if (strcmp(products[i].name, selected_flavor) == 0) {
            printf("จำนวนที่นำเข้า: %d\n", products[i].quantity);
            found = 1;
        }
    }
    if (!found) {
        printf("ไม่พบข้อมูลสินค้ารส %s\n", selected_flavor);
    }
}

void updateProductData(struct Product products[], int product_count, char *selected_product) {
    printf("1. นำเข้าสินค้า\n");
    printf("2. นำออกสินค้า\n");
    printf("กรุณาเลือกรายการ: ");
    int update_choice;
    scanf("%d", &update_choice);

    switch (update_choice) {
        case 1:
            printf("กรุณาป้อนจำนวนสินค้าที่ต้องการนำเข้าสต็อกสำหรับ %s: ", selected_product);
            int quantity_import;
            scanf("%d", &quantity_import);
            for (int i = 0; i < product_count; i++) {
                if (strcmp(products[i].name, selected_product) == 0) {
                    products[i].quantity += quantity_import;
                    printf("นำเข้าสินค้าสำเร็จสำหรับ %s\n", selected_product);
                    return;
                }
            }
            printf("ไม่พบข้อมูลสินค้า %s\n", selected_product);
            break;
        case 2:
            printf("กรุณาป้อนจำนวนสินค้าที่ต้องการนำออกสต็อกสำหรับ %s: ", selected_product);
            int quantity_export;
            scanf("%d", &quantity_export);
            for (int i = 0; i < product_count; i++) {
                if (strcmp(products[i].name, selected_product) == 0) {
                    if (products[i].quantity >= quantity_export) {
                        products[i].quantity -= quantity_export;
                        printf("นำออกสินค้าสำเร็จสำหรับ %s\n", selected_product);
                        return;
                    } else {
                        printf("จำนวนสินค้าไม่เพียงพอสำหรับการนำออก\n");
                        return;
                    }
                }
            }
            printf("ไม่พบข้อมูลสินค้า %s\n", selected_product);
            break;
        default:
            printf("กรุณาเลือกรายการที่ถูกต้อง\n");
            break;
    }
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

    int choice;
    char selected_product[MAX_LINE_LENGTH];
    while (1) {
        displayMenu();
        printf("กรุณาเลือก: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                displayTotal(products, product_count);
                break;
            case 2:
                printf("รายการสินค้าทั้งหมด:\n");
                for (int i = 0; i < product_count; i++) {
                    printf("%d. %s\n", i + 1, products[i].name);
                }
                printf("กรุณาเลือกสินค้า: ");
                int selected_index;
                scanf("%d", &selected_index);
                if (selected_index > 0 && selected_index <= product_count) {
                    strcpy(selected_product, products[selected_index - 1].name);
                    displayAllDataOfFlavor(products, product_count, selected_product);
                } else {
                    printf("กรุณาเลือกตัวเลือกที่ถูกต้อง\n");
                }
                break;
            case 3:
                printf("รายการสินค้าทั้งหมด:\n");
                for (int i = 0; i < product_count; i++) {
                    printf("%d. %s\n", i + 1, products[i].name);
                }
                printf("กรุณาเลือกสินค้าที่ต้องการอัพเดท: ");
                int selected_index_update;
                scanf("%d", &selected_index_update);
                if (selected_index_update > 0 && selected_index_update <= product_count) {
                    strcpy(selected_product, products[selected_index_update - 1].name);
                    updateProductData(products, product_count, selected_product);
                } else {
                    printf("กรุณาเลือกตัวเลือกที่ถูกต้อง\n");
                }
                break;
            case 0:
                printf("ปิดโปรแกรม\n");
                return 0;
            default:
                printf("กรุณาเลือกตัวเลือกที่ถูกต้อง!\n");
        }
    }

    return 0;
}
