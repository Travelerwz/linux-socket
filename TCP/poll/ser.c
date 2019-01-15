/*
1.创建socket
2.绑定套接字
3.监听
4.连接
5.poll 设置结构体
6.poll
*/

#include <stdio.h>
#include <poll.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 9999
#define MAX_NUM 1024
#define ERROR(m) \
do \
{\
	perror(m);\
	exit(1);\
}while(0)
	
int sockfd = -1;
int res = -1;
int confd = -1;
fd_set fs;
char recvbuff[MAX_NUM];
char sendbuf[MAX_NUM];
//1.创建套接字
int Socket()
{
	int fd = -1;
	fd = socket(AF_INET,SOCK_STREAM,0);
	if(fd < 0)
	{
		ERROR("socket");
	}
	return fd;
}


//2.绑定套接字				
void Bind(int fd,const struct sockaddr *addr,socklen_t addrlen)
{
	res = bind(fd,addr,addrlen);
	if(res < 0)
	{
		ERROR("bind");
	}
}

//3监听套接字

void Listen(int fd,int num)
{
	res = listen(fd,num);
	if(res < 0)
	{
		ERROR("listen");
	}
}

int main()
{
	int opt = 1;
	struct timeval tv;
	struct sockaddr_in ser;
	memset(&ser,0,sizeof(ser));
	ser.sin_family = AF_INET;
	ser.sin_port = htons(PORT);
	ser.sin_addr.s_addr = htonl(INADDR_ANY);
	
	sockfd = Socket();
	
	//端口复用
	res = setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,(const void *)&opt,sizeof(opt));
	if(res < 0)
	{
		ERROR("setsockopt");
	}
	Bind(sockfd,(struct sockaddr*)&ser,sizeof(ser));
	Listen(sockfd,10);
	//4.连接
	int len = sizeof(ser);
	confd = accept(sockfd,(struct sockaddr*)&ser,&len);
	if(confd < 0)
	{
		ERROR("accept");
	}
	
	struct pollfd rfds[2];
	rfds[0].fd = confd;
	rfds[0].events = POLLIN;
	
	tv.tv_sec = 10;
	tv.tv_usec = 0;
	
	while(1)
	{
		printf("poll....\n");
		res = poll(rfds,1,-1);
		if(res < 0)
		{
			ERROR("poll");
		}
		else if(res > 0)
		{
			if(rfds[0].revents & POLLIN == POLLIN)
			{
				res = recv(confd,recvbuff,MAX_NUM,0);
				if(res < 0)
				{
					ERROR("recv");
				}
				printf("recv:%s\n",recvbuff);
				
			}
		}
	}
	close(confd);
	close(sockfd);
	
	return 0;
}
