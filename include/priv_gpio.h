#ifndef PRIV_GPIO_H_
#define PRIV_GPIO_H_

#define GPIO_MODE 0
#define GPIO_TYPE 1
#define GPIO_PUPD 2
#define GPIO_SPEED 3
#define GPIO_OUT 4

void gpio(uint32_t call, uint32_t pin, uint32_t value) 
{
	register uint32_t r0 asm("r0") = call;
	register uint32_t r1 asm("r1") = pin;
	register uint32_t r2 asm("r2") = value;

	asm("\
		mov r0, %0; \
		mov r1, %1; \
		mov r2, %2; \
		svc 1; \
	" :: "r" (r0), "r" (r1), "r" (r2));
}

#endif // PRIV_GPIO_H_
