#ifndef ONLINE_JUDGE
#include "redirect.h"
#endif

#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
#ifndef ONLINE_JUDGE
    init(argc, argv);
#endif

    int sum = 0, i;
    while (cin >> i)
        sum += i;
    cout << sum << endl;
    return 0;
}
