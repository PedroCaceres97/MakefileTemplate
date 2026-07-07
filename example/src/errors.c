#include <stdio.h>
#include <stdlib.h>

void after_free() {
    int *p = malloc(sizeof(int));
    free(p);
    *p = 42;
}

void double_free() {
    int *p = malloc(8);
    free(p);
    free(p);
}

void heap_overflow() {
    int *p = malloc(4 * sizeof(int));
    p[4] = 42;
    free(p);
}

void integer_overflow() {
    int x = INT_MAX;
    x++;
}

void misaligned_pointer() {
    char buf[16];
    int *p = (int*)(buf + 1);
    *p = 42;
}

void stack_overflow() {
    int a[4];
    for (int i = 0; i < 10; ++i) { a[i] = i; }
}

void zero_division() {
    int x = 0;
    int y = 1 / x;
}

