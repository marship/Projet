#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

long hex2dec (char hex[])
{
    size_t i = 0;
    long dec = 0;
    long temp = 0;

    while (hex[i] != '\0')
    {
        const int isHEXA = (hex[i] >= 'A') && (hex[i] <= 'F');
        const int ishexa = (hex[i] >= 'a') && (hex[i] <= 'f');
        const int isdigit = (hex[i] >= '0') && (hex[i] <= '9');

        if (ishexa)
        {
            temp = hex[i] - 'a' + 10;
        }
        else if (isHEXA)
        {
            temp = hex[i] - 'A' + 10;
        }
        else if (isdigit)
        {
            temp = hex[i] - '0';
        }
        dec = dec * 16 + temp;
        i++;
    }
    return dec;
}