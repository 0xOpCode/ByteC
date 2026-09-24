/**
 * ==============================================================================
 * ByteC: Starter Template: Hello World
 * ==============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    printf("=========================================\n");
    printf("🚀 Hello from ByteC Pocket C Environment!\n");
    printf("=========================================\n");

    if (argc > 1)
    {
        printf("Arguments passed: %d\n", argc - 1);
        for (int i = 1; i < argc; i++)
        {
            printf("  Arg %d: %s\n", i, argv[i]);
        }
    }

    return 0;
}
