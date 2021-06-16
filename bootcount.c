#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/timerfd.h>
#include <stdio.h>
#include <error.h>
#include <semaphore.h>
#include <errno.h>

#define BOOTCOUNT_ADDR 0x7000A000

int main(int argc, char *argv[])
{
	int fid;
       
	fid = open("/dev/mem", O_RDWR);
	if(fid < 0) {
		perror("could not open /dev/mem\n");
		return -1;
	}
	u_int32_t *base = mmap(0, getpagesize(), PROT_READ | PROT_WRITE, 
			MAP_SHARED, fid, BOOTCOUNT_ADDR);
	if(base == MAP_FAILED) {
		perror("MAP_FAILED");
		return -1;
	}

	if(argc == 2) {
		int val;
		val = strtol(argv[1], NULL, 10);
		if((errno != 0) || (val <= 0) || (val > 1000)) {
			printf("invalid %s\n", argv[1]);
			return -1;
		}
		base[0] = val;
	}

        printf("bootcount=%d\n", base[0]);

	return 0;
}

