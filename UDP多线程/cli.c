/* client_udp.c: */
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

#define MAX_BUF_SIZE 1024
struct sockaddr_in srvaddr,clientaddr;
int sockfd,port,addrlen;
char s_msg[MAX_BUF_SIZE];
char receive[MAX_BUF_SIZE];
char bye[MAX_BUF_SIZE] = "bye\n";
void send_msg(void);//线程函数，发消息

int main(int argc, char *argv[])
{
    pthread_t pt_send = 0;
     int ret;      
    //bye="bye\n";
    if (argc!=3) {
        printf("Usage:%s server_ip server_port\n", argv[0]);
        return -1;
    }
    
    //convert port number
    if ((port=atoi(argv[2]))<0) {
        printf("port error\n");
        return -1;
    }

    //create client udp socket
    sockfd=socket(AF_INET,SOCK_DGRAM,0);
    if(sockfd==-1){
        printf("Socket create error\n");
        return -1;
    }
    
    //assign server's sock address
    addrlen=sizeof(struct sockaddr_in);
    memset(&srvaddr, 0, sizeof(srvaddr));
    srvaddr.sin_family=AF_INET;
    srvaddr.sin_port=htons(port);
    //convert IP address
    if(inet_aton(argv[1],&srvaddr.sin_addr)<0){
        printf("IP error\n");
        close(sockfd);
        return -1;
    }
    printf("input bye to end CLIENT\n");
    ret = pthread_create (&pt_send, NULL, (void *)send_msg, NULL);  
    if (ret != 0)      
        printf("pthread send_msg creat error\n"); 
    while(1) 
    {
        memset(receive,0, MAX_BUF_SIZE);    
        //receive message from remote server and print it
        recvfrom(sockfd, receive, MAX_BUF_SIZE, MSG_DONTWAIT, (struct sockaddr *)(&clientaddr), &addrlen);
        if (receive[0] != '\0' )
            fprintf(stdout, "Receive message from server: %s", receive);
        if (strcmp(receive,bye) == 0)
            {
                //printf("end session\n");
                break;
            }
    }
    
    //close socket
    pthread_join (pt_send, NULL);
    close(sockfd);
    return 0;
}
void send_msg(void)
{
    while(1)
    {
    memset(s_msg, 0, MAX_BUF_SIZE);

    //get message from standard input
    fgets(s_msg, MAX_BUF_SIZE, stdin);

    //send message to remote server and print it
    sendto(sockfd,s_msg,strlen(s_msg),MSG_DONTWAIT,(struct sockaddr *)(&srvaddr),addrlen);
    printf("Client input message: %s", s_msg);    
    if (strcmp(s_msg, bye) == 0)
            {
                printf("end session\n");
                break;
            }    
    }
}