char c;
char v[1024];

/**
 * @brief Read char by char from the stdin (0)
 * 
 * Remind:
 * 	ssize_t read(int fd, void *buf, size_t count);
 * 
 * @param s 
 */
void instring(char *s)
{
	int i = 0;

	do
	{
#ifndef m32
		asm volatile(
			"syscall;"
			:
			: "a"(0), "D"(0), "S"((unsigned long)(&c)), "d"((long)(1)));
#else
		asm volatile("int $0x80"
					 :
					 : "a"(3), "b"(0), "c"((unsigned long)(&c)), "d"((long)(1)));
#endif

		s[i] = c;
		i++;
	} while ((c != '\n') && (c != ' ') && (c != '\t'));

	i--;
	s[i] = '\0';
}

/**
 * @brief Write char by char to stdout (1)
 * 
 * Remind: 
 * 	ssize_t write(int fd, const void *buf, size_t count);
 * 
 * @param s 
 */
void outstring(char *s)
{
	int i = 0;
	while (s[i++] != '\0')
		;
	i--;

#ifndef m32
	asm volatile(
		"syscall;"
		:
		: "a"(1), "D"(1), "S"((unsigned long)(s)), "d"((long)(i)));
#else
	asm volatile("int $0x80"
				 :
				 : "a"(4), "b"(1), "c"((unsigned long)(s)), "d"((long)(i)));
#endif
}

/**
 * @brief _start is the actual entry point of your C program
 * 
 * @param argc 
 * @return int 
 */
int _start(int argc)
{
	while (1)
	{
		instring(v);
		outstring(v);
		outstring("\n");
	}
}
