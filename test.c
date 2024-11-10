#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>

#define READ_BYTES 1

int main(){
	int fd=open("/dev/lab8", O_RDWR);
	printf("%d\n",fd);
	printf("my userid is %d\n", getuid());
	
	char buf[READ_BYTES+1];
	int tmp;
	while(tmp=read(fd, buf, READ_BYTES)){
		buf[tmp]='\0';
		printf("%d %s\n",tmp,buf);
	}
	return 0;
}
