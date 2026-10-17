/**
 * ==============================================================================
 * ByteC: Starter Template: Searching Algorithms (Linear and Binary Search)
 * ==============================================================================
 */

#include <stdio.h>

int linearSearch(const int arr[], int n, int key)
{
    for (int i = 0; i < n; i++)
    {
        if (arr[i] == key)
        {
            return i;
        }
    }
    return -1;
}

int binarySearchIterative(const int arr[], int n, int key)
{
    int low = 0;
    int high = n - 1;

    while (low <= high)
    {
        int mid = low + (high - low) / 2;
        if (arr[mid] == key)
        {
            return mid;
        }
        if (arr[mid] < key)
        {
            low = mid + 1;
        }
        else
        {
            high = mid - 1;
        }
    }
    return -1;
}

int binarySearchRecursive(const int arr[], int low, int high, int key)
{
    if (low > high)
    {
        return -1;
    }
    int mid = low + (high - low) / 2;
    if (arr[mid] == key)
    {
        return mid;
    }
    if (arr[mid] > key)
    {
        return binarySearchRecursive(arr, low, mid - 1, key);
    }
    return binarySearchRecursive(arr, mid + 1, high, key);
}

void sortAscending(int arr[], int n)
{
    for (int i = 1; i < n; i++)
    {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void printArray(const int arr[], int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main(void)
{
    int n, choice, key, idx;

    printf("Enter number of elements: ");
    if (scanf("%d", &n) != 1 || n <= 0)
    {
        return 1;
    }

    int arr[n];
    printf("Enter %d integers:\n", n);
    for (int i = 0; i < n; i++)
    {
        scanf("%d", &arr[i]);
    }

    printf("\n--- Searching Algorithms Menu ---\n");
    printf("1. Linear Search\n");
    printf("2. Binary Search (Iterative)\n");
    printf("3. Binary Search (Recursive)\n");
    printf("Choice: ");
    if (scanf("%d", &choice) != 1)
    {
        return 1;
    }

    printf("Enter target value to search: ");
    if (scanf("%d", &key) != 1)
    {
        return 1;
    }

    if (choice == 1)
    {
        idx = linearSearch(arr, n, key);
    }
    else if (choice == 2 || choice == 3)
    {
        sortAscending(arr, n);
        printf("Sorted array for binary search: ");
        printArray(arr, n);
        if (choice == 2)
        {
            idx = binarySearchIterative(arr, n, key);
        }
        else
        {
            idx = binarySearchRecursive(arr, 0, n - 1, key);
        }
    }
    else
    {
        printf("Invalid choice.\n");
        return 1;
    }

    if (idx != -1)
    {
        printf("Element %d found at index %d.\n", key, idx);
    }
    else
    {
        printf("Element %d not found.\n", key);
    }

    return 0;
}
