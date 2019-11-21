//
// Created by Ruize Tang on 2019/11/8.
//

#include <stdio.h>

int addition (int a, int b)
{ return a + b; }

int subtraction (int a, int b)
{ return a - b; }

int multiplication(int a, int b)
{ return a * b; }

int operation (int x, int y, int (*func)(int, int)) {
    int g;
    g = (*func)(x, y);
    return g;
}

int main () {
    int m, n;
    char t;
    int (*minus)(int,int) = subtraction;
    int (*funcs[])(int, int) = {multiplication, addition};

    m = operation (8, 3, addition);
    n = operation (20, m, minus);

    printf("%d %d\n", m, n);

    while (scanf("%d %d %c", &m, &n, &t) == 3)
        printf("%d %c %d = %d\n", m, t, n, funcs[t - '*'](m, n));

    return 0;
}
