/**
 * ==============================================================================
 * ByteC: Starter Template: Recursion (Factorial, Fibonacci, GCD, Tower of Hanoi)
 * ==============================================================================
 */

#include <stdio.h>

unsigned long long factorial(int n)
{
    if (n <= 1)
    {
        return 1ULL;
    }
    return (unsigned long long)n * factorial(n - 1);
}

int fibonacci(int n)
{
    if (n <= 0)
    {
        return 0;
    }
    if (n == 1)
    {
        return 1;
    }
    return fibonacci(n - 1) + fibonacci(n - 2);
}

int gcd(int a, int b)
{
    if (b == 0)
    {
        return a < 0 ? -a : a;
    }
    return gcd(b, a % b);
}

void towerOfHanoi(int n, char fromRod, char toRod, char auxRod, int *step)
{
    if (n == 0)
    {
        return;
    }
    towerOfHanoi(n - 1, fromRod, auxRod, toRod, step);
    (*step)++;
    printf("Step %d: Move disk %d from %c to %c\n", *step, n, fromRod, toRod);
    towerOfHanoi(n - 1, auxRod, toRod, fromRod, step);
}

int main(void)
{
    int choice;

    printf("--- Recursion Lab Menu ---\n");
    printf("1. Factorial of N\n");
    printf("2. Fibonacci Series\n");
    printf("3. Greatest Common Divisor (Euclidean GCD)\n");
    printf("4. Tower of Hanoi Step Trace\n");
    printf("5. Exit\n");

    while (1)
    {
        printf("\nEnter choice (1-5): ");
        if (scanf("%d", &choice) != 1)
        {
            break;
        }

        if (choice == 1)
        {
            int n;
            printf("Enter non-negative integer N (0-20): ");
            if (scanf("%d", &n) == 1 && n >= 0 && n <= 20)
            {
                printf("%d factorial = %llu\n", n, factorial(n));
            }
            else
            {
                printf("Input out of range.\n");
            }
        }
        else if (choice == 2)
        {
            int terms;
            printf("Enter number of terms: ");
            if (scanf("%d", &terms) == 1 && terms > 0)
            {
                printf("Fibonacci series: ");
                for (int i = 0; i < terms; i++)
                {
                    printf("%d ", fibonacci(i));
                }
                printf("\n");
            }
        }
        else if (choice == 3)
        {
            int a, b;
            printf("Enter two integers: ");
            if (scanf("%d %d", &a, &b) == 2)
            {
                printf("GCD(%d, %d) = %d\n", a, b, gcd(a, b));
            }
        }
        else if (choice == 4)
        {
            int disks;
            int step = 0;
            printf("Enter number of disks (1-8): ");
            if (scanf("%d", &disks) == 1 && disks >= 1 && disks <= 8)
            {
                towerOfHanoi(disks, 'A', 'C', 'B', &step);
                printf("Total moves: %d\n", step);
            }
            else
            {
                printf("Disk count must be between 1 and 8.\n");
            }
        }
        else if (choice == 5)
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
