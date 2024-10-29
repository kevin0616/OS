#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<time.h>
#include<unistd.h>
#include<sys/wait.h>
#include<semaphore.h>
#include<sys/mman.h>
#include<sys/stat.h>
#include<fcntl.h>
#define NUM_TRIALS 1000000
#define NUM_PROCS 4
typedef struct{
	int nhits;
	sem_t sem;
} mystr;

int main(int argc, char *argv[]){
	int nhits=0;
	pid_t p1=-1;
	pid_t p2=-1;
	const char *name="OS";
	int shm=shm_open(name,O_CREAT|O_RDWR, 0666);
	ftruncate(shm,sizeof(int));
	int *buf=mmap(NULL,sizeof(int),PROT_READ|PROT_WRITE,MAP_SHARED,shm,0);
	//printf("%d\n", *buf);
	p1=fork();
	p2=fork();
	int n=atoi(argv[1]);
	//parent
	if(p1>0&&p2>0){
		
		wait(0);
		wait(0);
		printf("parent:%d\n", *buf);
		printf("prob for %d is %d%%\n", n, (*buf)*100/(NUM_TRIALS*NUM_PROCS));
	
	}else if(p1==0&&p2>0){
	//c1
		wait(0);
		int count=0;
		srand(time(NULL)+getpid());
		for(int i=0; i<NUM_TRIALS; i++){
			int arr[365]={0};
			for(int j=0; j<n; j++){
				int tmp=rand();
				tmp%=365;
				arr[tmp]+=1;
				if(arr[tmp]>1){
					
					count++;
					
					break;
				}
				//printf("%d\n",tmp);
			}
		}
		*buf+=count;
		printf("c1:%d\n", *buf);
		printf("c1\n");
	}else if(p1>0&&p2==0){
	//c2
		srand(time(NULL)+getpid());
		for(int i=0; i<1000000; i++){
			int arr[365]={0};
			for(int j=0; j<n; j++){
				int tmp=rand();
				tmp%=365;
				arr[tmp]+=1;
				if(arr[tmp]>1){

					*buf++;

					break;
				}
				//printf("%d\n",tmp);
			}
		}
		printf("c2\n");
	}else if(p1==0&&p2==0){
		pid_t p3;
		p3=fork();
		if(p3>0){
		//c3
			wait(0);
			srand(time(NULL)+getpid());
			for(int i=0; i<1000000; i++){
				int arr[365]={0};
				for(int j=0; j<n; j++){
					int tmp=rand();
					tmp%=365;
					arr[tmp]+=1;
					if(arr[tmp]>1){

						*buf++;

						break;
					}
					//printf("%d\n",tmp);
				}
			}
			printf("c3\n");
		}else if(p3==0){
			srand(time(NULL)+getpid());
			for(int i=0; i<1000000; i++){
				int arr[365]={0};
				for(int j=0; j<n; j++){
					int tmp=rand();
					tmp%=365;
					arr[tmp]+=1;
					if(arr[tmp]>1){

						*buf++;

						break;
					}
					//printf("%d\n",tmp);
				}
			}
		//c4
			printf("c4\n");
		}
	}
}
