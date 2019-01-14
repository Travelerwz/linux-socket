#include <stdio.h>
#include <sys/select.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
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
char sendbuf[MAX_NUM];
char recvbuff[MAX_NUM];
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
	struct sockaddr_in cli;
	memset(&cli,0,sizeof(cli));
	cli.sin_family = AF_INET;
	cli.sin_port = htons(PORT);
	cli.sin_addr.s_addr = htonl(INADDR_ANY);
	
	sockfd = Socket();
	
	res = connect(sockfd,(struct sockaddr *)&cli,sizeof(cli));
	if(res < 0)
	{
		ERROR("connect");
	}
	
	while(1)
	{
		fgets(sendbuf,MAX_NUM,stdin);
		res = send(sockfd,sendbuf,MAX_NUM,0);
		if(res < 0)
		{
			ERROR("send");
		}
		printf("send:%s\n",sendbuf);
		
		int n = recv(sockfd,recvbuff,MAX_NUM,0);
		if(n < 0)
		{
			ERROR("recv");
		}
		printf("recv:%s\n",recvbuff);
	}

	close(sockfd);
	
	return 0;
}
