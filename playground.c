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
typedef struct{
    int x;
    int y;
}Pair;

int main(){
    Pair* p = malloc(sizeof(Pair));
    printf("%d", p);
    free(p);
    printf("%d", p);
    return 0;
}