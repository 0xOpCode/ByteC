/**
 * ==============================================================================
 * ByteC: Starter Template: Pointers and Dynamic Memory
 * ==============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

int main()
{
    printf("=========================================\n");
    printf("ByteC: Pointers & Dynamic Memory Demo\n");
    printf("=========================================\n\n");

    int x = 10, y = 20;
    printf("[1] Pass by Reference Demonstration:\n");
    printf("    Before swap: x = %d, y = %d\n", x, y);
    swap(&x, &y);
    printf("    After swap:  x = %d, y = %d\n\n", x, y);

    int n;
    printf("[2] Dynamic Memory Allocation:\n");
    printf("    Enter number of elements to allocate on heap: ");
    if (scanf("%d", &n) != 1 || n <= 0) return 1;

    int *arr = (int *)malloc(n * sizeof(int));
    if (arr == NULL)
    {
        printf("    Memory allocation failed.\n");
        return 1;
    }

    printf("    Enter %d integers: ", n);
    for (int i = 0; i < n; i++)
    {
        scanf("%d", arr + i);
    }

    printf("    Heap-stored values: ");
    for (int i = 0; i < n; i++)
    {
        printf("%d ", *(arr + i));
    }
    printf("\n");

    free(arr);
    printf("    Memory freed.\n");

    return 0;
}
