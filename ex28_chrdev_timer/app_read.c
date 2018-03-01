#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define GLOBALSIZE 128
#define PATHNAME "/dev/test_dev"

int main(int argc, const char * argv[])
{
	int fd = -1;
	unsigned int sec,old_sec;
	int ret = 0;

	fd = open(PATHNAME,O_RDWR);
	if(fd < 0){
		perror("open failed.\n");
		return -1;
	}

	printf("open has done.\n");
	while(1){
		ret = read(fd,&sec,sizeof(unsigned int));
			if(old_sec != sec){
				printf("after linux work,sec:%d.\n",sec);
				old_sec = sec;
			}
	}
	close(fd);
	printf("closed.\n");
	return 0;
}
