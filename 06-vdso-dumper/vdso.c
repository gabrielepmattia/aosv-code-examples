/**
 * vDSO Dumper
 *
 * USAGE:
 * - compile this file using `gcc vdso.c`
 * - run as `./a.out > vdso.o`: this creates a dump of the vDSO
 *   shared object in memory
 * - Inspect the vDSO using `objdump -d vdso.o | less`
 */

#include <sys/auxv.h>
#include <stdio.h>
#include <unistd.h>

#define PAGE_SIZE 4096

int main(int argc, char **argv){
	void *p = (void *)getauxval(AT_SYSINFO_EHDR);
	fprintf(stderr, "vDSO is loaded at %p\n", p);
	write(1, p, 4*PAGE_SIZE);
}
