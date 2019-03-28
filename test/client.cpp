#include <netinet/in.h>  
#include <sys/socket.h>  
#include <stdio.h>  
#include <string.h>  
#include <stdlib.h>  
#include <sys/epoll.h>  
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <iostream>
#include <fcntl.h>

#define IP_ADDRESS   "127.0.0.1"  
#define SERV_PORT   8888

int setSocketNonBlocking(int fd)
{
    int flag = fcntl(fd, F_GETFL, 0);
    if(flag == -1)
        return -1;

    flag |= O_NONBLOCK;
    if(fcntl(fd, F_SETFL, flag) == -1)
        return -1;
    return 0;
}

int main(int argc, char *argv[])
{
    struct sockaddr_in servAddr;  
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);  
    bzero(&servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, IP_ADDRESS, &servAddr.sin_addr);
    char buff[4096];
    memset(buff, '\0', sizeof(buff));
    char *sendStr = "Hello Server!";
    
    if (connect(sockfd, (struct sockaddr*)&servAddr, sizeof(servAddr)) == 0)
    {
        setSocketNonBlocking(sockfd);
        ssize_t n = write(sockfd, sendStr, strlen(sendStr));
        sleep(1);
        n = read(sockfd, buff, 4096);
        printf("buff = %s\n", buff);
        close(sockfd);
    }
    
    return 0;
}