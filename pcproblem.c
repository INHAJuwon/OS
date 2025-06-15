#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX 30
#define ITER 1000
int x;
sem_t m; // 상호배제를 위한 binary semaphore
sem_t fill; // 초기 값이 0으로 설정되어 소비자가 wait될 때 사용
sem_t empty; // 초기 값이 MAX로 설정되어 생산자가 wait될 때 사용

void* thread_increment(void* arg){
	int i,val;
	for(int i=0;i<ITER;i++){
		sem_wait(&empty); //empty값을 1감소시키며, empty가 0이하라면 잠겨있는 상태니 go to sleep
		sem_wait(&m); // 전역 변수 x가 사용되는 critical section이기에 상호배제를 위한 lock
		val=x;
		printf("%u: %d\n",(unsigned int)pthread_self(),val);
		x=val+1;
		sem_post(&m); // unlock
		sem_post(&fill); //fill을 1 증가시키며, sleep 중이던 소비자를 wake
	}
	return NULL;
}

void* thread_decrement(void* arg){
	int i,val;
	for(int i=0;i<ITER;i++){
		sem_wait(&fill); //fill값을 1감소시키며, fill이 0이하라면 잠겨있는 상태니 go to sleep
		sem_wait(&m); // 전역 변수 x가 사용되는 critical section이기에 상호배제를 위한 lock
		val=x;
		printf("%u: %d\n",(unsigned int)pthread_self(),val);
		x=val-1;
		sem_post(&m); //unlock
		sem_post(&empty); //empty을 1 증가시키며, sleep 중이던 생산자자를 wake
	}
	return NULL;
}


int main(){
	pthread_t  t1,t2;

	sem_init(&m,0,1); //상호 배제를 위해 1로 초기화
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


