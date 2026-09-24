/**
 * ==============================================================================
 * ByteC: Starter Template: Singly Linked List Operations
 * ==============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

struct Node
{
    int data;
    struct Node *next;
};

struct Node *createNode(int data)
{
    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
    if (!newNode)
    {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

void insertAtEnd(struct Node **head, int data)
{
    struct Node *newNode = createNode(data);
    if (*head == NULL)
    {
        *head = newNode;
        return;
    }
    struct Node *temp = *head;
    while (temp->next != NULL)
    {
        temp = temp->next;
    }
    temp->next = newNode;
}

void deleteNode(struct Node **head, int key)
{
    struct Node *temp = *head, *prev = NULL;

    if (temp != NULL && temp->data == key)
    {
        *head = temp->next;
        free(temp);
        printf("Node %d deleted.\n", key);
        return;
    }

    while (temp != NULL && temp->data != key)
    {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL)
    {
        printf("Node %d not found in list.\n", key);
        return;
    }

    prev->next = temp->next;
    free(temp);
    printf("Node %d deleted.\n", key);
}

void displayList(struct Node *head)
{
    if (head == NULL)
    {
        printf("List is empty.\n");
        return;
    }
    printf("List: ");
    struct Node *temp = head;
    while (temp != NULL)
    {
        printf("%d -> ", temp->data);
        temp = temp->next;
    }
    printf("NULL\n");
}

void freeList(struct Node *head)
{
    struct Node *temp;
    while (head != NULL)
    {
        temp = head;
        head = head->next;
        free(temp);
    }
}

int main()
{
    struct Node *head = NULL;
    int choice, val;

    printf("--- Singly Linked List Menu ---\n");
    printf("1. Insert Node\n");
    printf("2. Delete Node\n");
    printf("3. Display List\n");
    printf("4. Exit\n");

    while (1)
    {
        printf("\nEnter choice (1-4): ");
        if (scanf("%d", &choice) != 1) break;

        switch (choice)
        {
            case 1:
                printf("Enter value to insert: ");
                scanf("%d", &val);
                insertAtEnd(&head, val);
                displayList(head);
                break;
            case 2:
                printf("Enter value to delete: ");
                scanf("%d", &val);
                deleteNode(&head, val);
                displayList(head);
                break;
            case 3:
                displayList(head);
                break;
            case 4:
                freeList(head);
                printf("Goodbye!\n");
                return 0;
            default:
                printf("Invalid choice. Try again.\n");
        }
    }

    freeList(head);
    return 0;
}
