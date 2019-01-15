/*
1.创建socket
2.绑定套接字
3.监听
4.连接
5.epoll设置结构体
6.epoll
*/

#include <stdio.h>
#include <sys/epoll.h> 
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
	
	
	tv.tv_sec = 10;
	tv.tv_usec = 0;
	//epoll
	struct epoll_event event;
	struct epoll_event wait_event[1];
	
	int epfd = epoll_create(10);
	if(epfd < 0)
	{
		ERROR("epoll_create");
	}
	
	event.data.fd = confd;
	event.events = EPOLLIN;
	int ctr = epoll_ctl(epfd,EPOLL_CTL_ADD,confd,&event);
	if(ctr < 0)
	{
		ERROR("epoll_ctl");
	}
	
	
	while(1)
	{
		printf("epoll");
		res = epoll_wait(epfd,wait_event,1,-1);
		if(res < 0)
		{
			ERROR("epoll_create");
		}
		else if(res > 0)
		{
			int i=0;
			for(i=0;i<res;i++)
			{
				if(wait_event[i].data.fd == confd &&(wait_event[i].events & EPOLLIN)==EPOLLIN)
				{
					recv(confd,recvbuff,MAX_NUM,0);
					printf("recv:%s\n",recvbuff);
				}
			}
		}
		else
		{
			printf("time out\n");
		}
	}
	close(confd);
	close(sockfd);
	
	return 0;
}
