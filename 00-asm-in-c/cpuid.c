/**
 * @file cpuid.c
 * @author Gabriele Proietti Mattia (pm.gabriele@outlook.com)
 * @brief 
 * @version 0.1
 * @date 2021-03-10
 * 
 * @copyright GPL license (c) 2021
 * 
 */
#include <stdio.h>
#include <stdint.h>

#define FEATURE_INFO 1

/**
 * @brief 
 * 
 * @param code 
 * @param a eax register content
 * @param d edx register content
 */
void cpuid(int code, uint32_t *a, uint32_t *c, uint32_t *d)
{
    __asm__("cpuid"
            : "=c"(*c), "=d"(*d)
            : "a"(code));
}

/**
 * @brief Test a single bit in the given register using btl
 * 
 * @param bitn # of the bit to test
 * @param reg register value
 * @return int 1 or 0
 */
int testbit(int bitn, uint32_t *reg)
{
    int res;
    __asm__ __volatile__(
        "btl %0, %%ebx;"
        "jnc .false;"
        "movl $1, %%eax;"
        "jmp .end;"
        ".false:;"
        "movl $0, %%eax;"
        ".end:;"
        : "=a"(res)
        : "a"(bitn), "b"(*reg));
    return res;
}

int main()
{
    uint32_t a;
    uint32_t c;
    uint32_t d;

    cpuid(FEATURE_INFO, &a, &c, &d);

    printf("EAX = %x\n", a);
    printf("ECX = %x\n", c);
    printf("EDX = %x\n", d);

    printf("\n");
    // check SSE4.1
    printf("SSE4.1 = %d\n", testbit(19, &c));
    printf("SSE4.2 = %d\n", testbit(20, &c));
    printf("x2APIC = %d\n", testbit(21, &c));
}