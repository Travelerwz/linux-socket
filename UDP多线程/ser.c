/* server_udp.c:  */
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define MAX_MSG_SIZE 1024
#define SERVER_PORT 3000

char s_msg[MAX_MSG_SIZE];//发消息
char receive[MAX_MSG_SIZE];//收消息
char bye[MAX_MSG_SIZE]="bye\n";

struct sockaddr_in srvaddr,clientaddr;  
int sockfd,addrlen;

void send_msg(void);//线程函数，发消息

int main()
{
     pthread_t pt_send = 0;
     int ret;      
    sockfd=socket(AF_INET,SOCK_DGRAM,0);
    
    if(sockfd==-1)//套接字描述符
        {
            printf("Socket create error\n");
            return -1;
        }
        
    //assign server's sock address
    memset(&srvaddr, 0, sizeof(srvaddr));
    addrlen=sizeof(srvaddr);
    srvaddr.sin_family=AF_INET;
    srvaddr.sin_port=htons(SERVER_PORT);
    if(inet_aton("127.0.0.1",&srvaddr.sin_addr)==-1)
        {
            printf("addr convert error\n");
            close(sockfd);
            return -1;
     }
    //srvaddr.sin_addr.s_addr=htonl(INADDR_ANY);

    //bind IP address and port
    if(bind(sockfd,(struct sockaddr *)&srvaddr,addrlen)==-1)
        {
            printf("bind error\n");
            close(sockfd);
            return -1;
        }
        
    printf("input bye to end SERVER\n");
    ret = pthread_create (&pt_send, NULL, (void *)send_msg, NULL);  
    if (ret != 0)      
        printf("pthread send_msg creat error\n"); 
    while(1) 
    {
        memset(receive, 0, MAX_MSG_SIZE);
        recvfrom(sockfd, receive, MAX_MSG_SIZE, 0, (struct sockaddr *)(&clientaddr), &addrlen);//非阻塞
        if (receive[0] != '\0' )
            fprintf(stdout, "Receive message from client: %s", receive);
        if (strcmp(receive,bye) == 0)
        break;
    }
    //while收消息，显示到标准输出，发消息，从标准如入写
    //close socket
    pthread_join (pt_send, NULL);
    close(sockfd);
    return 0;
}
void send_msg(void)
{
    while(1)
    {
        memset(s_msg, 0, MAX_MSG_SIZE);
        //get message from standard input
        fgets(s_msg, MAX_MSG_SIZE, stdin);
        //send message to client
        sendto(sockfd,s_msg,strlen(s_msg),0,(struct sockaddr *)(&clientaddr),addrlen);
        printf("Server input message: %s", s_msg);
        if (strcmp(s_msg, bye) == 0)
            {
                printf("end session\n");
                break;
            }    
    }
}