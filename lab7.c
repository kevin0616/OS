#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
#include<pthread.h>

#define NUM_THREADS 4
#define NUM_TRIALS 100000

pthread_t tid[NUM_THREADS];
int nhits=0;
int n;
pthread_mutex_t lock;

void* WorkerThread(void* pvar){
	/* random seed for each thread */
	unsigned int rand_state = (unsigned int)time(NULL) + pthread_self();
	
	for(int i=0; i<NUM_TRIALS; i++){
		int collide = 0;
		int count[365]={0};
		
		for(int j=0; j<n; j++){
			if(collide==1){
				break;
			}
			/* generate random day between 0 and 364 */
			int tmp = (rand_r(&rand_state))%365;

			if(count[tmp]>0){
			
				/* use mutex lock for writing shared variable */
				pthread_mutex_lock(&lock);
				nhits+=1;
				pthread_mutex_unlock(&lock);
				/* mutex unlock */
				
				collide=1;
			}else{
				count[tmp]+=1;
			}
		}
	}
	
	

	return NULL;
}

int main(int argc, char *argv[]){
	
	sscanf(argv[1], "%d", &n);
	
	/*initial mutex lock */
	pthread_mutex_init(&lock, NULL);
	
	for(int i=0; i<NUM_THREADS; i++){
		pthread_create(&(tid[i]),NULL, &WorkerThread, NULL);
	}
	
	for(int i=0; i<NUM_THREADS; i++){
		pthread_join(tid[i], NULL);
	}
	
	float p = (float)nhits/(NUM_THREADS*NUM_TRIALS)*100;
	printf("the probablility for n = %d is %f%%\n", n, p);
	
	pthread_mutex_destroy(&lock);
	
	return 0;
}

