#include <stdio.h>

/**
 * @brief Basic assembly with immediate
 * 
 * @param a An integer
 */
void asm01()
{
    int val;
    __asm__ __volatile__(
        "movl %1, %%eax;" // write %1 -> 14 in eax
        "movl %%eax, %0;"  // write eax in val
        : "=r"(val) // output
        : "i"(14) // input
        : "%eax");

    printf("val = %d\n", val);
}

/**
 * @brief Basic assembly with variable
 * 
 * @param a An integer
 */
void asm02()
{   
    int in = 100;
    int val;
    __asm__ __volatile__(
        "movl %1, %%eax;" // write %1 -> 14 in eax
        "movl %%eax, %0;"  // write eax in val
        : "=r"(val) // output
        : "r"(in) // input
        : "%eax");

    printf("val = %d\n", val);
}

int main()
{
    printf("asm01\n");
    asm01();
    printf("\n");
    
    printf("asm01\n");
    asm02();
    printf("\n");
}
