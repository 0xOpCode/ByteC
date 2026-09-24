/**
 * ==============================================================================
 * ByteC: Starter Template: Binary Search Tree (BST)
 * ==============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

struct TreeNode
{
    int data;
    struct TreeNode *left;
    struct TreeNode *right;
};

struct TreeNode *createNode(int val)
{
    struct TreeNode *node = (struct TreeNode *)malloc(sizeof(struct TreeNode));
    node->data = val;
    node->left = NULL;
    node->right = NULL;
    return node;
}

struct TreeNode *insert(struct TreeNode *root, int val)
{
    if (root == NULL) return createNode(val);
    if (val < root->data)
        root->left = insert(root->left, val);
    else if (val > root->data)
        root->right = insert(root->right, val);
    return root;
}

void inorder(struct TreeNode *root)
{
    if (root != NULL)
    {
        inorder(root->left);
        printf("%d ", root->data);
        inorder(root->right);
    }
}

void preorder(struct TreeNode *root)
{
    if (root != NULL)
    {
        printf("%d ", root->data);
        preorder(root->left);
        preorder(root->right);
    }
}

void postorder(struct TreeNode *root)
{
    if (root != NULL)
    {
        postorder(root->left);
        postorder(root->right);
        printf("%d ", root->data);
    }
}

int search(struct TreeNode *root, int key)
{
    if (root == NULL) return 0;
    if (root->data == key) return 1;
    if (key < root->data) return search(root->left, key);
    return search(root->right, key);
}

void freeTree(struct TreeNode *root)
{
    if (root != NULL)
    {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}

int main()
{
    struct TreeNode *root = NULL;
    int choice, val;

    printf("--- Binary Search Tree (BST) Menu ---\n");
    printf("1. Insert\n");
    printf("2. In-order Traversal (Sorted)\n");
    printf("3. Pre-order Traversal\n");
    printf("4. Post-order Traversal\n");
    printf("5. Search\n");
    printf("6. Exit\n");

    while (1)
    {
        printf("\nEnter choice (1-6): ");
        if (scanf("%d", &choice) != 1) break;

        switch (choice)
        {
            case 1:
                printf("Enter value to insert: ");
                scanf("%d", &val);
                root = insert(root, val);
                break;
            case 2:
                printf("In-order: ");
                inorder(root);
                printf("\n");
                break;
            case 3:
                printf("Pre-order: ");
                preorder(root);
                printf("\n");
                break;
            case 4:
                printf("Post-order: ");
                postorder(root);
                printf("\n");
                break;
            case 5:
                printf("Enter value to search: ");
                scanf("%d", &val);
                if (search(root, val))
                    printf("Found %d in tree!\n", val);
                else
                    printf("%d not found in tree.\n", val);
                break;
            case 6:
                freeTree(root);
                return 0;
            default:
                printf("Invalid choice!\n");
        }
    }

    freeTree(root);
    return 0;
}
