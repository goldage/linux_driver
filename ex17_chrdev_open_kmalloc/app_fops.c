#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

//#define PATHNAME "/dev/ex2"
//#define PATHNAME "/dev/test_device2"
#define PATHNAME "/dev/test_dev"

int main(int argc , const char * argv[]){
	int fd = -1;
	fd = open(PATHNAME , O_RDWR);
	if(fd < 0){
		perror("open failed.\n");
		return -1;
	}

	printf("open has done.\n");
	close(fd);
	return 0;
}
