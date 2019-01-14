/*
1.创建socket
2.绑定套接字
3.监听
4.连接
5.FD_XX(1.清零 2.设置 )
6.select
7.检查是否有消息到来（FD_ISSET为真，则消息到来）
*/

#include <stdio.h>
#include <sys/select.h>
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
	
	while(1)
	{
		FD_ZERO(&fs);
		FD_SET(confd,&fs);
		FD_SET(0,&fs);
		tv.tv_sec = 10;
		tv.tv_usec = 0;
		
		int maxfd = confd > 0?confd:0;
		res = select(maxfd+1,&fs,NULL,NULL,NULL);
		if(res < 0)
		{
			ERROR("select");
		}
		else if(res > 0)
		{
			if(FD_ISSET(confd,&fs))
			{
				int n = recv(confd,recvbuff,MAX_NUM,0);
				if(n < 0)
				{
					ERROR("recv");
				}
				printf("recv:%s\n",recvbuff);
			}
			else if(FD_ISSET(0,&fs))
			{
				fgets(sendbuf,MAX_NUM,stdin);
				int n = send(confd,sendbuf,MAX_NUM,0);
				if(n < 0)
				{
					ERROR("send");
				}
				printf("send:%s\n",sendbuf);
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
