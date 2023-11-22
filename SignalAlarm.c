#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

void intHandler();

int main()
{
	signal(SIGALRM,intHandler);
	alarm(5);
	printf("loop...\n");

	while(1){
		sleep(1);
		printf("1 sec..\n");

	}
	printf("End of main\n");
}

void intHandler(int signo)
{
	printf("Wake up\n");
	exit(0);
}
