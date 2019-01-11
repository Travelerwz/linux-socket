#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>

#define PORT 8080
#define MAX_NUM 128
#define ERROR(m) \
do \
{ \
	perror(m); \
	exit(0); \
}while(0)
	

int Socket()
{
	int fd = 0;
	fd = socket(AF_INET,SOCK_STREAM,0);
	if(fd < 0)
	{
		ERROR("socket");
	}
	return fd;
}



int main()
{
	int sockfd = -1;
	int res = -1;
	char recvbuf[MAX_NUM];
	char sendbuf[MAX_NUM];
	int num = 10;
	pid_t pid;
	struct sockaddr_in cliaddr;
	
	memset(&cliaddr,0,sizeof(cliaddr));
	cliaddr.sin_family = AF_INET;
	////IP地址设置成INADDR_ANY,让系统自动获取本机的IP地址
	cliaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	cliaddr.sin_port = htons(PORT);
	//创建套接字
	sockfd = Socket();

	
	int len = sizeof(cliaddr);
	
	res = connect(sockfd,(struct sockaddr*)&cliaddr,len);
	if(res < 0)
	{
		ERROR("connect");
	}
	printf("--------\n");
	

	while(1)
	{
		
		
		fgets(sendbuf,MAX_NUM,stdin);
		res = send(sockfd,sendbuf,MAX_NUM,0);
		printf("send:%s\n",sendbuf);
		memset(sendbuf,0,sizeof(sendbuf));
		
		res = recv(sockfd,recvbuf,MAX_NUM,0);
		if(res < 0)
		{
			ERROR("recv");
		}
		printf("recv:%s\n",recvbuf);
	}
		
	
	
	close(sockfd);
	return 0;
}

	
