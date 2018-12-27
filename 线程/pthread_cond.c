/*
	*@param:
	*@param:
	*@ 线程条件变量的使用
*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

pthread_mutex_t cond_mutex;
pthread_cond_t count;
int num = 0;

void* thread_one(void* arg)
{
	pthread_mutex_lock(&cond_mutex);
	while(num == 0)
	{
		printf("thread_one:num==0\n");
		//进入阻塞
		printf("thread_one before wait:\n");
		pthread_cond_wait(&count,&cond_mutex);
		printf("thread_one after wait:\n");
		
	}
	num = num+1;
	pthread_mutex_unlock(&cond_mutex);
}

void* thread_two(void* arg)
{
	pthread_mutex_lock(&cond_mutex);
	
	printf("get cond_mutex:\n");
	
	if(num == 0)
	{
		printf("thread_two:num==0\n");
		printf("thread_two before wait:\n");
		//解除阻塞
		pthread_cond_signal(&count);
		printf("thread_two after wait:\n");
	}
	num = num+1;
	printf("after thread_two if :%d\n",num);
	pthread_mutex_unlock(&cond_mutex);
}

int main()
{
	pthread_t tid1,tid2;
	pthread_mutex_init(&cond_mutex,NULL);
	pthread_cond_init(&count,NULL);
	
	pthread_create(&tid1,NULL,thread_one,NULL);
	sleep(2);
	pthread_create(&tid2,NULL,thread_two,NULL);
	
	sleep(10);
	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);
	
	return 0;
}
