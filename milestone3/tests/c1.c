#include <stdio.h>

int f (int n) {
  if (n < 2) return n;
  return f (n - 1) * n;
}

int main() {
    int n = 5;
    int fact = f(n);
    printf("%d\n", fact);
    return 0;
}