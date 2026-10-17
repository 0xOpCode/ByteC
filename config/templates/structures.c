/**
 * ==============================================================================
 * ByteC: Starter Template: Student Record Management System (Structures)
 * ==============================================================================
 */

#include <stdio.h>
#include <string.h>

#define MAX_STUDENTS 100

struct Student
{
    int roll;
    char name[50];
    float marks;
};

void clearInputLine(void)
{
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF)
    {
    }
}

void addStudent(struct Student records[], int *count)
{
    if (*count >= MAX_STUDENTS)
    {
        printf("Record limit reached.\n");
        return;
    }

    struct Student s;
    printf("Enter roll number: ");
    if (scanf("%d", &s.roll) != 1)
    {
        return;
    }
    clearInputLine();

    printf("Enter student name: ");
    if (fgets(s.name, sizeof(s.name), stdin) == NULL)
    {
        return;
    }
    size_t len = strlen(s.name);
    if (len > 0 && s.name[len - 1] == '\n')
    {
        s.name[len - 1] = '\0';
    }

    printf("Enter marks: ");
    if (scanf("%f", &s.marks) != 1)
    {
        return;
    }

    records[*count] = s;
    (*count)++;
    printf("Record added for roll %d.\n", s.roll);
}

void displayAll(const struct Student records[], int count)
{
    if (count == 0)
    {
        printf("No student records stored.\n");
        return;
    }

    printf("\n%-10s %-25s %-10s\n", "Roll", "Name", "Marks");
    printf("-----------------------------------------------\n");
    for (int i = 0; i < count; i++)
    {
        printf("%-10d %-25s %-10.2f\n", records[i].roll, records[i].name, records[i].marks);
    }
}

void searchByRoll(const struct Student records[], int count, int targetRoll)
{
    for (int i = 0; i < count; i++)
    {
        if (records[i].roll == targetRoll)
        {
            printf("Record found: Roll=%d, Name=%s, Marks=%.2f\n",
                   records[i].roll, records[i].name, records[i].marks);
            return;
        }
    }
    printf("No student found with roll %d.\n", targetRoll);
}

int main(void)
{
    struct Student records[MAX_STUDENTS];
    int count = 0;
    int choice;

    printf("--- Student Record Management System ---\n");
    printf("1. Add Student Record\n");
    printf("2. Display All Records\n");
    printf("3. Search by Roll Number\n");
    printf("4. Exit\n");

    while (1)
    {
        printf("\nEnter choice (1-4): ");
        if (scanf("%d", &choice) != 1)
        {
            break;
        }

        if (choice == 1)
        {
            addStudent(records, &count);
        }
        else if (choice == 2)
        {
            displayAll(records, count);
        }
        else if (choice == 3)
        {
            int roll;
            printf("Enter roll number to search: ");
            if (scanf("%d", &roll) == 1)
            {
                searchByRoll(records, count, roll);
            }
        }
        else if (choice == 4)
        {
            printf("Exiting.\n");
            return 0;
        }
        else
        {
            printf("Invalid choice.\n");
        }
    }

    return 0;
}
