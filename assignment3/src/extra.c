#include "extra.h"

int fact(int x) {
    if (x < 0) {
        return -1;
    }
    if (x == 0) {
        return 1;
    }

    int res = 1;
    for (x; x > 0; x--) {
        // printf("%d\n", i);
        res *= x;
    
    }
    return res;
}

// used and modified a bit of https://www.freecodecamp.org/news/euclidian-gcd-algorithm-greatest-common-divisor/
int gcd(int x, int y) {
    if (x <= 0 || y <= 0)
        return -1;

    int rem;
    while ((x % y) > 0) {
        rem = x % y;
        x = y;
        y = rem;
    }
    return y;
}


int lntwo(int x) {
    return log(x) / log(2);
}
