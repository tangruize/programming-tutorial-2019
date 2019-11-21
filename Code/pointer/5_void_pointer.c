//
// Created by Ruize Tang on 2019/11/8.
//

#include <stdio.h>

void increase(void *data, int psize) {
    if (psize == sizeof(char)) {
        char *pchar;
        pchar = (char *) data;
        ++(*pchar);
    } else if (psize == sizeof(int)) {
        int *pint;
        pint = (int *) data;
        ++(*pint);
    }
}

int main() {
    char a = 'A';
    int b = 1108;
    increase(&a, sizeof(a));
    increase(&b, sizeof(b));
    printf("%c %d\n", a, b);
    return 0;
}
