#include <stdio.h>
#include <stdbool.h>

int f(int a, int b, int c, int d, int e, int f, int g, int h) {
    int x = 10;
    int y = x * 2;
    int z = x * 3;
    int v = x * 4;
    return a * x;
}

int main() {
    int a = 4;
    int b = 5;
    f(1 + a, 2 + a, 3 + b, 4 + a, 5 + a, 6 + a, 7 + a, 8 + a);
    // // printf("hello world");
    // return 0;
    char* n = "abc";
    // printf("%s\n", n);
    return 0;
}