/**
 * ==============================================================================
 * ByteC: Starter Template: Graph Traversals (Adjacency Matrix BFS and DFS)
 * ==============================================================================
 */

#include <stdio.h>

#define MAX_VERTICES 20

void initGraph(int adj[MAX_VERTICES][MAX_VERTICES], int vertices)
{
    for (int i = 0; i < vertices; i++)
    {
        for (int j = 0; j < vertices; j++)
        {
            adj[i][j] = 0;
        }
    }
}

void addEdge(int adj[MAX_VERTICES][MAX_VERTICES], int u, int v)
{
    adj[u][v] = 1;
    adj[v][u] = 1;
}

void bfs(int adj[MAX_VERTICES][MAX_VERTICES], int vertices, int start)
{
    int visited[MAX_VERTICES] = {0};
    int queue[MAX_VERTICES];
    int front = 0;
    int rear = 0;

    visited[start] = 1;
    queue[rear++] = start;

    printf("BFS Traversal: ");
    while (front < rear)
    {
        int curr = queue[front++];
        printf("%d ", curr);

        for (int v = 0; v < vertices; v++)
        {
            if (adj[curr][v] == 1 && visited[v] == 0)
            {
                visited[v] = 1;
                queue[rear++] = v;
            }
        }
    }
    printf("\n");
}

void dfsVisit(int adj[MAX_VERTICES][MAX_VERTICES], int vertices, int curr, int visited[])
{
    visited[curr] = 1;
    printf("%d ", curr);

    for (int v = 0; v < vertices; v++)
    {
        if (adj[curr][v] == 1 && visited[v] == 0)
        {
            dfsVisit(adj, vertices, v, visited);
        }
    }
}

void dfs(int adj[MAX_VERTICES][MAX_VERTICES], int vertices, int start)
{
    int visited[MAX_VERTICES] = {0};
    printf("DFS Traversal: ");
    dfsVisit(adj, vertices, start, visited);
    printf("\n");
}

int main(void)
{
    int adj[MAX_VERTICES][MAX_VERTICES];
    int vertices, edges, start;

    printf("--- Graph Traversal Lab (BFS & DFS) ---\n");
    printf("Enter number of vertices (1-%d): ", MAX_VERTICES);
    if (scanf("%d", &vertices) != 1 || vertices <= 0 || vertices > MAX_VERTICES)
    {
        printf("Invalid vertex count.\n");
        return 1;
    }

    initGraph(adj, vertices);

    printf("Enter number of undirected edges: ");
    if (scanf("%d", &edges) != 1 || edges < 0)
    {
        return 1;
    }

    for (int i = 0; i < edges; i++)
    {
        int u, v;
        printf("Edge %d (u v): ", i + 1);
        if (scanf("%d %d", &u, &v) == 2 && u >= 0 && u < vertices && v >= 0 && v < vertices)
        {
            addEdge(adj, u, v);
        }
        else
        {
            printf("Invalid vertex index.\n");
            return 1;
        }
    }

    printf("Enter starting vertex (0-%d): ", vertices - 1);
    if (scanf("%d", &start) != 1 || start < 0 || start >= vertices)
    {
        printf("Invalid starting vertex.\n");
        return 1;
    }

    bfs(adj, vertices, start);
    dfs(adj, vertices, start);

    return 0;
}
