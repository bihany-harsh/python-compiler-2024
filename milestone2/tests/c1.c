#include <stdio.h>
#include <stdbool.h>

int f(int a, int b) {
    int c = a + b;
    return c;
}


int main() {
    printf("%d\n", f(5, 5));
    return 0;
}