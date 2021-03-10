/**
 * From http://www.ce.uniroma2.it/~pellegrini/
 */

#include <stdlib.h>
#include <stdio.h>

void swap(long *a, long *b)
{
    __asm__ __volatile__(
        "push (%%rax)\n"
        "push (%%rbx)\n"
        "pop (%%rax)\n"
        "pop (%%rbx)"
        : "=a"(a), "=b"(b)
        : "a"(a), "b"(b)
        : "memory");
}

int main(int argc, char **argv)
{
    long a, b;

    if (argc < 3)
    {
        fprintf(stderr, "usage: %s val1 val2\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    a = strtol(argv[1], NULL, 10);
    b = strtol(argv[2], NULL, 10);

    printf("input:\n \tval1 = %ld\n\tval2 = %ld\n", a, b);
    swap(&a, &b);
    printf("swapped:\n\tval1 = %ld\n\tval2 = %ld\n", a, b);
}