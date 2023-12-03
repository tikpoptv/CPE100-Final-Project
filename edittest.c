#include<stdio.h>
#include<string.h>//
#include <stdlib.h>

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
    printf("4. End of the Day\n");
    printf("0. Close Program\n");
}

void displaySummary() {
    printf("<== Select features: ==>\n");
    printf("1. Amount Left in stock\n");
    printf("2. Summary of sales right now\n");
    printf("0. Close Program\n");
}

//read data from file
void readfile(struct Product *products, int *row) {
    FILE* file = fopen("Ice_Cream_Shop.csv", "r");
    if (!file) {
        printf("Can't open file\n");
        return;
    }
    char line[MAX_LINE_LENGTH];
    fgets(line, MAX_LINE_LENGTH, file);
    *row = 0;
    while (fgets(line, MAX_LINE_LENGTH, file) != NULL)
    {
        char* value = strtok(line, ",");
        int cnt =0;
        while (value != NULL) {
            if((cnt%8)==0) strcpy(products[*row].inout, value);
            else if((cnt%8)==1) strcpy(products[*row].date, value);
            else if((cnt%8)==2) strcpy(products[*row].expireD, value);
            else if((cnt%8)==3) products[*row].time = atof(value);
            else if((cnt%8)==4) strcpy(products[*row].productID, value);
            else if((cnt%8)==5) strcpy(products[*row].name, value);
            else if((cnt%8)==6) products[*row].quantity = atof(value);
            else if((cnt%8)==7) products[*row].price = atoi(value);
            cnt++;
            value = strtok(NULL, ",");
        }
        (*row)++;
    }
    fclose(file);
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
void appendpro(struct Product *products) {
    FILE* file = fopen("Ice_Cream_Shop.csv", "a");
    int want;
    printf("How many items : ");
    scanf("%d",&want);//Input the number of item to update
    for(int i=1;i<=want;i++) {
        struct Product update;
        char a[4];
        printf("In or Out : ");
        scanf("%s",a);
        if(a[0] == 'I' && a[1] == 'n') {
            strcpy(update.inout,a);
            printf("Date : ");
            scanf("%s",update.date);
            printf("Expired date : ");
            scanf("%s",update.expireD);
            printf("Time : ");
            scanf("%lf",&update.time);
            printf("Product ID : ");
            scanf("%s",update.productID);
            printf("Name : ");
            scanf("%s",update.name);
            printf("Amount : ");
            scanf("%lf",&update.quantity);
            printf("Price : ");
            scanf("%d",&update.price);
            fprintf(file,"\n%s,%s,%s,%lf,%s,%s,%lf,%d",update.inout,update.date,update.expireD,update.time,update.productID,update.name,update.quantity,update.price);
        }
        else if(a[0]=='O' && a[1]=='u' && a[2] == 't') {
            strcpy(update.inout,a);
            printf("Date : ");
            scanf("%s",update.date);
            strcpy(update.expireD,"NULL");
            printf("Time : ");
            scanf("%lf",&update.time);
            printf("Product ID : ");
            scanf("%s",update.productID);
            printf("Name : ");
            scanf("%s",update.name);
            printf("Amount : ");
            scanf("%lf",&update.quantity);
            printf("Price : ");
            scanf("%d",&update.price);
            fprintf(file,"\n%s,%s,%s,%.2lf,%s,%s,%.2lf,%d",update.inout,update.date,update.expireD,update.time,update.productID,update.name,update.quantity,update.price);
        }
    }
    fclose(file);
}

void EditTransaction(struct Product *products, int row) {
    char dateToEdit[MAX_LINE_LENGTH];
    printf("Enter the date (DD/MM/YYYY) of the transactions to display: ");
    scanf("%s", dateToEdit);

    printf("Transactions on %s:\n", dateToEdit);
    int found = 0;
    for (int i = 0; i < row; i++) {
        if (strcmp(products[i].date, dateToEdit) == 0) {
            found = 1;
            printf("Product ID: %s, Name: %s\n", products[i].productID, products[i].name);
            printf("Quantity: %.2lf, Price: %d\n", products[i].quantity, products[i].price);
        }
    }

    if (!found) {
        printf("No transactions found for %s\n", dateToEdit);
    }
}


int main() {
    int row=0,select;
    struct Product products[MAX_PRODUCTS];
    displayMenu();
    scanf("%d",&select);
    readfile(products,&row);
    if(select == 2) {
        appendpro(products);
    }
    else if(select == 1) {
        system("clear");
        Display(products,&row);
    }else if(select == 3) {
        system("clear || cls");
        EditTransaction(products, row);
    }
    return 0;
}