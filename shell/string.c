#include "string.h"
#include "math.h"

int strcmp(const char *s1, const char *s2)
{
    int i = 0;
    while (s1[i] && s2[i]) {
        if (s1[i] < s2[i])
            return -1;
        else if (s1[i] > s2[i])
            return 1;
        i++;
    }
    return s1[i] - s2[i];
}

int strncmp(const char *s1, const char *s2, int len)
{
    int i = 0;
    while (i < len) {
        if (s1[i] < s2[i])
            return -1;
        else if (s1[i] > s2[i])
            return 1;
        i++;
    }
    return 0;
}

void memset(char *s, int c, int len)
{
    int i;

    for (i = 0; i < len; i++)
        s[i] = c;
}

int strlen(const char *str)
{
    int i = 0;
    while (str[i])
        i++;
    return i;
}

char *strcpy(char *dest, const char *src)
{
    int i = 0;
    while (src[i]) {
        dest[i] = src[i];
        i++;
    }
    dest[i] = 0;
    return dest;
}

char *strncpy(char *dest, const char *src, int len)
{
    int i = 0;
    int beyond = 0;
    while (i < len) {
        if (beyond == 0) {
            dest[i] = src[i];
            if (src[i] == 0)
                beyond = 1;
        } else {
            dest[i] = 0;
        }
        i++;
    }
    return dest;
}

void itoa(int x, char str[], int d)
{
    int i = 0;
    while (x) {
        str[i++] = (x % 10) + '0';
        x = x / 10;
    }

    // If number of digits required is more, then
    // add 0s at the beginning
    while (i < d)
        str[i++] = '0';

    str[i] = '\0';
    reverse(str);
}

void ftoa(float n, char *res, int afterpoint)
{
    // Extract integer part
    int ipart = (int) n;

    // Extract floating part
    float fpart = n - (float) ipart;

    // convert integer part to string
    itoa(ipart, res, 0);
    int i = strlen(res);

    // check for display option after point
    if (afterpoint != 0) {
        res[i] = '.';  // add dot

        // Get the value of fraction part upto given no.
        // of points after dot. The third parameter
        // is needed to handle cases like 233.007
        fpart = fpart * pow(10, afterpoint);

        itoa((int) fpart, res + i + 1, afterpoint);
    }
}

void reverse(char *str)
{
    int i;
    char temp;

    for (i = 0; i < strlen(str) / 2; ++i) {
        temp = str[strlen(str) - i - 1];
        str[strlen(str) - i - 1] = str[0];
        str[0] = temp;
    }
}