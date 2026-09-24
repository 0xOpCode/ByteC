/**
 * ==============================================================================
 * ByteC: Starter Template: String Operations without Library Functions
 * ==============================================================================
 */

#include <stdio.h>

int strLength(const char *s)
{
    int len = 0;
    while (s[len] != '\0') len++;
    return len;
}

void strCopy(char *dest, const char *src)
{
    int i = 0;
    while (src[i] != '\0')
    {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

void strReverse(char *s)
{
    int i = 0, j = strLength(s) - 1;
    while (i < j)
    {
        char temp = s[i];
        s[i] = s[j];
        s[j] = temp;
        i++;
        j--;
    }
}

int strCompare(const char *s1, const char *s2)
{
    int i = 0;
    while (s1[i] != '\0' && s2[i] != '\0')
    {
        if (s1[i] != s2[i])
            return s1[i] - s2[i];
        i++;
    }
    return s1[i] - s2[i];
}

int main()
{
    char str1[100], str2[100], copyStr[100];

    printf("=========================================\n");
    printf("🛡️ ByteC: Custom String Operations Demo\n");
    printf("=========================================\n\n");

    printf("Enter first string: ");
    if (scanf("%99s", str1) != 1) return 1;

    printf("Enter second string: ");
    if (scanf("%99s", str2) != 1) return 1;

    printf("\n1. Length of '%s': %d\n", str1, strLength(str1));
    printf("   Length of '%s': %d\n", str2, strLength(str2));

    strCopy(copyStr, str1);
    printf("2. Copy of string 1: '%s'\n", copyStr);

    int cmp = strCompare(str1, str2);
    if (cmp == 0)
        printf("3. Comparison: Strings are identical.\n");
    else if (cmp > 0)
        printf("3. Comparison: '%s' is greater than '%s'.\n", str1, str2);
    else
        printf("3. Comparison: '%s' is less than '%s'.\n", str1, str2);

    strReverse(str1);
    printf("4. Reversed first string: '%s'\n", str1);

    return 0;
}
