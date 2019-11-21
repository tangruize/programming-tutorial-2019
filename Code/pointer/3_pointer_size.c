//
// Created by Ruize Tang on 2019/11/8.
//

#include <stdio.h>

int main() {
    char *mychar = (char *)0x1000;    // point to address: 0x1000
    short *myshort = (short *)0x2000; // point to address: 0x2000
    int *myint = (int *)0x3000;       // point to address: 0x3000
    ++mychar;
    ++myshort;
    ++myint;
    printf("mychar: %p\n", (void *)mychar);
    printf("myshort: %p\n", (void *)myshort);
    printf("myint: %p\n", (void *)myint);
}
