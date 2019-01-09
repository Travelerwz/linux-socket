/*
无名管道
*/


#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

#define ERROR(m) \
do \
{	\
	perror(m); \
	exit(1);	\
}while(0)
int main()
{
	int fd[2];
	char buf[10];
	pid_t pid;
	int res = pipe(fd);
	if(res < 0)
	{
		ERROR("pipe");
	}
	pid = fork();
	if(pid < 0 )
	{
		ERROR("fork");
	}
	
	else if(pid == 0)
	{
		close(fd[0]);
		write(fd[1],"hello",10);
	}
	else
	{
		close(fd[1]);
		read(fd[0],buf,10);
		printf("%s\n",buf);
	}
	return 0;
}
