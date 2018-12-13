#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>          
#include <sys/socket.h>
#include <netinet/in.h>
#include<string.h>


#define BUFFER 1024
#define PORT 8888

#define ERROR_EXIT(m) \
do \
{\
	perror(m);\
	exit(1);\
}while(0)
int main()
{
	int len = 0;
	char buf[BUFFER];
	int recv = -1;
	int res = -1;
	struct sockaddr_in ser;
	bzero(&ser,sizeof(ser));
	ser.sin_family = AF_INET;
	ser.sin_port = htons(PORT);
	ser.sin_addr.s_addr = htonl(INADDR_ANY);

	int socketfd  = -1;
	//建立socket
	socketfd = socket(AF_INET,SOCK_DGRAM,0);
	if(socketfd < 0)
	{
		ERROR_EXIT("socket");
	
	}
	len = sizeof(struct sockaddr);
	while(1)
	{
		printf("waiting---->\n");
		char recv_buf[512];
		char send_buf[512];
		
		if(NULL != fgets(recv_buf,sizeof(recv_buf),stdin))
		{
			
			res = sendto(socketfd,recv_buf,strlen(recv_buf),0,(struct sockaddr*)&ser,sizeof(struct sockaddr));
			if(res < 0)
				ERROR_EXIT("recvfrom");
			else
				printf("send data:%s\n",recv_buf);
		}
		res = recvfrom(socketfd,send_buf,strlen(send_buf),0,(struct sockaddr*)&ser,&len);
		if(res < 0)
		{
			ERROR_EXIT("recvfrom");
		}
		else
		{
			printf("recv from ser :%s\n",send_buf);
		}
		
	}
	close(socketfd);
	return 0;
}
