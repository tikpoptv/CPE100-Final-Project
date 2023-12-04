#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Record {
    char inOut[4]; // In/Out
    char date[11]; // Date
    char expiredDate[11]; // Expired Date
    char time[6]; // Time
    char productID[8]; // Product ID
    char name[50]; // Name
    int amount; // Amount
    int price; // Price
};

int main() {
    FILE *csvFile = fopen("Ice_Cream_Shop.csv", "r"); // เปิดไฟล์ CSV เพื่ออ่าน
    FILE *binaryFile = fopen("Ice_Cream_Shop.bin", "wb"); // เปิดไฟล์สำหรับเขียน binary

    if (csvFile != NULL && binaryFile != NULL) {
        char line[256]; // สำหรับเก็บข้อมูลแต่ละบรรทัดจาก CSV
        struct Record record;

        // ข้ามบรรทัดแรก (หัว column)
        fgets(line, sizeof(line), csvFile);

        // อ่านและเขียนข้อมูลจาก CSV เป็น binary
        while (fgets(line, sizeof(line), csvFile) != NULL) {
            sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%d,%d",
                   record.inOut, record.date, record.expiredDate, record.time,
                   record.productID, record.name, &record.amount, &record.price);

            // เขียนข้อมูลลงในไฟล์ binary
            fwrite(&record, sizeof(struct Record), 1, binaryFile);
        }

        fclose(csvFile); // ปิดไฟล์ CSV
        fclose(binaryFile); // ปิดไฟล์ binary
        printf("เขียนข้อมูลเป็นไฟล์ binary เรียบร้อย\n");
    } else {
        printf("ไม่สามารถเปิดไฟล์ได้");
    }

    return 0;
}
