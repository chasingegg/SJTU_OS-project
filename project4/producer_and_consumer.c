#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<semaphore.h>

typedef int buffer_item;
#define BUFFER_SIZE 5
#define NUM 100 //maximum num of producer_threads or consumer_threads
buffer_item buffer[BUFFER_SIZE]; //the shared buffer

sem_t empty, full;
sem_t mutex;
int in = 0, out = 0; //in = first empty buffer, out = first full buffer
int pOrder[NUM], cOrder[NUM]; //every producer and consumer has an id(from 1 to NUM)

int counter1 =0;
int counter2 = 0;

void *producer(void *param)
{
    buffer_item ran;
    int id = *(int *) param;

    while(1)
    {
        sleep(rand() % 5);
        ran = rand() % 1000;
		sem_wait(&empty);
		sem_wait(&mutex);
		
		buffer[in] = ran;
		++counter1;
		
		printf("producer %d produced %d\tto buffer[%d]  %d\n", id, ran, in, counter1); 

		in = (in + 1) % BUFFER_SIZE;
		sem_post(&mutex);
		sem_post(&full);
          
    }
    return NULL;
    
}

void *consumer(void *param)
{
    buffer_item ran;
	int id = *(int *) param;

    while(1)
    {
        sleep(rand() % 5);
       
		sem_wait(&full);
		sem_wait(&mutex);
		ran = buffer[out];
		
		++counter2;
		
		printf("consumer %d consumed %d\tin buffer[%d]  %d\n", id, ran, out, counter2);

		out = (out + 1) % BUFFER_SIZE;
		sem_post(&mutex);
		sem_post(&empty);
		
    }
    return NULL;
    
}

void callError()
{
	perror("create thread error!\n");
	exit(0);
}

int main(int argc, char *argv[])
{
    int pNum, cNum, sleepTime, i, ret;
    pthread_t p_thread[NUM];
    pthread_t c_thread[NUM];
	
	//input error
	if(argc != 4)
	{
		printf("error input!\n");
		exit(-1);
	}
	
	//read from the command line
    sleepTime = atoi(argv[1]);
    pNum = atoi(argv[2]);
    cNum = atoi(argv[3]);

	//semaphore initialization 
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
	sem_init(&mutex, 0, 1);
    
    
	//create pthreads
    for(i = 0; i < pNum; ++i)
    {
		pOrder[i] = 1 + i;
        ret = pthread_create(&p_thread[i], NULL, producer, &pOrder[i]);
		if(ret != 0)
			callError();
    }
    for(i = 0; i < cNum; ++i)
    {
		cOrder[i] = 1 + i;
        ret = pthread_create(&c_thread[i], NULL, consumer, &cOrder[i]);
		if(ret != 0)
			callError();
    }
    
    sleep(sleepTime);
    return 0;
    
}
