/**
 * ==============================================================================
 * ByteC: Starter Template: File I/O Operations (Read and Write)
 * ==============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 256

void writeFile(const char *filename)
{
    FILE *fp = fopen(filename, "w");
    if (!fp)
    {
        perror("Error opening file for writing");
        return;
    }

    printf("Enter text to write into %s (Type 'END' on a new line to finish):\n", filename);
    char line[BUFFER_SIZE];

    // Clear input buffer before reading lines
    while (getchar() != '\n');

    while (fgets(line, sizeof(line), stdin))
    {
        if (line[0] == 'E' && line[1] == 'N' && line[2] == 'D') break;
        fputs(line, fp);
    }

    fclose(fp);
    printf("File written successfully.\n");
}

void readFile(const char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (!fp)
    {
        perror("Error opening file for reading");
        return;
    }

    printf("\n--- Contents of %s ---\n", filename);
    char line[BUFFER_SIZE];
    int lineNum = 1;

    while (fgets(line, sizeof(line), fp))
    {
        printf("%3d: %s", lineNum++, line);
    }
    printf("---------------------------\n");

    fclose(fp);
}

int main()
{
    char filename[100];
    int choice;

    printf("--- File Operations Menu ---\n");
    printf("Enter target file name: ");
    if (scanf("%99s", filename) != 1) return 1;

    printf("1. Write to file\n");
    printf("2. Read from file\n");
    printf("Choice: ");
    scanf("%d", &choice);

    if (choice == 1)
        writeFile(filename);
    else if (choice == 2)
        readFile(filename);
    else
        printf("Invalid choice!\n");

    return 0;
}
