#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <sys/stat.h>

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

void displayMenuAndGetChoice(const char *filename);

//<-------------------Display funtions------------------------->
//display when start of the process
void displayMenu() {
    printf("\x1b[94m\n<== Select features: ==>\n\x1b[0m");
    printf("1. Display Products\n");
    printf("2. Update product\n");
    printf("3. Edit the transaction\n");
    printf("4. Display All Products\n");
    printf("0. Close Program\n");
}

//Display choices for summary
void displaySummary() {
    printf("\x1b[94m<== Select features: ==>\n\x1b[0m");
    printf("1. Amount Left in stock\n");
    printf("2. Summary of sales right now\n");
    printf("0. Close Program\n");
}

//Display choices of Ice Cream Menu
void displayIceCreamMenu() {
    printf("\x1b[94m<== Select features: ==>\n\x1b[0m");
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
void generateCSVAll(struct Product *products, int *row) {
    FILE *file;
    time_t now;
    struct tm *timestamp;
    char filename[200];
    char directory[200];
    char name[200];

    time(&now);
    timestamp = localtime(&now);
    
    sprintf(directory, "../File/%04d-%02d-%02d", timestamp->tm_year + 1900, timestamp->tm_mon + 1, timestamp->tm_mday);
    mkdir(directory, 0777); 

    sprintf(name, "%s/All_Products", directory);
    mkdir(name, 0777); 

   // printf("%s\n", directory);

    sprintf(filename, "%s/All_Products_%04d_%02d_%02d:%02d-%02d-%02d.csv",
            name, 
            timestamp->tm_year + 1900, timestamp->tm_mon + 1, timestamp->tm_mday,
            timestamp->tm_hour, timestamp->tm_min, timestamp->tm_sec);

    file = fopen(filename, "w");
    if (file == NULL) {
    perror("Error opening file");
    return;
    }else{
        printf("\x1b[93m\nThe csv file has been created!! \n\n\x1b[0m");
    }

    fprintf(file, "No., In/Out, Date, Expired Date, Time, Product ID, Name, Amount, Price\n");
    for (int i = 0; i < *row; i++) {
        fprintf(file, "%-4d, %-6s, %-10s, %-12s, %.2lf, %-11s, %-12s, %-6.2lf, %d\n", 
            i + 1, products[i].inout, products[i].date, products[i].expireD, 
            products[i].time, products[i].productID, products[i].name, 
            products[i].quantity, products[i].price);
    }

    fclose(file);
    displayMenuAndGetChoice(filename);
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
    generateCSVAll(products, row);

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

void generateCSV(struct Product *summary, int count) {
    char command[100];
    FILE *file;
    time_t now;
    struct tm *timestamp;
    char filename[200];
    char directory[200];
    char name[200];

    time(&now);
    timestamp = localtime(&now);
    
    sprintf(directory, "../File/%04d-%02d-%02d", timestamp->tm_year + 1900, timestamp->tm_mon + 1, timestamp->tm_mday);
    mkdir(directory, 0777); 

    sprintf(name, "%s/Amount_Left_in_stock", directory);
    mkdir(name, 0777); 

  //  printf("%s\n", directory); 

    sprintf(filename, "%s/Amount_Left_in_stock_%04d_%02d_%02d:%02d-%02d-%02d.csv",
            name, 
            timestamp->tm_year + 1900, timestamp->tm_mon + 1, timestamp->tm_mday,
            timestamp->tm_hour, timestamp->tm_min, timestamp->tm_sec);

    file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }else{
        printf("\x1b[93m\nThe csv file has been created!! \n\n\x1b[0m");
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
    char name[200];

    time(&now);
    timestamp = localtime(&now);
    
    sprintf(directory, "../File/%04d-%02d-%02d", timestamp->tm_year + 1900, timestamp->tm_mon + 1, timestamp->tm_mday);
    mkdir(directory, 0777); 

    sprintf(name, "%s/Summary_of_sales", directory);
    mkdir(name, 0777); 

   // printf("%s\n", directory);

    sprintf(filename, "%s/Summary_of_sales_%04d_%02d_%02d:%02d-%02d-%02d.csv",
            name, 
            timestamp->tm_year + 1900, timestamp->tm_mon + 1, timestamp->tm_mday,
            timestamp->tm_hour, timestamp->tm_min, timestamp->tm_sec);


    //Summary_of_sales_

    file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }else{
        printf("\x1b[93m\nThe csv file has been created!! \n\n\x1b[0m");
    }

    int moneynow = 0;
    fprintf(file, "Product ID, Name, Amount, Price\n");
    for (int i = 0; i < count; i++) {
        fprintf(file, "%s, %s, %.2lf, %d\n", summary[i].productID, summary[i].name, summary[i].quantity, summary[i].price);
        moneynow += summary[i].price; // คำนวณรายได้ทั้งหมด
    }

    fprintf(file, "\nNow, the income is %d baht\n", moneynow); // เขียนผลลัพธ์รายได้ลงในไฟล์

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
        system("clear || cls");
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
        system("clear || cls");
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


//<------------------Update a product in our file-------------------------->
void appendpro(struct Product *products) {
    // Open the binary file for append in binary mode
    FILE *file = fopen("../Database/IceCreambin.dat", "ab");  

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

//--------------------------------------------- Start of TIKPOPTV ---------------------------------------------//
//Editing and Deleting the transaction in FILE
#define MAX_PASSWORD_LENGTH 50
#define ROTRIGHT(word, bits) (((word) >> (bits)) | ((word) << (32 - (bits))))
#define ROTLEFT(word, bits) (((word) << (bits)) | ((word) >> (32 - (bits))))
#define CH(x, y, z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x, y, z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
//Using SHA256 to keep the password
#define SHA256_BLOCK_SIZE 64
#define SHA256_DIGEST_LENGTH 32

typedef struct {
    uint8_t data[64];
    uint32_t datalen;
    uint64_t bitlen;
    uint32_t state[8];
} SHA256_CTX;

uint32_t sig0(uint32_t x) {
    return (ROTRIGHT(x, 7) ^ ROTRIGHT(x, 18) ^ (x >> 3));
}

uint32_t sig1(uint32_t x) {
    return (ROTRIGHT(x, 17) ^ ROTRIGHT(x, 19) ^ (x >> 10));
}

uint32_t SIG0(uint32_t x) {
    return (ROTRIGHT(x, 2) ^ ROTRIGHT(x, 13) ^ ROTRIGHT(x, 22));
}

uint32_t SIG1(uint32_t x) {
    return (ROTRIGHT(x, 6) ^ ROTRIGHT(x, 11) ^ ROTRIGHT(x, 25));
}

const uint32_t k[] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

void sha256_transform(SHA256_CTX *ctx, const uint8_t data[]) {
    uint32_t a, b, c, d, e, f, g, h, i, j, t1, t2, m[64];

    for (i = 0, j = 0; i < 16; ++i, j += 4)
        m[i] = (data[j] << 24) | (data[j + 1] << 16) | (data[j + 2] << 8) | (data[j + 3]);

    for (; i < 64; ++i)
        m[i] = sig1(m[i - 2]) + m[i - 7] + sig0(m[i - 15]) + m[i - 16];

    a = ctx->state[0];
    b = ctx->state[1];
    c = ctx->state[2];
    d = ctx->state[3];
    e = ctx->state[4];
    f = ctx->state[5];
    g = ctx->state[6];
    h = ctx->state[7];

    for (i = 0; i < 64; ++i) {
        t1 = h + SIG1(e) + CH(e, f, g) + k[i] + m[i];
        t2 = SIG0(a) + MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;
    }

    ctx->state[0] += a;
    ctx->state[1] += b;
    ctx->state[2] += c;
    ctx->state[3] += d;
    ctx->state[4] += e;
    ctx->state[5] += f;
    ctx->state[6] += g;
    ctx->state[7] += h;
}

void sha256_init(SHA256_CTX *ctx) {
    ctx->datalen = 0;
    ctx->bitlen = 0;
    ctx->state[0] = 0x6a09e667;
    ctx->state[1] = 0xbb67ae85;
    ctx->state[2] = 0x3c6ef372;
    ctx->state[3] = 0xa54ff53a;
    ctx->state[4] = 0x510e527f;
    ctx->state[5] = 0x9b05688c;
    ctx->state[6] = 0x1f83d9ab;
    ctx->state[7] = 0x5be0cd19;
}

void sha256_update(SHA256_CTX *ctx, const uint8_t data[], size_t len) {
    for (size_t i = 0; i < len; ++i) {
        ctx->data[ctx->datalen] = data[i];
        ctx->datalen++;
        if (ctx->datalen == 64) {
            sha256_transform(ctx, ctx->data);
            ctx->bitlen += 512;
            ctx->datalen = 0;
        }
    }
}

void sha256_final(SHA256_CTX *ctx, uint8_t hash[]) {
    uint32_t i = ctx->datalen;

    if (ctx->datalen < 56) {
        ctx->data[i++] = 0x80;
        while (i < 56)
            ctx->data[i++] = 0x00;
    } else {
        ctx->data[i++] = 0x80;
        while (i < 64)
            ctx->data[i++] = 0x00;
        sha256_transform(ctx, ctx->data);
        memset(ctx->data, 0, 56);
    }

    ctx->bitlen += ctx->datalen * 8;
    ctx->data[63] = ctx->bitlen;
    ctx->data[62] = ctx->bitlen >> 8;
    ctx->data[61] = ctx->bitlen >> 16;
    ctx->data[60] = ctx->bitlen >> 24;
    ctx->data[59] = ctx->bitlen >> 32;
    ctx->data[58] = ctx->bitlen >> 40;
    ctx->data[57] = ctx->bitlen >> 48;
    ctx->data[56] = ctx->bitlen >> 56;
    sha256_transform(ctx, ctx->data);

    for (i = 0; i < 4; ++i) {
        hash[i] = (ctx->state[0] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 4] = (ctx->state[1] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 8] = (ctx->state[2] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 12] = (ctx->state[3] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 16] = (ctx->state[4] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 20] = (ctx->state[5] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 24] = (ctx->state[6] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 28] = (ctx->state[7] >> (24 - i * 8)) & 0x000000ff;
    }
}

void sha256(const uint8_t *data, size_t len, uint8_t hash[SHA256_DIGEST_LENGTH]) {
    SHA256_CTX ctx;
    sha256_init(&ctx);
    sha256_update(&ctx, data, len);
    sha256_final(&ctx, hash);
}

struct AdminData {
    char username[MAX_PASSWORD_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
};

void writeAdminDataToBinaryFile(struct AdminData *adminData) {
    FILE *file = fopen("../Database/AdminDB.dat", "wb");
    if (!file) {
        printf("Error opening file for writing\n");
        return;
    }

    fwrite(adminData, sizeof(struct AdminData), 1, file);

    fclose(file);
}

void readAdminDataFromBinaryFile(struct AdminData *adminData) {
    FILE *file = fopen("../Database/AdminDB.dat", "rb");
    if (!file) {
        printf("Error opening file for reading\n");
        return;
    }

    fread(adminData, sizeof(struct AdminData), 1, file);

    fclose(file);
}


void addAdmin() {
    struct AdminData admin;

    printf("Enter admin username: ");
    scanf("%s", admin.username);

    printf("Enter admin password: ");
    scanf("%s", admin.password);

    // Hash the password before storing it
    uint8_t hashedPassword[SHA256_DIGEST_LENGTH];
    sha256((uint8_t *)admin.password, strlen(admin.password), hashedPassword);

    // Copy the hashed password to the admin data
    memcpy(admin.password, hashedPassword, SHA256_DIGEST_LENGTH);

    FILE *file = fopen("../Database/AdminDB.dat", "ab");
    if (!file) {
        printf("Error opening file for writing\n");
        return;
    }

    fwrite(&admin, sizeof(struct AdminData), 1, file);

    fclose(file);

    printf("Admin added successfully.\n");
}

int login() {
    char enteredUsername[MAX_PASSWORD_LENGTH];
    char enteredPassword[MAX_PASSWORD_LENGTH];

    struct AdminData adminData;
    readAdminDataFromBinaryFile(&adminData);

    printf("Enter username: ");
    scanf("%s", enteredUsername);

    printf("Enter password: ");
    scanf("%s", enteredPassword);

    if (strcmp(enteredUsername, adminData.username) == 0 && strcmp(enteredPassword, adminData.password) == 0) {
        return 1; // return 1 if login Successfully
    } else {
        return 0; //return 0 if login is not Successful
    }
}

int adminLogin() {
    struct AdminData admin;
    char username[MAX_PASSWORD_LENGTH];
    char password[MAX_PASSWORD_LENGTH];

    printf("Enter username: ");
    scanf("%s", username);

    printf("Enter password: ");
    scanf("%s", password);

    FILE *file = fopen("../Database/AdminDB.dat", "rb");
    if (!file) {
        printf("Error opening file for reading\n");
        return 0;
    }

    int found = 0;
    while (fread(&admin, sizeof(struct AdminData), 1, file)) {
        if (strcmp(admin.username, username) == 0) {
            // Hash the entered password for comparison
            uint8_t enteredPasswordHash[SHA256_DIGEST_LENGTH];
            sha256((uint8_t *)password, strlen(password), enteredPasswordHash);

            // Compare the hashed password with stored hashed password
            if (memcmp(admin.password, enteredPasswordHash, SHA256_DIGEST_LENGTH) == 0) {
                found = 1;
                break;
            }
        }
    }

    fclose(file);

    if (found) {
        printf("Login successful!\n");
        return 1;
    } else {
        printf("Login failed. Invalid username or password.\n");
        return 0;
    }
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
    FILE *binaryFile = fopen("../Database/IceCreambin.dat", "wb");

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


void editTransaction(struct Product *products, int row);
//--------------------------------------------- End Of TIKPOPTV ---------------------------------------------//
//Check the near expiration items in the stock
void DisplayNearExpiration(struct Product *products, int *row) {
    printf("\x1b[33mItems Near Expiration (within 30 days):\x1b[0m\n");
    printf("\x1b[33mProduct ID\tName\t       Expired Date\x1b[0m\n");

    // Get the current time
    time_t currentTime;
    time(&currentTime);

    // Assuming the date format is DD/MM/YYYY
    struct tm expirationDate;

    for (int i = 0; i < *row; i++) {
        if (strcmp(products[i].expireD, "null") != 0) {
            // Convert the string expiration date to a tm struct
            memset(&expirationDate, 0, sizeof(struct tm)); // Initialize the struct
            if (strptime(products[i].expireD, "%d/%m/%Y", &expirationDate) == NULL) {
                fprintf(stderr, "Error: Invalid date format\n");
                continue;
            }

            // Convert tm struct to time_t
            time_t expirationTime = mktime(&expirationDate);

            // Check if mktime failed
            if (expirationTime == (time_t)(-1)) {
                fprintf(stderr, "Error: Invalid date/time\n");
                continue;
            }

            // Calculate the difference in seconds
            double difference = difftime(expirationTime, currentTime);

            // Calculate the difference in days
            double differenceInDays = difference / (60 * 60 * 24);

            // Check if the item is within 30 days of expiration
            if (differenceInDays <= 30 && differenceInDays >= 0) {
                printf("\x1b[91m%-12s\t%-12s\t%s\x1b[0m\n", products[i].productID, products[i].name, products[i].expireD);
            }
        }
    }
}


//Displaying Out of stock
void DisplayOutofStock(struct Product *products, int *row) {
    printf("\x1b[33mItems Near Out of stock :\x1b[0m\n");
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
        printf("\x1b[1;31mChocolate is low on stock! Amount left : %.2lf\x1b[0m\n",sum[i].quantity);
        count++;
        }  
        else if(strcmp(sum[i].name,"Matcha")==0 && sum[i].quantity <= 3) {
            printf("\x1b[1;31mMatcha is low on stock! Amount left : %.2lf\x1b[0m\n",sum[i].quantity);
            count++;
        }  
        else if(strcmp(sum[i].name,"Strawberry")==0 && sum[i].quantity <= 3) {
            printf("\x1b[1;31mStrawberry is low on stock! Amount left : %.2lf\x1b[0m\n",sum[i].quantity);
            count++;
        }  
        else if(strcmp(sum[i].name,"Cookies & Cream")==0 && sum[i].quantity <= 3) {
            printf("\x1b[1;31mCookies & Cream is low on stock! Amount left : %.2lf\x1b[0m\n",sum[i].quantity);
            count++;
        }  
        else if(strcmp(sum[i].name,"Cone")==0 && sum[i].quantity <= 2) {
            printf("\x1b[1;31mCone is low on stock! Amount left : %.2lf\x1b[0m\n",sum[i].quantity);
            count++;
        }  
        else if(strcmp(sum[i].name,"Cup")==0 && sum[i].quantity <= 2) {
            printf("\x1b[1;31mCup is low on stock! Amount left : %.2lf\x1b[0m\n",sum[i].quantity);
            count++;
        } 
    }
    if(count == 0) printf("None\n");
    printf("\n");
}
void DisplayRemainingStock(struct Product *products, int *row) {
printf("\x1b[95m+----------------------------------------+\x1b[0m\n");
printf("\x1b[94m|    Remaining Stock for All Products    |\x1b[0m\n");
printf("\x1b[95m|                     |                  |\x1b[0m\n");
printf("\x1b[95m|     Product Name    | Remaining Stock  |\x1b[0m\n");
printf("\x1b[95m+----------------------------------------+\x1b[0m\n");


    // Arrays to store unique product IDs and names
    char *uniqueIDs[MAX_Product_ID];
    char *uniqueNames[MAX_Product_ID];
    int uniqueCount = 0;

    // Memory allocation for arrays
    for (int i = 0; i < MAX_Product_ID; i++) {
        uniqueIDs[i] = malloc(MAX_LINE_LENGTH);
        uniqueNames[i] = malloc(MAX_LINE_LENGTH);
        if (uniqueIDs[i] == NULL || uniqueNames[i] == NULL) {
            // Handle memory allocation failure
            exit(EXIT_FAILURE);
        }
    }

    // Find unique product IDs and names
    for (int i = 0; i < *row; i++) {
        int isDuplicate = 0;
        for (int j = 0; j < uniqueCount; j++) {
            if (strcmp(uniqueIDs[j], products[i].productID) == 0) {
                isDuplicate = 1;
                break;
            }
        }
        if (!isDuplicate) {
            strcpy(uniqueIDs[uniqueCount], products[i].productID);
            strcpy(uniqueNames[uniqueCount], products[i].name);
            uniqueCount++;
        }
    }

    // Struct array to hold aggregated product data
    struct Product aggregatedProducts[MAX_Product_ID];
    for (int i = 0; i < uniqueCount; i++) {
        aggregatedProducts[i].price = 0;
        aggregatedProducts[i].quantity = 0.0;
    }

    // Aggregate price and quantity for each unique product
    for (int i = 0; i < uniqueCount; i++) {
        strcpy(aggregatedProducts[i].productID, uniqueIDs[i]);
        strcpy(aggregatedProducts[i].name, uniqueNames[i]);
    }

    for (int i = 0; i < uniqueCount; i++) {
        for (int j = 0; j < *row; j++) {
            if (strcmp(aggregatedProducts[i].productID, products[j].productID) == 0) {
                if (strcmp(products[j].inout, "In") == 0) {
                    aggregatedProducts[i].price += products[j].price;
                    aggregatedProducts[i].quantity += products[j].quantity;
                } else if (strcmp(products[j].inout, "Out") == 0) {
                    aggregatedProducts[i].price -= products[j].price;
                    aggregatedProducts[i].quantity -= products[j].quantity;
                }
            }
        }
    }

    for (int i = 0; i < uniqueCount; i++) {
    printf("\x1b[95m|\x1b[0m \x1b[93m%-19s\x1b[0m \x1b[95m|\x1b[0m \x1b[92m%-16.2lf\x1b[0m \x1b[95m|\x1b[0m\n", aggregatedProducts[i].name, aggregatedProducts[i].quantity);

    }

    printf("\x1b[95m+---------------------+------------------+\x1b[0m\n");

    if (uniqueCount == 0) {
        printf("No products available.\n");
    }
}



int main() {
    int row=0,select;
    struct Product products[MAX_PRODUCTS];
    size_t recordsize = sizeof(struct Product);
    row = findNumberOfRecords("../Database/IceCreambin.dat",recordsize);
    readFromBinaryFile(products,&row,"../Database/IceCreambin.dat");
    system("clear|| cls");
    printf("\033[1;34m"); // ใช้สีน้ำเงินเข้ม
    printf("+----------------------------------------+\n");
    printf("|             ");
    printf("\033[1;36m"); // ใช้สีฟ้าเข้ม
    printf("ICE CREAM SHOP");
    printf("\033[1;34m"); // ใช้สีน้ำเงินเข้ม
    printf("             |\n");
    printf("+----------------------------------------+\n");
    printf("\033[0m"); // รีเซ็ตสีกลับมาเป็นค่าเริ่มต้น
    DisplayRemainingStock(products,&row);
    DisplayOutofStock(products,&row);
    DisplayNearExpiration(products,&row);
    displayMenu();
    printf("Select features: ");
    scanf("%d",&select);

    if(select == 2) {
        system("clear || cls");
        appendpro(products);
        main();
    }
    else if(select == 1) {
        system("clear|| cls");
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
        system("clear || cls");
        printf("Selected option 4. Displaying all products...\n");
        displayAll(products, &row);
    }
    else if(select == 5) {
      //  displayRemainingStock(products, &row);
    }
    return 0;
}

void displayMenuAndGetChoice(const char *filename) {
    printf("\033[1;34mDo you want to openFile or openFileFinder this item?\033[0m\n");
    printf("\033[1;33m1. openFile\033[0m\n");
    printf("\033[1;33m2. openFileFinder\033[0m\n");
    printf("\033[1;31m0. Main Menu\033[0m\n");

    int action;
    printf("Enter your choice: ");
    scanf("%d", &action);

    system("clear|| cls");
    switch (action) {
        case 1:
            openFile(filename);
            displayMenuAndGetChoice(filename);
            break;
        case 2:
            openFileFinder(filename);
            displayMenuAndGetChoice(filename);
            break;
        case 0:
            main();
            break; 
        default:
            printf("\033[1;31mInvalid choice.\033[0m\n");
            displayMenuAndGetChoice(filename);
            break;
    }
}

void editTransaction(struct Product *products, int row) {
    if (adminLogin()) { 
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
        printf("\033[1;33m1. EDIT Date\033[0m\n");
        printf("\033[1;33m2. EDIT ExpireD\033[0m\n");
        printf("\033[1;33m3. EDIT Quantity\033[0m\n");
        printf("\033[1;31m4. DELETE\033[0m\n");
        printf("\033[1;31m0. Main Menu\033[0m\n");
        int action;
        printf("Enter your choice: ");
        scanf("%d", &action);

        switch (action) {
            case 0:
                main();
                break;
            case 1:
                printf("Enter new date (YYYY-MM-DD): ");
                scanf("%s", products[selection].date); // Assuming date is stored as a string in the format YYYY-MM-DD
                
                printf("Updated transaction:\n");
                printf("In/Out  Date       Expired Date  Time  Product ID  Name         Amount  Price\n");
                printf("%-7s %-10s %-12s %.2lf %-11s %-12s %-7.2lf %d\n", products[selection].inout, products[selection].date, products[selection].expireD, products[selection].time, products[selection].productID, products[selection].name, products[selection].quantity, products[selection].price);

                char confirmEdit;
                printf("Confirm changes? (Y/N): ");
                scanf(" %c", &confirmEdit);

                if (confirmEdit == 'Y' || confirmEdit == 'y') {
                    updateBinary(products, row);
                } else {
                    printf("Changes discarded.\n");
                }
                break;
            case 2:
                printf("Enter new expiration date (YYYY-MM-DD): ");
                scanf("%s", products[selection].expireD); // Assuming expiration date is stored as a string in the format YYYY-MM-DD
                
                printf("Updated transaction:\n");
                printf("In/Out  Date       Expired Date  Time  Product ID  Name         Amount  Price\n");
                printf("%-7s %-10s %-12s %.2lf %-11s %-12s %-7.2lf %d\n", products[selection].inout, products[selection].date, products[selection].expireD, products[selection].time, products[selection].productID, products[selection].name, products[selection].quantity, products[selection].price);

                char confirmExpire;
                printf("Confirm changes? (Y/N): ");
                scanf(" %c", &confirmExpire);

                if (confirmExpire == 'Y' || confirmExpire == 'y') {
                    updateBinary(products, row);
                } else {
                    printf("Changes discarded.\n");
                }
                break;
            case 3:
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
            case 4:
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
    } else {
        printf("Invalid username or password. Access denied!\n");
    }
}