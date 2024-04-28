#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <string.h>
void printBits(size_t const size, void const * const ptr)
{
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;

    for (i = size-1; i >= 0; i--) {
        for (j = 7; j >= 0; j--) {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
    }
    puts("");
}
int main(){
    unsigned char x = 255;
    signed char y = -1;
    int a = (signed char) x;
    int b = y;
    printBits(sizeof(x), &x);
    printBits(sizeof(y), &y);
    printBits(sizeof(a), &a);
    printBits(sizeof(b), &b);

    printf("%b %b\n", x, y);
    printf("%d %d\n", a, b);
    return 0;
}