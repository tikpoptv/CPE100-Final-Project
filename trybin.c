#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

//Define the Worst case scenario number
#define MAX_LINE_LENGTH 100
#define MAX_PRODUCTS 100
#define MAX_Product_ID 18

struct Product {
    char inout[MAX_LINE_LENGTH];
    char date[MAX_LINE_LENGTH];
    char expireD[MAX_LINE_LENGTH];
    double time;
    char productID[MAX_LINE_LENGTH];
    char name[MAX_LINE_LENGTH];
    double quantity;
    int price;
};

//display when start of the process
void displayMenu() {
    printf("<== Select features: ==>\n");
    printf("1. Display Products\n");
    printf("2. Update product\n");
    printf("3. Edit the transaction\n");
    printf("4. Display All Products\n");
    printf("0. Close Program\n");
}

void displaySummary() {
    printf("<== Select features: ==>\n");
    printf("1. Amount Left in stock\n");
    printf("2. Summary of sales right now\n");
    printf("0. Close Program\n");
}

void displayIceCreamMenu() {
    printf("<== Select features: ==>\n");
    printf("1. Cone Chocolate\n");
    printf("2. Cone Matcha\n");
    printf("3. Cone Strawberry\n");
    printf("4. Cone Cookies & Cream\n");
    printf("5. Cup Chocolate\n");
    printf("6. Cup Matcha\n");
    printf("7. Cup Strawberry\n");
    printf("8. Cup Cookies & Cream\n");
    printf("0. Close Program\n");
}

int findNumberOfRecords(const char *filename, size_t recordSize) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("Error opening file for reading\n");
        return -1;  // Return -1 to indicate an error
    }

    // Find the number of records in the file
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    int numberOfRecords = fileSize / recordSize;
    fseek(file, 0, SEEK_SET);

    fclose(file);

    return numberOfRecords;
}

void readFromBinaryFile(struct Product *products, int *count, const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("Error opening file for reading\n");
        return;
    }

    *count = fread(products, sizeof(struct Product), *count, file);

    fclose(file);
}

void displayAll(struct Product *products, int *row) {
    printf("Displaying all products:\n");
    printf("No. In/Out Date       Expired Date Time Product ID Name         Amount Price\n");

    for (int i = 0; i < *row; i++) {
        printf("%-4d %-6s %-10s %-12s %.2lf %-11s %-12s %-6.2lf %d\n", 
            i + 1, products[i].inout, products[i].date, products[i].expireD, 
            products[i].time, products[i].productID, products[i].name, 
            products[i].quantity, products[i].price);
    }
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

    for(int i=0;i<cnt;i++ ) {
        sum[i].price=0;
        sum[i].quantity=0.0;
    }

    struct Product sum2[cnt];

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

    displaySummary();
    int want;
    scanf("%d",&want);
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
    }

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
    }
    // Free allocated memory
    for (int i = 0; i < MAX_Product_ID; i++) {
        free(strings[i]);
        free(name[i]);
    }
}


//Update a product in Daily file
void appendpro(struct Product *products, char date[20]) {
    FILE *file = fopen("Database/IceCreambin.dat", "ab");  // Open the binary file for appending in binary mode

    if (!file) {
        printf("Error opening file for appending\n");
        return;
    }

    struct Product update;

    char a[9];
    printf("In or Out or Cancel: ");
    scanf("%s", a);

    if (strcmp(a, "In") == 0) {
        strcpy(update.inout, a);
        printf("Date : ");
        scanf("%s", update.date);
        printf("Expired date : ");
        scanf("%s", update.expireD);
        printf("Time : ");
        scanf("%lf", &update.time);
        printf("Product ID : ");
        scanf("%s", update.productID);
        printf("Name : ");
        scanf("%s", update.name);
        printf("Amount : ");
        scanf("%lf", &update.quantity);
        printf("Price : ");
        scanf("%d", &update.price);

        // Write the new product to the end of the binary file
        fwrite(&update, sizeof(struct Product), 1, file);
    } else if (strcmp(a, "Out") == 0) {
        displayIceCreamMenu();
        int inp;
        printf("Input your Menu : ");
        scanf("%d",&inp);
        if(inp==1) { //Cone Chocolate
            //Chocolate Flavor
            strcpy(update.inout, a);
            strcpy(update.date,date);
            strcpy(update.expireD,"null");
            printf("Time : ");
            scanf("%lf", &update.time);
            printf("Product ID of flavor: ");
            scanf("%s", update.productID);
            strcpy(update.name,"Chocolate");
            update.quantity = 0.02;
            update.price = 50;
            fwrite(&update, sizeof(struct Product), 1, file);
            //Cone
            strcpy(update.inout, a);
            strcpy(update.date,date);
            strcpy(update.expireD,"null");
            printf("Product ID of Cone: ");
            scanf("%s", update.productID);
            strcpy(update.name,"Cone");
            update.quantity = 0.02;
            update.price = 9;
            fwrite(&update, sizeof(struct Product), 1, file);
        }
        else if(inp==2) { //Cone Matcha
            //Flavor
            strcpy(update.inout, a);
            strcpy(update.date,date);
            strcpy(update.expireD,"null");
            printf("Time : ");
            scanf("%lf", &update.time);
            printf("Product ID of flavor: ");
            scanf("%s", update.productID);
            strcpy(update.name,"Matcha");
            update.quantity = 0.02;
            update.price = 50;
            fwrite(&update, sizeof(struct Product), 1, file);
            //Cone
            strcpy(update.inout, a);
            strcpy(update.date,date);
            strcpy(update.expireD,"null");
            printf("Product ID of Cone: ");
            scanf("%s", update.productID);
            strcpy(update.name,"Cone");
            update.quantity = 0.02;
            update.price = 9;
            fwrite(&update, sizeof(struct Product), 1, file);
        }
        else if(inp==3) { //Cone Strawberry
            //Flavor
            strcpy(update.inout, a);
            strcpy(update.date,date);
            strcpy(update.expireD,"null");
            printf("Time : ");
            scanf("%lf", &update.time);
            printf("Product ID of flavor: ");
            scanf("%s", update.productID);
            strcpy(update.name,"Strawberry");
            update.quantity = 0.02;
            update.price = 50;
            fwrite(&update, sizeof(struct Product), 1, file);
            //Cone
            strcpy(update.inout, a);
            strcpy(update.date,date);
            strcpy(update.expireD,"null");
            printf("Product ID of Cone: ");
            scanf("%s", update.productID);
            strcpy(update.name,"Cone");
            update.quantity = 0.02;
            update.price = 9;
            fwrite(&update, sizeof(struct Product), 1, file);
        }
        else if(inp==4) { //Cone Cookies & Cream
            //Flavor
            strcpy(update.inout, a);
            strcpy(update.date,date);
            strcpy(update.expireD,"null");
            printf("Time : ");
            scanf("%lf", &update.time);
            printf("Product ID of flavor: ");
            scanf("%s", update.productID);
            strcpy(update.name,"Cookies & Cream");
            update.quantity = 0.02;
            update.price = 50;
            fwrite(&update, sizeof(struct Product), 1, file);
            //Cone
            strcpy(update.inout, a);
            strcpy(update.date,date);
            strcpy(update.expireD,"null");
            printf("Product ID of Cone: ");
            scanf("%s", update.productID);
            strcpy(update.name,"Cone");
            update.quantity = 0.02;
            update.price = 9;
            fwrite(&update, sizeof(struct Product), 1, file);
        }
        else if(inp==5) { //Cup Chocolate
            //Flavor
            strcpy(update.inout, a);
            strcpy(update.date,date);
            strcpy(update.expireD,"null");
            printf("Time : ");
            scanf("%lf", &update.time);
            printf("Product ID of flavor: ");
            scanf("%s", update.productID);
            strcpy(update.name,"Chocolate");
            update.quantity = 0.04;
            update.price = 100;
            fwrite(&update, sizeof(struct Product), 1, file);
            //Cup
            strcpy(update.inout, a);
            strcpy(update.date,date);
            strcpy(update.expireD,"null");
            printf("Product ID of Cup: ");
            scanf("%s", update.productID);
            strcpy(update.name,"Cup");
            update.quantity = 0.02;
            update.price = 9;
            fwrite(&update, sizeof(struct Product), 1, file);
        }
        else if(inp==6) { //Cup Matcha
            //Flavor
            strcpy(update.inout, a);
            strcpy(update.date,date);
            strcpy(update.expireD,"null");
            printf("Time : ");
            scanf("%lf", &update.time);
            printf("Product ID of flavor: ");
            scanf("%s", update.productID);
            strcpy(update.name,"Matcha");
            update.quantity = 0.04;
            update.price = 100;
            fwrite(&update, sizeof(struct Product), 1, file);
            //Cup
            strcpy(update.inout, a);
            strcpy(update.date,date);
            strcpy(update.expireD,"null");
            printf("Product ID of Cup: ");
            scanf("%s", update.productID);
            strcpy(update.name,"Cup");
            update.quantity = 0.02;
            update.price = 9;
            fwrite(&update, sizeof(struct Product), 1, file);
        }
        else if(inp==7) {//Cup Strawberry
            //Flavor
            strcpy(update.inout, a);
            strcpy(update.date,date);
            strcpy(update.expireD,"null");
            printf("Time : ");
            scanf("%lf", &update.time);
            printf("Product ID of flavor: ");
            scanf("%s", update.productID);
            strcpy(update.name,"Strawberry");
            update.quantity = 0.04;
            update.price = 100;
            fwrite(&update, sizeof(struct Product), 1, file);
            //Cup
            strcpy(update.inout, a);
            strcpy(update.date,date);
            strcpy(update.expireD,"null");
            printf("Product ID of Cup: ");
            scanf("%s", update.productID);
            strcpy(update.name,"Cup");
            update.quantity = 0.02;
            update.price = 9;
            fwrite(&update, sizeof(struct Product), 1, file);
        }
        else if(inp==8) {//Cup Cookies & Cream
            //Flavor
            strcpy(update.inout, a);
            strcpy(update.date,date);
            strcpy(update.expireD,"null");
            printf("Time : ");
            scanf("%lf", &update.time);
            printf("Product ID of flavor: ");
            scanf("%s", update.productID);
            strcpy(update.name,"Cookies & Cream");
            update.quantity = 0.04;
            update.price = 100;
            fwrite(&update, sizeof(struct Product), 1, file);
            //Cup
            strcpy(update.inout, a);
            strcpy(update.date,date);
            strcpy(update.expireD,"null");
            printf("Product ID of Cup: ");
            scanf("%s", update.productID);
            strcpy(update.name,"Cup");
            update.quantity = 0.02;
            update.price = 9;
            fwrite(&update, sizeof(struct Product), 1, file);
        }
    } else if (strcmp(a, "Cancel") == 0) {
        printf("Canceled!!!\n");
    } else {
        printf("Error Input! Please Try Again\n");
        appendpro(products, date);
    }

    fclose(file);
}

void updateCSV(struct Product *products, int row) {
    FILE *file = fopen("Ice_Cream_Shop.csv", "w");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    fprintf(file, "In/Out,Date,Expired Date,Time,Product ID,Name,Amount,Price\n");

   
    for (int i = 0; i < row; i++) {
        fprintf(file, "%s,%s,%s,%.2lf,%s,%s,%.2lf,%d\n", products[i].inout, products[i].date, products[i].expireD, products[i].time, products[i].productID, products[i].name, products[i].quantity, products[i].price);
    }

    fclose(file);
}

void updateBinary(struct Product *products, int row) {
    FILE *binaryFile = fopen("Database/IceCreambin.dat", "wb");

    if (binaryFile == NULL) {
        printf("Error opening binary file for writing.\n");
        return;
    }

    // Write the header or any necessary information for the binary file (if needed)
    // For instance:
    // fwrite(&someHeaderInfo, sizeof(SomeHeaderType), 1, binaryFile);

    // Write each product to the binary file
    fwrite(products, sizeof(struct Product), row, binaryFile);

    fclose(binaryFile);
}


void editTransaction(struct Product *products, int row) {
    int selection;

    printf("Products List:\n");
    printf("No.  In/Out  Date       Expired Date  Time  Product ID  Name         Amount  Price\n");
    for (int i = 0; i < row; i++) {
        printf("%-5d %-7s %-10s %-12s %.2lf %-11s %-12s %-7.2lf %d\n", i + 1, products[i].inout, products[i].date, products[i].expireD, products[i].time, products[i].productID, products[i].name, products[i].quantity, products[i].price);
    }

    printf("Select the item number to edit or delete: ");
    scanf("%d", &selection);
    selection--; 

    if (selection < 0 || selection >= row) {
        printf("Invalid selection.\n");
        return;
    }

    printf("\033[1;34mDo you want to edit or delete this item?\033[0m\n");
    printf("\033[1;33m1. EDIT\033[0m\n");
    printf("\033[1;31m2. DELETE\033[0m\n");

    int action;
    printf("Enter your choice: ");
    scanf("%d", &action);

    switch (action) {
        case 1:
            // To edit an item
            printf("Enter new quantity: ");
            scanf("%lf", &products[selection].quantity);
            printf("Enter new price: ");
            scanf("%d", &products[selection].price);

            printf("Updated transaction:\n");
            printf("In/Out  Date       Expired Date  Time  Product ID  Name         Amount  Price\n");
            printf("%-7s %-10s %-12s %.2lf %-11s %-12s %-7.2lf %d\n", products[selection].inout, products[selection].date, products[selection].expireD, products[selection].time, products[selection].productID, products[selection].name, products[selection].quantity, products[selection].price);

            char confirm;
            printf("Confirm changes? (Y/N): ");
            scanf(" %c", &confirm);

            if (confirm == 'Y' || confirm == 'y') {
                updateBinary(products, row);
            } else {
                printf("Changes discarded.\n");
            }
            break;
        case 2:
            // To delete an item
            for (int i = selection; i < row - 1; ++i) {
                products[i] = products[i + 1];
            }
            row--;

            char confirmDelete;
            printf("\033[1;31mConfirm deletion? (Y/N): \033[0m");
            scanf(" %c", &confirmDelete);

            if (confirmDelete == 'Y' || confirmDelete == 'y') {
                updateBinary(products, row);
            } else {
                printf("Deletion discarded.\n");
            }
            break;
        default:
            printf("\033[1;31mInvalid choice.\033[0m\n");
            break;
    }
}

void DisplayNearExpiration(struct Product *products, int *row) {
    printf("Items Near Expiration (within 30 days):\n");
    printf("Product ID\tName\t       Expired Date\n");

    // Get the current time
    time_t currentTime;
    time(&currentTime);

    // Assuming the date format is DD/MM/YYYY
    struct tm expirationDate;

    for (int i = 0; i < *row; i++) {
        if (strcmp(products[i].expireD, "null") != 0) {
            // Convert the string expiration date to a tm struct
            strptime(products[i].expireD, "%d/%m/%Y", &expirationDate);

            // Convert tm struct to time_t
            time_t expirationTime = mktime(&expirationDate);

            // Calculate the difference in seconds
            double difference = difftime(expirationTime, currentTime);

            // Calculate the difference in days
            double differenceInDays = difference / (60 * 60 * 24);

            // Check if the item is within 30 days of expiration
            if (differenceInDays < 30) {
                printf("%-12s\t%-12s\t%s\n", products[i].productID, products[i].name, products[i].expireD);
            }
        }
    }
}



int main() {
    int row=0,select;
    char date[20];
    struct Product products[MAX_PRODUCTS];
    size_t recordsize = sizeof(struct Product);
    row = findNumberOfRecords("Database/IceCreambin.dat",recordsize);
    readFromBinaryFile(products,&row,"Database/IceCreambin.dat");
    system("clear|| cls");
    DisplayNearExpiration(products,&row);
    displayMenu();
    scanf("%d",&select);
    if(select == 2) {
        appendpro(products,date);
        main();
    }
    else if(select == 1) {
        //system("clear|| cls");
        Display(products,&row);
        //main();
    }
    else if(select == 3) {
        system("clear || cls");
        editTransaction(products, row);
        //selectDateAndDisplay(products, &row);
        system("clear || cls");
        main();
    }
    else if(select == 0) {
        system("clear || cls");
        printf("Closed the Program\n");
    }
    else if(select == 4) {
        printf("Selected option 4. Displaying all products...\n");
        displayAll(products, &row);
    }
    return 0;
}