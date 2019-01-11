/*
功能概述：服务器和客户端可以进行通信，客户端发送信息并接收，
服务器接收信息并返回发送信息
*/

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

void Listen(int fd,int num)
{
	int res = -1;
	res = listen(fd,num);
	if(res < 0)
	{
		ERROR("listen");
	}
	return ;

}



int main()
{
	int sockfd = -1;
	int connect_fd = -1;
	pid_t pid;
	int res = -1;
	char recvbuf[MAX_NUM];
	char sendbuf[MAX_NUM];
	int num = 10;
	struct sockaddr_in seraddr;
	
	memset(&seraddr,0,sizeof(seraddr));
	seraddr.sin_family = AF_INET;
	////IP地址设置成INADDR_ANY,让系统自动获取本机的IP地址
	seraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	seraddr.sin_port = htons(PORT);
	//创建套接字
	sockfd = Socket();
	//端口复用
	int opt = 1;
	if(setsockopt(sockfd, SOL_SOCKET,SO_REUSEADDR, (const void *) &opt, sizeof(opt)))
	{
		ERROR("setsockopt");
	}
	
	//绑定
	if(bind(sockfd,(struct sockaddr*)&seraddr,sizeof(seraddr)) < 0)
	{
		ERROR("bind");
	}
	
	//监听
	Listen(sockfd,num);
	
	//等待客户端连接
	printf("waiting client coming:\n");
	int len = sizeof(seraddr);
	
	while(1)
	{
		connect_fd = accept(sockfd,(struct sockaddr*)&seraddr,&len);
		if(connect_fd < 0)
		{
			ERROR("accept");
		}
		
		printf("--------\n");
		//创建进程
		pid = fork();
		if(pid == 0)
		{
			while(1)
			{
				/*
				fgets(sendbuf,MAX_NUM,stdin);
				res = send(connect_fd,sendbuf,MAX_NUM,0);
				if(res >= 0)
				{
					printf("send:%s\n",sendbuf);
				}
				*/
				res = recv(connect_fd,recvbuf,MAX_NUM,0);
				if(res >= 0)
				{
					printf("recv:%s\n",recvbuf);
				}
				send(connect_fd,recvbuf,MAX_NUM,0);
			}
			close(connect_fd);
			
		}
	}
	
	close(sockfd);
	return 0;
}

	
