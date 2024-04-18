#include <stdio.h>
#include <stdbool.h>
#include <string.h>


int main() {
    char a[] = "Hello World, this is Swarnendu and I am conducting Compiler Design Course at IIT Kanpur aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    char b[] = "hello world, this is Debadatta and I am conducting OS    at IIT Kanpur bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";

    printf("%d\n", strcmp(b, a));
}