#include <stdio.h>
#include <stdlib.h>

int main(void);

void _start(void)
{
	exit(main());
}

int main(void)
{
	printf("Hello, world!\n");
	return 0;
}

