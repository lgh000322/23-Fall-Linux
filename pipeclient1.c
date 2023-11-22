#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#define MAXLINE 256

void* Receive(void* argument)
{
    int* read_fd = (int*)argument;
    int n;
    char inmsg[MAXLINE];

    while (1)
    {
        n = read(*read_fd, inmsg, MAXLINE);
        if (n <= 0) {
            perror("read");
            exit(1);
        }
        printf("\n%s\n", inmsg);
    }
}

int main()
{
    int* read_fd = malloc(sizeof(int));  // 메모리 할당
    pthread_t thread;
    int fd1, fd2, n;
    char inmsg[MAXLINE];

    fd1 = open("./chatfifo1", O_WRONLY);
    fd2 = open("./chatfifo2", O_RDONLY);
    
    // 파일 디스크립터 확인
    if (fd1 == -1 || fd2 == -1)
    {
        perror("open");
        exit(1);
    }

    *read_fd = fd2;  // 포인터에 파일 디스크립터 할당
    pthread_create(&thread, NULL, Receive, (void*)read_fd);

    while (1)
    {
        fgets(inmsg, MAXLINE, stdin);
        write(fd1, inmsg, strlen(inmsg) + 1);
    }

    // 무한 루프를 빠져나가면서 쓰레드 종료
    pthread_join(thread, NULL);

    // 메모리 해제
    free(read_fd);

    return 0;
}
