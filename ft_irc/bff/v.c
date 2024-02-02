#include <stdio.h>
#include <stdlib.h>

char r(char ch) {
    if ((ch >= 'A' && ch <= 'Z')) {
        return 'A' + (ch - 'A' + 13) % 26;
    } else if (ch >= 'a' && ch <= 'z') {
        return 'a' + (ch - 'a' + 13) % 26;
    } else {
        return ch; // Return unchanged if not a letter
    }
}

void enc(char *str) {
    while (*str) {
        *str = r(*str);
        str++;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    // Open the file for reading
    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    // Read the file content
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *fileContent = (char *)malloc(fileSize + 1);
    if (fileContent == NULL) {
        perror("Error allocating memory");
        fclose(file);
        return 1;
    }

    fread(fileContent, 1, fileSize, file);
    fclose(file);

    // Null-terminate the content
    fileContent[fileSize] = '\0';

    enc(fileContent);

    printf("Enc:\n%s", fileContent);

    // Clean up memory
    free(fileContent);

    return 0;
}

