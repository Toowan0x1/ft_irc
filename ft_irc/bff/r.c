#include <stdio.h>
#include <stdlib.h> // Include the <stdlib.h> header for malloc

char r(char ch) {
    if ((ch >= 'A' && ch <= 'Z')) {
        return 'A' + (ch - 'A' + 13) % 26;
    } else if (ch >= 'a' && ch <= 'z') {
        return 'a' + (ch - 'a' + 13) % 26;
    } else {
        return ch; // Return unchanged if not a letter
    }
}

void dec(char *str) {
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

    char *fileContent = (char *)malloc(fileSize + 1); // Cast the result of malloc to char*
    if (fileContent == NULL) {
        perror("Error allocating memory");
        fclose(file);
        return 1;
    }

    fread(fileContent, 1, fileSize, file);
    fclose(file);

    fileContent[fileSize] = '\0';

    dec(fileContent);

    printf("Dec:\n%s", fileContent);

    free(fileContent);

    return 0;
}

