/**
 * ==============================================================================
 * ByteC: Starter Template: Matrix Operations (Addition and Multiplication)
 * ==============================================================================
 */

#include <stdio.h>

#define MAX 10

void inputMatrix(int mat[MAX][MAX], int r, int c, const char *name)
{
    printf("Enter elements for Matrix %s (%dx%d):\n", name, r, c);
    for (int i = 0; i < r; i++)
    {
        for (int j = 0; j < c; j++)
        {
            scanf("%d", &mat[i][j]);
        }
    }
}

void printMatrix(int mat[MAX][MAX], int r, int c)
{
    for (int i = 0; i < r; i++)
    {
        for (int j = 0; j < c; j++)
        {
            printf("%5d ", mat[i][j]);
        }
        printf("\n");
    }
}

int main()
{
    int a[MAX][MAX], b[MAX][MAX], res[MAX][MAX];
    int r1, c1, r2, c2, choice;

    printf("--- Matrix Operations Menu ---\n");
    printf("1. Matrix Addition\n");
    printf("2. Matrix Multiplication\n");
    printf("Choice: ");
    if (scanf("%d", &choice) != 1) return 1;

    if (choice == 1)
    {
        printf("Enter rows and columns for matrices (max %d): ", MAX);
        scanf("%d %d", &r1, &c1);
        inputMatrix(a, r1, c1, "A");
        inputMatrix(b, r1, c1, "B");

        for (int i = 0; i < r1; i++)
        {
            for (int j = 0; j < c1; j++)
            {
                res[i][j] = a[i][j] + b[i][j];
            }
        }
        printf("\nResult of Matrix Addition (A + B):\n");
        printMatrix(res, r1, c1);
    }
    else if (choice == 2)
    {
        printf("Enter rows and columns for Matrix A: ");
        scanf("%d %d", &r1, &c1);
        printf("Enter rows and columns for Matrix B: ");
        scanf("%d %d", &r2, &c2);

        if (c1 != r2)
        {
            printf("Error: Matrix multiplication not possible! (Cols of A must equal Rows of B)\n");
            return 1;
        }

        inputMatrix(a, r1, c1, "A");
        inputMatrix(b, r2, c2, "B");

        for (int i = 0; i < r1; i++)
        {
            for (int j = 0; j < c2; j++)
            {
                res[i][j] = 0;
                for (int k = 0; k < c1; k++)
                {
                    res[i][j] += a[i][k] * b[k][j];
                }
            }
        }
        printf("\nResult of Matrix Multiplication (A x B):\n");
        printMatrix(res, r1, c2);
    }
    else
    {
        printf("Invalid choice!\n");
    }

    return 0;
}
