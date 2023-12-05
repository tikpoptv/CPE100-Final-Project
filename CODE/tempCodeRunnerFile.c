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
            strptime(products[i].expireD, "%d/%m/%Y", &expirationDate);

            // Convert tm struct to time_t
            time_t expirationTime = mktime(&expirationDate);

            // Calculate the difference in seconds
            double difference = difftime(expirationTime, currentTime);

            // Calculate the difference in days
            double differenceInDays = difference / (60 * 60 * 24);

            // Check if the item is within 30 days of expiration
            if (differenceInDays < 30) {
                printf("\x1b[91m%-12s\t%-12s\t%s\x1b[0m\n", products[i].productID, products[i].name, products[i].expireD);
            }
        }
    }
}