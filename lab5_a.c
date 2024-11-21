#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<sys/shm.h>
#include<sys/stat.h>
#include<sys/mman.h>
#include<unistd.h>
#include<sys/wait.h>
#include<time.h>
#define BUF_SZ 5

int main(int argc, char *argv[]){
	int n;
	sscanf(argv[1], "%d", &n);
	
	const char *name = "OS";
	const int SIZE = BUF_SZ*sizeof(long long);
	void *ptr;
	int shm_fd;
	
	int in = 0;
	int out = 0;
	long long buf[BUF_SZ];
	
	shm_fd = shm_open(name, O_CREAT|O_RDWR, 0666);	
	ftruncate(shm_fd, SIZE);
	
	/*the last 8 bytes are for in and out and the rest are for buf.*/
	ptr = mmap(0, SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, shm_fd, 0);
	
	/*initial in and out as 0*/
	*(int*)(ptr+sizeof(long long)*4)=in;
	*(int*)(ptr+sizeof(long long)*4+sizeof(int))=out;
	
	pid_t pid;
	pid=fork();
	
	if(pid>0){
		for(int i=0; i<n; i++){
			/*get in & out value from shared memory.*/
			in = *(int*)(ptr+sizeof(long long)*4);
			out = *(int*)(ptr+sizeof(long long)*4+sizeof(int));
			
			while(in==out){
				/*refresh in & out value in every while loop.*/
				in = *(int*)(ptr+sizeof(long long)*4);
				out = *(int*)(ptr+sizeof(long long)*4+sizeof(int));
			};
			
			/*this is the address of buf[out] in shared memory.*/
			long long result = *(long long *)(ptr+sizeof(long long)*out);
			
			/*(out=out+1)%(BUF_SZ-1)*/
			/*because the last 8 bytes can not be used but are for in and out. */
			*(int*)(ptr+sizeof(long long)*4+sizeof(int))=(out+1)%(BUF_SZ-1);
			
			printf("output=%lld\n", result);	
			
		}
		fflush(stdout);
		shm_unlink(name);
	}
	else if(pid==0){
		long long fib[999999];
		fib[0]=0;
		fib[1]=1;
		for(int i=2; i<n; i++){
			fib[i]=fib[i-1]+fib[i-2];
		}
		for(int i=0; i<n; i++){
			/*get in & out value from shared memory.*/
			in = *(int*)(ptr+sizeof(long long)*4);
			out = *(int*)(ptr+sizeof(long long)*4+sizeof(int));
			
			while((in+1)%(BUF_SZ-1)==out){
				/*refresh in & out value in every while loop.*/
				in = *(int*)(ptr+sizeof(long long)*4);
				out = *(int*)(ptr+sizeof(long long)*4+sizeof(int));
			};
			
			/*this is the address of buf[in] in shared memory.*/
			*(long long *)(ptr+sizeof(long long)*in) = fib[i];

			/*in=(in+1)%BUF_SZ*/
			*(int*)(ptr+sizeof(long long)*4)=(in+1)%(BUF_SZ-1);
			
			/*sleep for random time from 0 to 2 seconds.*/
			srand(time(NULL));
			int r = rand();
			sleep(r%3);	
		}
	}
	
	return 0;
}

