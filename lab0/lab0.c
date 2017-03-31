#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <getopt.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define CATCH 'c'
#define INPUT 'i'
#define OUTPUT 'o'
#define SEGFAULT 's'

void seg_fault_me() {
	int* seg_faulter = NULL;
	*seg_faulter = 12;
	exit(3);
	//note: exit(3) is only reached when catch has already defined the signal handling
}

void seg_handler(int sig_num) {
	fprintf(stderr, "signal number: %d received, now quitting... \n", sig_num);
	exit(3);
}

int main(int argc, char **argv) {

	//if seg_flag > 0, then execute a segmentation fault
	int seg_flag = 0;
	//if catch_flag = 0, then no input/output/seg ops were called and program should exit(3)
	int catch_flag = 0;

	//long options
	struct option long_options[] = {
		
		{"segfault", no_argument, NULL, SEGFAULT},
		{"catch", no_argument, NULL, CATCH},
		{"input", required_argument, NULL, INPUT},
		{"output", required_argument, NULL, OUTPUT},
		{0, 0, 0, 0}
	};
	int ifd, ofd;
	int ret = 0;
	while (1) {
		ret = getopt_long(argc, argv, "", long_options, NULL);
		if (ret == -1) {
			break;
		}
		fprintf(stderr, "ret: %c, optind: %d, optarg: %s \n", ret, optind, optarg);
		switch (ret) {
			case SEGFAULT:
			{
				seg_flag++;
				break;
			}
			case CATCH:
			{
				signal(SIGSEGV, seg_handler);
				break;
			}
			case INPUT:
			{
				//catch_flag++;
				ifd = open(optarg, O_RDONLY);
				if (ifd >= 0) {
					close(0);
					dup(ifd);
					close(ifd);
				} else {
					perror("Error! Unable to open specified input file");
          			exit(1);
				}
				break;
			}
			case OUTPUT:
			{
				//catch_flag++;
				ofd = creat(optarg, 0666);
				if (ofd >= 0) {
					close(1);
					dup(ofd);
					close(ofd);
				} else {
          			perror("Error! Unable to create specified output file");
          			exit(2);
          		}
				break;
			}
			/*default:
			{
				fprintf(stderr, "Oops! You did not enter arguments in correctly \n ");
				exit(1);
			}*/
		}
	}
	if(seg_flag > 0) {
		seg_fault_me();
	}
	
	/*
	if(catch_flag == 1) {
		fprintf(stderr, "Oops! You did not enter arguments in correctly \n ");
		exit(1);
	}
	
	if(catch_flag == 0) {
		printf("catch_flag == 0 \n");
		exit(0);
	}*/
	
	char buf;
	
	while (read(0, &buf, 1) > 0) {
    write(1, &buf, 1);
  	}
	
	return 0;
} 