#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <wait.h>
#include <sys/mman.h>

#define IN (pshared->in)
#define OUT (pshared->out)
#define BUF_SZ 5

//structure definition
typedef struct {
    int in;
    int out;
    long long buf[BUF_SZ];
} shared_struct;

// Main routine
int main(int argc, char *argv[]){
    int n;
    
    // validate the input parameter n
    if(argc != 2){
        printf("usage: ./lab5a <n>\n");
        return -1;
    }

    // validate and obtain the input
    n = atoi(argv[1]);
	if(n<=1){
        printf("usage: ./lab5a <n> -- n must be > 1 \n");
        return -1;
    }

    // create the shared buffer
    shared_struct* pshared = (shared_struct*) mmap(NULL, sizeof(shared_struct), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    
    // initialize indices
    IN = 0;
    OUT = 0;

    // spawn the child process
    pid_t pid = fork();    
    if (pid < 0) {
        perror("Error ");
        return -1;
    }


    // child process
    else if (pid == 0){
        // child process field
        int ii, interval;
		long long prevfib[2] = {1,1};	// history of previous 2 fibs
		long long fib=1;
        
        // set the seed
        srand((unsigned) time(NULL));
        
        for(ii=0;ii<n;ii++){
             // wait for a random interval of time (0 to 1.999 seconds)
            interval = rand() % 3000;
            usleep(interval * 1000);
			
			// generate the fib
			if(ii==0 || ii==1) prevfib[ii]=1;
			else{ // ii is 2 or above
				fib = prevfib[0] + prevfib[1];
				prevfib[1] = prevfib[0];
				prevfib[0] = fib;
			}

            // wait till buffer is not full
            while((IN + 1) % BUF_SZ == OUT);
            
            // produce an item
            pshared->buf[IN] = fib;
            
            //increment the index using module arithmatic
            IN = (IN + 1) % BUF_SZ;
           
        }
    }
        
    // parent process    
    else{
        // parent process field
        int ii;
        
        for(ii=0;ii<n;ii++){
            // wait till buffer is not empty
            while(IN == OUT);
            
            // consume an item
            printf("%lld ", pshared->buf[OUT]);
            fflush(stdout);

            // increment the read index using modulo arithmatic
            OUT = (OUT + 1) % BUF_SZ;
            
        }
        
        printf("\n");
        fflush(stdout);


        //wait the child to finish
        wait(NULL);

        // remove the shared buffer
        munmap(pshared,sizeof(shared_struct));
    
    }
   
    return 0;
}

