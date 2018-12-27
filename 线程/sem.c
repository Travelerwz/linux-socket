#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<semaphore.h>
#include<stdlib.h>
#define MAX_NUM 10


sem_t sem;

void* thread(void *arg)
{	
	sem_post(&sem);
	int count = *((int*)arg);
	int res = -1;
	sem_wait(&sem);
	printf("thread %d is closed\n",count);
	

}

int main()
{
	pthread_t tid[MAX_NUM];
	sem_init(&sem,0,0);
	int i=0;
	for(i;i<MAX_NUM;i++)
	{
		int res=-1;
		res = pthread_create(&tid[i],NULL,thread,&tid[i]);
		if(res < 0)
		{
			printf("thread_create is error\n");
			exit(1);
		}
		else
		{
			printf("coming number:%d\n",i);
		}
		sleep(3);
	}
	int j=0;
	for(j;j<MAX_NUM;j++)
	{
		pthread_join(tid[j],NULL);
	}
	sem_destroy(&sem);
	return 0;
}
