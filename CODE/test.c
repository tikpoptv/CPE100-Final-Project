#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<stdint.h>

//Define the Worst case scenario number
#define MAX_LINE_LENGTH 100
#define MAX_PRODUCTS 100
#define MAX_Product_ID 20

//Struct
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

//<-------------------Display funtions------------------------->
//display when start of the process
void displayMenu() {
    printf("<== Select features: ==>\n");
    printf("1. Display Products\n");
    printf("2. Update product\n");
    printf("3. Edit the transaction\n");
    printf("4. Display All Products\n");
    printf("0. Close Program\n");
}

//Display choices for summary
void displaySummary() {
    printf("<== Select features: ==>\n");
    printf("1. Amount Left in stock\n");
    printf("2. Summary of sales right now\n");
    printf("0. Close Program\n");
}

//Display choices of Ice Cream Menu
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

//To find the number of rows in the file
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
//To read the binary file from the path
void readFromBinaryFile(struct Product *products, int *count, const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("Error opening file for reading\n");
        return;
    }

    *count = fread(products, sizeof(struct Product), *count, file);

    fclose(file);
}
//Displaying all transactions in the file
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
//Check the year is a leap year or not
int isLeapYear(int year) {
    // Leap year is divisible by 4
    // Exception: Years divisible by 100 are not leap years, unless they are divisible by 400
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}
//Check the date is valid or not
int isValidDate(const char *date) {
    int day, month, year;
    if (sscanf(date, "%d/%d/%d", &day, &month, &year) != 3) {
        // Incorrect format
        return 0;
    }

    // Check year range
    if (year < 1) {
        return 0;
    }

    // Check month range
    if (month < 1 || month > 12) {
        return 0;
    }

    // Check day range based on the month
    int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // Adjust days in February for leap years
    if (month == 2 && isLeapYear(year)) {
        daysInMonth[2] = 29;
    }

    if (day < 1 || day > daysInMonth[month]) {
        return 0;
    }

    // If all checks pass, it's a valid date
    return 1;
}


//<------------------Display products in our file-------------------------->
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
    }
    // Free allocated memory
    for (int i = 0; i < MAX_Product_ID; i++) {
        free(strings[i]);
        free(name[i]);
    }
}


//<------------------Update a product in our file-------------------------->
void appendpro(struct Product *products) {
    // Open the binary file for append in binary mode
    FILE *file = fopen("IceCreambin.dat", "ab");  

    if (!file) {
        printf("Error opening file for appending\n");
        return;
    }

    struct Product update;//to keep the information in update and add to the file
    char date[20];
    //input the date
    printf("What's Date is today ?(Write in DD/MM/YYYY format) : ");
    scanf("%s",date);
    while (isValidDate(date) == 0) {
        printf("Please Input the date Correctly?(Write in DD/MM/YYYY format) : ");
        scanf("%s",date);
    }
    char a[9];
    //input the transaction type
    printf("In or Out or Cancel: ");
    scanf("%s", a);
    //In
    if (strcmp(a, "In") == 0) {
        strcpy(update.inout, a);
        strcpy(update.date,date);
        printf("Expired date : ");
        scanf("%s", update.expireD);
        while (isValidDate(update.expireD) == 0) {
            printf("Please Input the expire date Correctly?(Write in DD/MM/YYYY format) : ");
            scanf("%s",update.expireD);
        }
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
    } 
    //Out
    else if (strcmp(a, "Out") == 0) {
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
    } 
    //Cancel
    else if (strcmp(a, "Cancel") == 0) {
        printf("Canceled!!!\n");
    } 
    else {
        printf("Error Input! Please Try Again\n");
        appendpro(products);
    }

    fclose(file);
}

//Check the near expiration items in the stock
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

//Displaying Out of stock
void DisplayOutofStock(struct Product *products, int *row) {
    printf("Items Near Out of stock :\n");
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
    for (int i = 0; i < cnt; i++) {
        strcpy(sum[i].productID, strings[i]);
        strcpy(sum[i].name,name[i]);
    }
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
    int count=0;
    for(int i=0;i<*row;i++) {
        if(strcmp(sum[i].name,"Chocolate")==0 && sum[i].quantity <= 3) {
            printf("Chocolate is low on stock Amount left : %.2lf\n",sum[i].quantity);
            count++;
        }  
        else if(strcmp(sum[i].name,"Matcha")==0 && sum[i].quantity <= 3) {
            printf("Matcha is low on stock Amount left : %.2lf\n",sum[i].quantity);
            count++;
        }  
        else if(strcmp(sum[i].name,"Strawberry")==0 && sum[i].quantity <= 3) {
            printf("Strawberry is low on stock! Amount left : %.2lf\n",sum[i].quantity);
            count++;
        }  
        else if(strcmp(sum[i].name,"Cookies & Cream")==0 && sum[i].quantity <= 3) {
            printf("Cookies & Cream is low on stock! Amount left : %.2lf\n",sum[i].quantity);
            count++;
        }  
        else if(strcmp(sum[i].name,"Cone")==0 && sum[i].quantity <= 2) {
            printf("Cone is low on stock! Amount left : %.2lf\n",sum[i].quantity);
            count++;
        }  
        else if(strcmp(sum[i].name,"Cup")==0 && sum[i].quantity <= 2) {
            printf("Cup is low on stock! Amount left : %.2lf\n",sum[i].quantity);
            count++;
        } 
    }
    if(count == 0) printf("None\n");
}

int main() {
    int row=0,select;
    struct Product products[MAX_PRODUCTS];
    size_t recordsize = sizeof(struct Product);
    row = findNumberOfRecords("IceCreambin.dat",recordsize);
    
    readFromBinaryFile(products,&row,"IceCreambin.dat");
    system("clear|| cls");
    DisplayNearExpiration(products,&row);
    DisplayOutofStock(products,&row);
    displayMenu();
    scanf("%d",&select);

    if(select == 2) {
        appendpro(products);
        main();
    }
    else if(select == 1) {
        //system("clear|| cls");
        Display(products,&row);
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
    else if(select == 5) {
        adminLogin();
    }
    return 0;
}