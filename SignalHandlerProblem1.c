#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int child_id;
void intHandler();
void intHandler2();
void intHandler3();

void print_gugu()
{
	int i,j;
	for(i=1;i<=20;i++)
	{
		for(j=1;j<=20;j++)
		{				
			printf("%d x %d = %d\n",i,j,i*j);
		}
		if(i==9)
			signal(SIGINT,intHandler3);

		sleep(1);
		
	}
}
int main()
{
	int pid;
	int status;
	if((pid=fork())==0)
	{	
		signal(SIGINT,intHandler2);
		print_gugu();
		pause();
	}
	else{
		child_id=pid;
		signal(SIGINT,intHandler);
		wait(&status);
	}
}

void intHandler(int signo)
{
	kill(child_id,SIGINT);
	
}
void intHandler2(int signo)
{
	printf("can't terminate..\n");
}
void intHandler3(int signo)
{
	kill(-getppid(),SIGKILL);
}
