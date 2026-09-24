/**
 * ==============================================================================
 * ByteC: Starter Template: Circular Queue Operations
 * ==============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#define SIZE 5

struct Queue
{
    int items[SIZE];
    int front, rear;
};

void initQueue(struct Queue *q)
{
    q->front = -1;
    q->rear = -1;
}

int isFull(struct Queue *q)
{
    return ((q->front == 0 && q->rear == SIZE - 1) || (q->front == q->rear + 1));
}

int isEmpty(struct Queue *q)
{
    return (q->front == -1);
}

void enqueue(struct Queue *q, int element)
{
    if (isFull(q))
    {
        printf("Queue is Full!\n");
        return;
    }
    if (q->front == -1) q->front = 0;
    q->rear = (q->rear + 1) % SIZE;
    q->items[q->rear] = element;
    printf("Inserted %d\n", element);
}

int dequeue(struct Queue *q)
{
    int element;
    if (isEmpty(q))
    {
        printf("Queue is Empty!\n");
        return -1;
    }
    element = q->items[q->front];
    if (q->front == q->rear)
    {
        q->front = -1;
        q->rear = -1;
    }
    else
    {
        q->front = (q->front + 1) % SIZE;
    }
    return element;
}

void display(struct Queue *q)
{
    if (isEmpty(q))
    {
        printf("Queue is Empty!\n");
        return;
    }
    printf("Queue elements: ");
    int i = q->front;
    while (1)
    {
        printf("%d ", q->items[i]);
        if (i == q->rear) break;
        i = (i + 1) % SIZE;
    }
    printf("\n");
}

int main()
{
    struct Queue q;
    initQueue(&q);
    int choice, val;

    printf("--- Circular Queue Menu (Capacity: %d) ---\n", SIZE);
    printf("1. Enqueue\n");
    printf("2. Dequeue\n");
    printf("3. Display\n");
    printf("4. Exit\n");

    while (1)
    {
        printf("\nEnter choice (1-4): ");
        if (scanf("%d", &choice) != 1) break;

        switch (choice)
        {
            case 1:
                printf("Enter value: ");
                scanf("%d", &val);
                enqueue(&q, val);
                break;
            case 2:
                val = dequeue(&q);
                if (val != -1) printf("Dequeued: %d\n", val);
                break;
            case 3:
                display(&q);
                break;
            case 4:
                return 0;
            default:
                printf("Invalid choice!\n");
        }
    }
    return 0;
}
