#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h> // ไลบรารีนอนสำหรับใช้ฟังก์ชัน sleep

void displayCurrentTime() {
    time_t currentTime;
    struct tm *localTime;

    currentTime = time(NULL);
    localTime = localtime(&currentTime);

    printf("Current Time: %02d:%02d:%02d\n",
           localTime->tm_hour, localTime->tm_min, localTime->tm_sec);
}

int main() {
    while (1) { // วงรอบการทำงานตลอดเวลา
        system("clear || cls"); // ล้างหน้าจอ (สำหรับ Unix-based system เช่น Linux, Mac) หรือ (สำหรับ Windows)
        displayCurrentTime(); // แสดงเวลาปัจจุบัน
        sleep(1); // หน่วงเวลา 1 วินาที
    }

    return 0;
}
