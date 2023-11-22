#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

#define MAXLINE 256

struct ThreadInfo {
    int id;
    char readFifo[20];
    char writeFifo[20];
};

void writeToLogFile(char* message){

	FILE *file=fopen("chat_log.txt","a");
	if(file==NULL)
	{
		perror("Error opening File\n");
		exit(10);
	}
	fprintf(file,"%s\n",message);
	fclose(file);
}

char* readLogFile(){

	FILE* file=fopen("chat_log.txt","r");
	if(file==NULL)
	{
		perror("Error openinf File\n");
		return NULL;
	}

	fseek(file,0,SEEK_END);
	long fileSize=ftell(file);
	rewind(file);

	char* content=(char*)malloc(fileSize+1);
	if(content==NULL)
	{
		perror("failed to malloc\n");
		fclose(file);
		return NULL;
	}

	size_t bytesRead=fread(content,1,fileSize,file);
	if(bytesRead!=fileSize)
	{
		perror("failed to read file\n");
		free(content);
		fclose(file);
		return NULL;
	}

	content[fileSize]='\0';

	fclose(file);
	return content;

	
}
void* ReceiveAndSend(void* argument) {
    struct ThreadInfo* info = (struct ThreadInfo*)argument;

    int fd[2];
    int n;

    printf("%d is id\n", info->id);

    if (mkfifo(info->readFifo, 0660) == -1) {
        if (errno != EEXIST) {
            perror("mkfifo");
            exit(info->id + 1);
        }
    }
    if (mkfifo(info->writeFifo, 0660) == -1) {
        if (errno != EEXIST) {
            perror("mkfifo");
            exit(info->id + 3);
        }
    }

    printf("%d mkfifo is completed\n", info->id);

    // FIFO 파일 열기
    fd[0] = open(info->readFifo, O_RDONLY);
    fd[1] = open(info->writeFifo, O_WRONLY);

    if (fd[0] == -1 || fd[1] == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    printf("connected%d\n", info->id);

    char msg[MAXLINE];
    char* fileContent=readLogFile();
    write(fd[1],fileContent,strlen(fileContent)+1);
    while (1) {
        n = read(fd[0], msg, MAXLINE);

    
        if (n == -1 && errno == EAGAIN) {
            continue;
        } else if (n <= 0) {
            break;
        }

	if(info->id==1)
	{
		char formatMsg[strlen("[Client1}: ")+strlen(msg)+1];
		strcpy(formatMsg,"[Client1]: ");
		strcat(formatMsg,msg);
		strcpy(msg,formatMsg);
	}
	
	else{
		char formatMsg[strlen("[Client2]: ")+strlen(msg)+1];
		strcpy(formatMsg,"[Client2]: ");
		strcat(formatMsg,msg);
		strcpy(msg,formatMsg);
	}

        write(fd[1],msg,strlen(msg)+1);
	writeToLogFile(msg);
    }
    close(fd[0]);
    close(fd[1]);
    return NULL;
}

int main() {
    pthread_t threads[2];
    struct ThreadInfo info1 = {1, "./chatfifo1", "./chatfifo4"};
    struct ThreadInfo info2 = {2, "./chatfifo3", "./chatfifo2"};

    printf("서버 테스트 출력\n");

    // 스레드를 생성할 때 각각 다른 구조체 주소 전달
    pthread_create(&threads[0], NULL, ReceiveAndSend, (void*)&info1);
    pthread_create(&threads[1], NULL, ReceiveAndSend, (void*)&info2);

    printf("excute Thread\n");
    // 여기서 main 스레드가 종료되지 않고 기다리도록 하기 위해 스레드 조인 추가
    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);

    return 0;
}

