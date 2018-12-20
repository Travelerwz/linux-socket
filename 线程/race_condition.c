/*
	*@param
	*@param
	*@param 数据竞争引发条件竞争的典型例子
*/

#include<stdio.h>
#include<pthread.h>
#include<unistd.h>

int i = 1;

void* mythread(void* arg)
{
	if(i == 1)
	{
		sleep(3);
		if(i == 2)
		{
			printf("hack it\n");
		}
		else
		{
			printf("you can try again\n");
		}
	}
}

void* mythread2(void* arg)
{
	sleep(1);
	i = 2;
}

int main()
{
	
	pthread_t tid1,tid2;
	pthread_create(&tid1,NULL,mythread,NULL);
	pthread_create(&tid2,NULL,mythread2,NULL);
	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);
	
	return 0;
}
