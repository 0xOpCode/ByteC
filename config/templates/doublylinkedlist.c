/**
 * ==============================================================================
 * ByteC: Starter Template: Doubly Linked List Operations
 * ==============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

struct DNode
{
    int data;
    struct DNode *prev;
    struct DNode *next;
};

struct DNode *createNode(int data)
{
    struct DNode *node = (struct DNode *)malloc(sizeof(struct DNode));
    if (node == NULL)
    {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    node->data = data;
    node->prev = NULL;
    node->next = NULL;
    return node;
}

void insertAtHead(struct DNode **head, int data)
{
    struct DNode *newNode = createNode(data);
    if (*head != NULL)
    {
        newNode->next = *head;
        (*head)->prev = newNode;
    }
    *head = newNode;
}

void insertAtTail(struct DNode **head, int data)
{
    struct DNode *newNode = createNode(data);
    if (*head == NULL)
    {
        *head = newNode;
        return;
    }
    struct DNode *temp = *head;
    while (temp->next != NULL)
    {
        temp = temp->next;
    }
    temp->next = newNode;
    newNode->prev = temp;
}

void deleteByValue(struct DNode **head, int key)
{
    struct DNode *curr = *head;
    while (curr != NULL && curr->data != key)
    {
        curr = curr->next;
    }

    if (curr == NULL)
    {
        printf("Node %d not found in list.\n", key);
        return;
    }

    if (curr->prev != NULL)
    {
        curr->prev->next = curr->next;
    }
    else
    {
        *head = curr->next;
    }

    if (curr->next != NULL)
    {
        curr->next->prev = curr->prev;
    }

    free(curr);
    printf("Node %d deleted.\n", key);
}

void displayForward(struct DNode *head)
{
    if (head == NULL)
    {
        printf("List is empty.\n");
        return;
    }
    printf("Forward: NULL <-> ");
    struct DNode *temp = head;
    while (temp != NULL)
    {
        printf("%d <-> ", temp->data);
        temp = temp->next;
    }
    printf("NULL\n");
}

void displayReverse(struct DNode *head)
{
    if (head == NULL)
    {
        printf("List is empty.\n");
        return;
    }
    struct DNode *temp = head;
    while (temp->next != NULL)
    {
        temp = temp->next;
    }
    printf("Reverse: NULL <-> ");
    while (temp != NULL)
    {
        printf("%d <-> ", temp->data);
        temp = temp->prev;
    }
    printf("NULL\n");
}

void freeList(struct DNode *head)
{
    struct DNode *temp;
    while (head != NULL)
    {
        temp = head;
        head = head->next;
        free(temp);
    }
}

int main(void)
{
    struct DNode *head = NULL;
    int choice, val;

    printf("--- Doubly Linked List Menu ---\n");
    printf("1. Insert at Head\n");
    printf("2. Insert at Tail\n");
    printf("3. Delete by Value\n");
    printf("4. Display Forward\n");
    printf("5. Display Reverse\n");
    printf("6. Exit\n");

    while (1)
    {
        printf("\nEnter choice (1-6): ");
        if (scanf("%d", &choice) != 1)
        {
            break;
        }

        switch (choice)
        {
            case 1:
                printf("Enter value to insert at head: ");
                if (scanf("%d", &val) == 1)
                {
                    insertAtHead(&head, val);
                    displayForward(head);
                }
                break;
            case 2:
                printf("Enter value to insert at tail: ");
                if (scanf("%d", &val) == 1)
                {
                    insertAtTail(&head, val);
                    displayForward(head);
                }
                break;
            case 3:
                printf("Enter value to delete: ");
                if (scanf("%d", &val) == 1)
                {
                    deleteByValue(&head, val);
                    displayForward(head);
                }
                break;
            case 4:
                displayForward(head);
                break;
            case 5:
                displayReverse(head);
                break;
            case 6:
                freeList(head);
                printf("Exiting.\n");
                return 0;
            default:
                printf("Invalid choice.\n");
        }
    }

    freeList(head);
    return 0;
}
