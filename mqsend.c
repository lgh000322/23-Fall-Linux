#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

int main(int argc,char* argv[])
{
	char buf[50];
	struct mq_attr attr;
	attr.mq_maxmsg=10;
	attr.mq_msgsize=50;

	mqd_t mq;
	mq=mq_open("/mymq",O_RDWR|O_CREAT,0666,&attr);

	if(mq==-1){
		perror("mq_open()");
		exit(1);
	}

	sprintf(buf,"Hello World\n");

	if(mq_send(mq,buf,50,1)==-1){
		perror("mq_send()");
		exit(2);
	}

	mq_close(mq);
	return 0;
}
