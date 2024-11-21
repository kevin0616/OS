#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#include<time.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<wait.h>
#include<sys/mman.h>

#define IN (p->in)
#define OUT (p->out)
#define BUF_SZ 5

typedef struct {
    int in;
    int out;
    long long buf[BUF_SZ];
} shared_struct;
int n=-1;

int main(int argc, char *argv[]){
    
    n = atoi(argv[1]);
    printf("address of n: %p\n", &n);
    int shm_fd;
    int SIZE=BUF_SZ*sizeof(shared_struct);
	
    shm_fd = shm_open("/lab9_shm", O_CREAT|O_RDWR, 0666);
    ftruncate(shm_fd, SIZE);
    
    shared_struct* p = (shared_struct*) mmap(NULL, sizeof(shared_struct), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    printf("address of shared memory: %p\n", p);
    
    IN = 0;
    OUT = 0;
    long long prevfib[2] = {1,1};
    long long fib=1;
    
    srand(time(NULL));
    
    for(int i=0;i<n;i++){
    	int r = rand();
    	sleep(r%3);
	if(i==0 || i==1) prevfib[i]=1;
	else{
		fib = prevfib[0] + prevfib[1];
		prevfib[1] = prevfib[0];
		prevfib[0] = fib;
	}
        while((IN + 1) % BUF_SZ == OUT);
        p->buf[IN] = fib;
        IN = (IN + 1) % BUF_SZ;
    }
    munmap(p, sizeof(shared_struct));
    return 0;
}

