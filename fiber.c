#include <stdlib.h>
#include <stdio.h>
#include <ucontext.h>
#include <errno.h>

#define STACK_SIZE 1600
void func(int);

ucontext_t fcontext, mcontext;
int x = 0;

int main(void)
{
	int value = 3;
	getcontext(&fcontext);
	// TODO: why we need this line, does prinf need some buf?
	printf("test\n");
	if ((fcontext.uc_stack.ss_sp = (char*) malloc(STACK_SIZE)) != NULL)
	{
		fcontext.uc_stack.ss_size = STACK_SIZE;
		fcontext.uc_stack.ss_flags = 0;
		makecontext(&fcontext, (void (*)(void))func, 1, value);
		if (errno != 0)
		{
			perror("Error in makecontext");
			return -1;
		}

	}
	else
	{
		perror("error in malloc");
		return -1;
	}

	printf("context has been built\n");
	swapcontext(&mcontext, &fcontext);
	if (x == 1)
	{
		printf("I am back!\n");
	}
	else
		printf("Something bad happened!\n");

	//return 0;
}

void func(int arg)
{
	asm ("movq $2, -0x4(%rbp);");
	printf("Entering fiber func, the arg is: %d\n", arg);
	x++;
	printf("function returning to main\n");
	setcontext(&mcontext); // set back to main, where we swap context
}
