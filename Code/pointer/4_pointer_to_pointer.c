//
// Created by Ruize Tang on 2019/11/8.
//

#include <stdio.h>

int main() {
    char a;
    char *b = NULL;
    char **c;
    a = 'x';
    b = &a;
    c = &b;

    printf("%c\n", **c);
    return 0;
}
