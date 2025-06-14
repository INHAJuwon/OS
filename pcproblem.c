#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX 30
#define ITER 1000
int x;
sem_t m;
sem_t fill;
sem_t empty;

void* thread_increment(void* arg){
	int i,val;
	for(int i=0;i<ITER;i++){
		sem_wait(&empty);
		sem_wait(&m);
		val=x;
		printf("%u: %d\n",(unsigned int)pthread_self(),val);
		x=val+1;
		sem_post(&m);
		sem_post(&fill);
	}
	return NULL;
}

void* thread_decrement(void* arg){
	int i,val;
	for(int i=0;i<ITER;i++){
		sem_wait(&fill);
		sem_wait(&m);
		val=x;
		printf("%u: %d\n",(unsigned int)pthread_self(),val);
		x=val-1;
		sem_post(&m);
		sem_post(&empty);
	}
	return NULL;
}


int main(){
	pthread_t  t1,t2;

	sem_init(&m,0,1);
	sem_init(&fill,0,0);
	sem_init(&empty,0,MAX);
	
	pthread_create(&t1,NULL,thread_increment,NULL);
	pthread_create(&t2,NULL,thread_decrement,NULL);

	pthread_join(t1,NULL);
	pthread_join(t2,NULL);

	if(x==0)
		printf("OK counter=%d\n", x);
	else
		printf("BOOM! counter=%d\n", x);

	return 0;
}


