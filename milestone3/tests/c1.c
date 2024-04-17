#include <stdio.h>
#include <stdbool.h>

int f(int a, int b) {
    return a + b;
}

int main() {
    int a = f(3, 4);
    printf("%d\n", a);
    printf("hello");
    return 0;
}