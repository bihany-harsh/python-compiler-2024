#include <stdio.h>
#include <stdbool.h>
// #include <string.h>

int f(int a, int b) {
    return a + b;
}


int main() {
    printf("%d\n", f(1, 2));

    char s[] = "hello";
    printf("hello world\n");
    printf("%s\n", s);

    return 0;
    // printf("%d\n", strcmp(b, a));
}