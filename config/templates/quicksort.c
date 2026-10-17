/**
 * ==============================================================================
 * ByteC: Starter Template: Divide and Conquer Sorting (Quick Sort and Merge Sort)
 * ==============================================================================
 */

#include <stdio.h>

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int arr[], int low, int high)
{
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++)
    {
        if (arr[j] <= pivot)
        {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

void quickSort(int arr[], int low, int high)
{
    if (low < high)
    {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

void merge(int arr[], int left, int mid, int right)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;
    int leftArr[n1];
    int rightArr[n2];

    for (int i = 0; i < n1; i++)
    {
        leftArr[i] = arr[left + i];
    }
    for (int j = 0; j < n2; j++)
    {
        rightArr[j] = arr[mid + 1 + j];
    }

    int i = 0;
    int j = 0;
    int k = left;

    while (i < n1 && j < n2)
    {
        if (leftArr[i] <= rightArr[j])
        {
            arr[k++] = leftArr[i++];
        }
        else
        {
            arr[k++] = rightArr[j++];
        }
    }

    while (i < n1)
    {
        arr[k++] = leftArr[i++];
    }

    while (j < n2)
    {
        arr[k++] = rightArr[j++];
    }
}

void mergeSort(int arr[], int left, int right)
{
    if (left < right)
    {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
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
    int n, choice;

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

    printf("\n--- Divide and Conquer Sort Menu ---\n");
    printf("1. Quick Sort\n");
    printf("2. Merge Sort\n");
    printf("Choice: ");
    if (scanf("%d", &choice) != 1)
    {
        return 1;
    }

    printf("\nOriginal array: ");
    printArray(arr, n);

    if (choice == 1)
    {
        quickSort(arr, 0, n - 1);
        printf("Quick Sorted:   ");
    }
    else if (choice == 2)
    {
        mergeSort(arr, 0, n - 1);
        printf("Merge Sorted:   ");
    }
    else
    {
        printf("Invalid choice.\n");
        return 1;
    }

    printArray(arr, n);
    return 0;
}
