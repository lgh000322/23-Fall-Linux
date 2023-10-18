#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
int S[100],A[100],B[100];

void *TaskCode(void *argument)
{
	int tid;
	tid=*((int*)argument);
	for(int i=tid*25;i<(tid+1)*25;i++)
	{
		S[i]=A[i]+B[i];
	}
	return NULL;
}

int main(int argc,char* argv[])
{
	pthread_t threads[4];
	int args[4];
	int i;
	for(int k=0;k<100;k++)
	{
		A[k]=k;
		B[k]=k;
	}
	for(i=0;i<4;i++)
	{
		args[i]=i;
		pthread_create(&threads[i],NULL,TaskCode,(void*)&args[i]);
	}

	for(int i=0;i<4;i++)
	{
		pthread_join(threads[i],NULL);
	}

	for(int k=0;k<100;k++)
		printf("%d\n",S[k]);

	return 0;
}
