#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

void intHandler();
void intHandler2();

int main()
{
	signal(SIGALRM,intHandler);
	signal(SIGINT,intHandler2);
	alarm(10);
	
	int correct=0;
	while(!correct)
	{
		if(getchar()=='1')
			if(getchar()=='2')
				if(getchar()=='3')
					if(getchar()=='\n')
						correct=1;
	}

	printf("Correct\n");
	return 0;
}

void intHandler(int signo)
{
	printf("Fire\n");
	exit(0);
}

void intHandler2(int signo)
{
	printf("10 Sec reset..\n");
	alarm(10);
}


