#include <stdio.h>
#include <string.h>

#define MAX_PASSWORD_LENGTH 50
#define SHA256_BLOCK_SIZE 32

struct AdminData {
    char username[MAX_PASSWORD_LENGTH];
    unsigned char passwordHash[SHA256_BLOCK_SIZE];
};

void sha256(const char *password, unsigned char *hash) {
    unsigned char sha256_hash[SHA256_BLOCK_SIZE];
    unsigned int i;

    // Simple SHA-256 implementation (not recommended for production)
    for (i = 0; i < SHA256_BLOCK_SIZE; i++) {
        sha256_hash[i] = password[i % strlen(password)] ^ (i * 13); // Simple XOR operation
    }

    // Copy the generated hash to the given hash pointer
    for (i = 0; i < SHA256_BLOCK_SIZE; i++) {
        hash[i] = sha256_hash[i];
    }
}

void saveAdminData(const char *username, const char *password) {
    struct AdminData admin;
    strcpy(admin.username, username);
    sha256(password, admin.passwordHash);

    FILE *file = fopen("AdminDB.dat", "wb");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    fwrite(&admin, sizeof(struct AdminData), 1, file);
    fclose(file);

    printf("Admin data saved successfully.\n");
}

int main() {
    char username[MAX_PASSWORD_LENGTH];
    char password[MAX_PASSWORD_LENGTH];

    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);

    saveAdminData(username, password);

    return 0;
}
