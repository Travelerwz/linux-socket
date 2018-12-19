/*  *
	*@author:wangzhuang
	*@describe:创建线程
	*@others:
*/

#include<iostream>
#include<pthread.h>
using namespace std;

const int NUM_THREADS = 5;
pthread_mutex_t mutex;

class Thread
{
public:
	static void* fun1(void *arg)
	{
		pthread_mutex_lock(&mutex);
		int num = *((int*)arg);
		cout<<"hello thread"<<num<<endl;
		pthread_mutex_unlock(&mutex);
	}
};

int main()
{
	int index[NUM_THREADS];//防止被修改
	pthread_t tid[NUM_THREADS];
	pthread_mutex_init(&mutex,NULL);
	for(int i=0;i<NUM_THREADS;i++)
	{
		index[i] = i;
		int ret = pthread_create(&tid[i],NULL,Thread::fun1,(void*)&(index[i]));
		if(0 != ret)
		{
			cout<<"create thread is error"<<endl;
		}
	}
	for(int i=0;i<NUM_THREADS;i++)
	{
		pthread_join(tid[i],NULL);
	}
	pthread_exit(NULL);
	return 0;
}