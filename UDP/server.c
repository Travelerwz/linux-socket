#include<stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<netdb.h>
#include<stdlib.h>
#include<errno.h>
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
	int res = -1;
	int cli_len = 0;
	char send_buf[BUFFER];
	int recv = -1;
	struct sockaddr_in ser,cli;
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
	int on = 1;
	int ret = -1;
	ret = setsockopt( socketfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on) );
	if(ret < 0)
	{
		ERROR_EXIT("setsockopt");
	}
	len = sizeof(struct sockaddr);
	int b = -1;
	b = bind(socketfd,(const struct sockaddr*)&ser,len);
	if(b < 0)
	{
		ERROR_EXIT("bind");
	}
	//printf("IP:%s,port %d\n",SERVER_IP,PORT);
	
	cli_len = sizeof(cli);
	while(1)
	{
		printf(" server is waiting:\n");
		char recv_send_buf[BUFFER];
		char send_buf[BUFFER];
		
		res = recvfrom(socketfd,recv_send_buf,sizeof(recv_send_buf),0,(struct sockaddr*)&cli,&cli_len);
		if(res < 0)
		{
			ERROR_EXIT("recvfrom");
		}
		else
		{
			printf("from client data:%s\n",recv_send_buf);
		}
		if(NULL != fgets(send_buf,sizeof(send_buf),stdin))
		{
			res = sendto(socketfd,send_buf,sizeof(send_buf),0,(struct sockaddr*)&cli,cli_len);
			if(res < 0)
			{
				ERROR_EXIT("sendto");
			
			}
			else
			{
				printf("sendto client:%s\n",send_buf);
			}
		}
	}
	close(socketfd);
	return 0;
}
