/**
 * ==============================================================================
 * ByteC: Starter Template: Stack (Array Implementation)
 * ==============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 100

struct Stack
{
    int items[MAX_SIZE];
    int top;
};

void initStack(struct Stack *s)
{
    s->top = -1;
}

int isFull(struct Stack *s)
{
    return s->top == MAX_SIZE - 1;
}

int isEmpty(struct Stack *s)
{
    return s->top == -1;
}

void push(struct Stack *s, int val)
{
    if (isFull(s))
    {
        printf("Stack Overflow! Cannot push %d\n", val);
        return;
    }
    s->items[++(s->top)] = val;
    printf("Pushed %d onto stack.\n", val);
}

int pop(struct Stack *s)
{
    if (isEmpty(s))
    {
        printf("Stack Underflow! Stack is empty.\n");
        return -1;
    }
    return s->items[(s->top)--];
}

int peek(struct Stack *s)
{
    if (isEmpty(s))
    {
        printf("Stack is empty.\n");
        return -1;
    }
    return s->items[s->top];
}

void display(struct Stack *s)
{
    if (isEmpty(s))
    {
        printf("Stack is empty.\n");
        return;
    }
    printf("Stack (top to bottom): ");
    for (int i = s->top; i >= 0; i--)
    {
        printf("%d ", s->items[i]);
    }
    printf("\n");
}

int main()
{
    struct Stack s;
    initStack(&s);
    int choice, val;

    printf("--- Stack Menu (Max: %d) ---\n", MAX_SIZE);
    printf("1. Push\n");
    printf("2. Pop\n");
    printf("3. Peek\n");
    printf("4. Display\n");
    printf("5. Exit\n");

    while (1)
    {
        printf("\nEnter choice (1-5): ");
        if (scanf("%d", &choice) != 1) break;

        switch (choice)
        {
            case 1:
                printf("Enter value to push: ");
                scanf("%d", &val);
                push(&s, val);
                break;
            case 2:
                val = pop(&s);
                if (val != -1) printf("Popped: %d\n", val);
                break;
            case 3:
                val = peek(&s);
                if (val != -1) printf("Top element: %d\n", val);
                break;
            case 4:
                display(&s);
                break;
            case 5:
                printf("Exiting stack program.\n");
                return 0;
            default:
                printf("Invalid choice!\n");
        }
    }
    return 0;
}
