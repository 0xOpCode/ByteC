/**
 * ==============================================================================
 * ByteC: Starter Template: Sorting Algorithms (Bubble, Selection, Insertion)
 * ==============================================================================
 */

#include <stdio.h>

void printArray(int arr[], int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void bubbleSort(int arr[], int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void selectionSort(int arr[], int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        int minIdx = i;
        for (int j = i + 1; j < n; j++)
        {
            if (arr[j] < arr[minIdx]) minIdx = j;
        }
        int temp = arr[minIdx];
        arr[minIdx] = arr[i];
        arr[i] = temp;
    }
}

void insertionSort(int arr[], int n)
{
    for (int i = 1; i < n; i++)
    {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

int main()
{
    int n, choice;
    printf("Enter number of elements: ");
    if (scanf("%d", &n) != 1 || n <= 0) return 1;

    int arr[n];
    printf("Enter %d integers separated by spaces:\n", n);
    for (int i = 0; i < n; i++)
    {
        scanf("%d", &arr[i]);
    }

    printf("\nChoose Sorting Algorithm:\n");
    printf("1. Bubble Sort\n");
    printf("2. Selection Sort\n");
    printf("3. Insertion Sort\n");
    printf("Choice: ");
    scanf("%d", &choice);

    printf("\nOriginal array: ");
    printArray(arr, n);

    switch (choice)
    {
        case 1:
            bubbleSort(arr, n);
            printf("Bubble Sorted:  ");
            break;
        case 2:
            selectionSort(arr, n);
            printf("Selection Sorted: ");
            break;
        case 3:
            insertionSort(arr, n);
            printf("Insertion Sorted: ");
            break;
        default:
            printf("Invalid choice!\n");
            return 1;
    }

    printArray(arr, n);
    return 0;
}
