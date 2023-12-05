#include "../.hFile/Display_Products.h"

void openFile(const char *filename) {
    char command[200];
    sprintf(command, "open \"%s\"", filename);
    system(command);
}

void openFileFinder(const char *filename) {
    char command[200];
    sprintf(command, "open -R \"%s\"", filename);
    system(command);
}

void displayMenuAndGetChoice(const char *filename) {
    printf("\033[1;34mDo you want to openFile or openFileFinder this item?\033[0m\n");
    printf("\033[1;33m1. openFile\033[0m\n");
    printf("\033[1;33m2. openFileFinder\033[0m\n");
    printf("\033[1;31m3. Main Menu\033[0m\n");

    int action;
    printf("Enter your choice: ");
    scanf("%d", &action);

    switch (action) {
        case 1:
            openFile(filename);
            displayMenuAndGetChoice(filename);
            break;
        case 2:
            openFileFinder(filename);
            displayMenuAndGetChoice(filename);
            break;
        case 3:
            main();
            break; 
        default:
            printf("\033[1;31mInvalid choice.\033[0m\n");
            displayMenuAndGetChoice(filename);
            break;
    }
}



void generateCSV(struct Product *summary, int count) {
    char command[100];
    FILE *file;
    time_t now;
    struct tm *timestamp;
    char filename[200];
    char directory[200];

    time(&now);
    timestamp = localtime(&now);
    
    sprintf(directory, "../File/%04d-%02d-%02d/Amount_Left_in_stock", timestamp->tm_year + 1900, timestamp->tm_mon + 1, timestamp->tm_mday);
    mkdir(directory, 0777); // สร้างโฟลเดอร์เพื่อเก็บไฟล์ CSV ในกรณีที่ยังไม่ได้ถูกสร้าง

    printf("%s\n", directory); // แสดงที่อยู่ของไดเรกทอรี

    sprintf(filename, "%s/Amount_Left_in_stock_%02d-%02d-%02d.csv",
            directory, timestamp->tm_hour, timestamp->tm_min, timestamp->tm_sec);

    file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    fprintf(file, "Product ID, Name, Amount, Price\n");
    for (int i = 0; i < count; i++) {
        fprintf(file, "%s, %s, %.2lf, %d\n", summary[i].productID, summary[i].name, summary[i].quantity, summary[i].price);
    }

    fclose(file);
    displayMenuAndGetChoice(filename);

}


void generateSalesCSV(struct Product *summary, int count) {
     FILE *file;
    time_t now;
    struct tm *timestamp;
    char filename[200];
    char directory[200];

    time(&now);
    timestamp = localtime(&now);
    
    sprintf(directory, "../File/%04d-%02d-%02d/Summary_of_sales", timestamp->tm_year + 1900, timestamp->tm_mon + 1, timestamp->tm_mday);
    mkdir(directory, 0777); // สร้างโฟลเดอร์เพื่อเก็บไฟล์ CSV ในกรณีที่ยังไม่ได้ถูกสร้าง

    printf("%s\n", directory); // แสดงที่อยู่ของไดเรกทอรี

    sprintf(filename, "%s/Summary_of_sales_%02d:%02d:%02d.csv",
            directory, timestamp->tm_hour, timestamp->tm_min, timestamp->tm_sec);


    file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    int moneynow = 0;
    fprintf(file, "Product ID, Name, Amount, Price\n");
    for (int i = 0; i < count; i++) {
        fprintf(file, "%s, %s, %.2lf, %d\n", summary[i].productID, summary[i].name, summary[i].quantity, summary[i].price);
        moneynow += summary[i].price; // คำนวณรายได้ทั้งหมด
    }

    fprintf(file, "\nNow, the income is %d baht\n", moneynow); // เขียนผลลัพธ์รายได้ลงในไฟล์
    printf("Now, the income is %d baht\n", moneynow); // แสดงผลลัพธ์รายได้ทั้งหมดในโปรแกรม

    fclose(file);
    displayMenuAndGetChoice(filename);
}


void Display(struct Product *products, int *row) {
    char *strings[MAX_Product_ID];
    char *name[MAX_Product_ID];
    int cnt = 0;
    
    // Initialize pointers
    for (int i = 0; i < MAX_Product_ID; i++) {
        strings[i] = malloc(MAX_LINE_LENGTH);  // Adjust MAX_LINE_LENGTH according to your needs
        name[i] = malloc(MAX_LINE_LENGTH);
        if (strings[i] == NULL) {
            // Handle memory allocation failure
            exit(EXIT_FAILURE);
        }
    }

    // Copy unique strings
    for (int i = 0; i < *row; i++) {
        int isDuplicate = 0;
        for (int j = 0; j < cnt; j++) {
            if (strcmp(strings[j], products[i].productID) == 0) {
                isDuplicate = 1;
                break;
            }
        }
        if (!isDuplicate) {
            strcpy(strings[cnt], products[i].productID);
            strcpy(name[cnt],products[i].name);
            cnt++;
        }
    }

    struct Product sum[cnt];
    //set the price and quantity to be zero
    for(int i=0;i<cnt;i++ ) {
        sum[i].price=0;
        sum[i].quantity=0.0;
    }

    struct Product sum2[cnt];
    //set the price and quantity to be zero
    for(int i=0;i<cnt;i++ ) {
        sum2[i].price=0;
        sum2[i].quantity=0.0;
    }

    // Copy strings from strings to sum
    for (int i = 0; i < cnt; i++) {
        strcpy(sum[i].productID, strings[i]);
        strcpy(sum[i].name,name[i]);
        strcpy(sum2[i].productID, strings[i]);
        strcpy(sum2[i].name,name[i]);
    }

    displaySummary();//display choices of display summary
    int want;
    scanf("%d",&want);
    //Show the amount left in stock
    if(want == 1) {
        for(int i=0;i<cnt;i++) {
            for(int j=0;j<*row;j++) {
                if(strcmp(sum[i].productID,products[j].productID) == 0) {
                    if(strcmp(products[j].inout, "In") == 0) {
                        sum[i].price += products[j].price;
                        sum[i].quantity += products[j].quantity;
                    }
                    else if(strcmp(products[j].inout, "Out") == 0) {
                        sum[i].price -= products[j].price;
                        sum[i].quantity -= products[j].quantity;
                    }
                }
            }
        }
        printf("Product ID\tName\t       Amount\tPrice\n");
        for(int i=0;i<cnt;i++) {
            printf("%-12s\t%-12s\t%.2lf\t%d\n",sum[i].productID,sum[i].name,sum[i].quantity,sum[i].price);
        }

        generateCSV(sum, cnt);
    }
    //Show the summary of sales
    else if(want == 2) {
        int moneynow = 0;
        for(int i=0;i<cnt;i++) {
            for(int j=0;j<*row;j++) {
                if(strcmp(sum[i].productID,products[j].productID) == 0) {
                    if(strcmp(products[j].inout, "Out") == 0) {
                        sum2[i].price += products[j].price;
                        sum2[i].quantity += products[j].quantity;
                    }
                }
            }
        }
        printf("Product ID\tName\t       Amount\tPrice\n");
        for(int i=0;i<cnt;i++) {
            printf("%-12s\t%-12s\t%.2lf\t%d\n",sum2[i].productID,sum2[i].name,sum2[i].quantity,sum2[i].price);
        }
        for(int i=0;i<cnt;i++) {
            moneynow+=sum2[i].price;
        }
        printf("Now, the income is %d baht\n",moneynow);
        generateSalesCSV(sum2, cnt);
    }
    // Free allocated memory
    for (int i = 0; i < MAX_Product_ID; i++) {
        free(strings[i]);
        free(name[i]);
    }
}