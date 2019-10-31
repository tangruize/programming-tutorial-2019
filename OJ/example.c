#ifndef ONLINE_JUDGE
#include "redirect.h"
#endif

#include <stdio.h>

int main(int argc, char *argv[]) {
#ifndef ONLINE_JUDGE
    init(argc, argv);
#endif

    int sum = 0, i;
    while (scanf("%d", &i) == 1)
        sum += i;
    printf("%d\n", sum);
    return 0;
}
