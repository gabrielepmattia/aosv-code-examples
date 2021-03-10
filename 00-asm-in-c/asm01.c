/**
 * @file asm01.c
 * @author Gabriele Proietti Mattia (pm.gabriele@outlook.com)
 * @brief 
 * @version 0.1
 * @date 2021-03-10
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <stdio.h>

/**
 * @brief Basic assembly with immediate
 * ! a single % refer to an operand, a double %% refers to a register
 */
void asm01()
{
    int val;
    __asm__ __volatile__(
        "movl %1, %%eax;" // write %1 -> 14 in eax
        "movl %%eax, %0;" // write eax in val
        : "=r"(val)       // output
        : "i"(14)         // input
        : "%eax");

    printf("val = %d\n", val);
}

/**
 * @brief Basic assembly with a stack variable
 * 
 * @param a An integer
 */
void asm02()
{
    int in = 100;
    int val;
    __asm__ __volatile__(
        "movl %1, %%eax;" // write %1 -> 14 in eax
        "movl %%eax, %0;" // write eax in val
        : "=r"(val)       // output
        : "r"(in)         // input
        : "%eax");

    printf("val = %d\n", val);
}

/**
 * @brief Basic assembly with add from stack
 * 
 * @param a An integer
 */
void asm03(int num1, int num2)
{
    int sum;
    __asm__ __volatile__(
        "addl %%ebx, %%eax;"
        : "=a"(sum)
        : "a"(num1), "b"(num2));

    printf("sum = %d\n", sum);
}

int main()
{
    printf("asm01\n");
    asm01();
    printf("\n");

    printf("asm02\n");
    asm02();
    printf("\n");

    printf("asm03\n");
    asm03(4, 3);
}
