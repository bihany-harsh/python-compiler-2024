#include <stdio.h>
#include <stdbool.h>

int f(int a, int b, int c, int d, int e, int f, int g, int h) {
    int x = 5*8 + 6;
    int y = a + b + c + d + e + f;
    int z = (e + 4)*(f + 2) + g * d + f*c;
    int w = 5;
    int v = 9;
    // printf("%d\n", x);
    return y;
}


int main() {
    printf("%d\n", f(5, 20, 1, 2, 3, 4, 6, 67));
    return 0;
}