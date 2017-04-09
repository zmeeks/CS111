#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <getopt.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <fcntl.h>
#include <time.h>

#define PIPE 'a'
#define COMMAND 'b'
#define WAIT 'c'
#define CLOSE 'd'
#define VERBOSE 'e'
#define PROFILE 'f'
#define ABORT 'g'
#define CATCH 'h'
#define IGNORE 'i'
#define DEFAULT 'j'
#define PAUSE 'k'

#ifndef O_RSYNC
#define O_RSYNC O_SYNC
#endif

int verbose_flag;
int wait_flag;	/*see readme for notes on wait_flag and wait() */
int open_flags;
int profile_flag;
int *file_array; //array that shall map actions of simpsh. see below note for more info
int size_of_fa; //size_of_file_array
int fa_index;     //current location (aka "index") wrt file_array
int *pid_array;		//array to keep track of child process ids that parent should wait for
int pa_index;		
int size_of_pa;
char ***cmd_list;
int *cmd_size_array;
int max_exit;
int pipe_fds[2];
struct timeval main_prev_usr, main_prev_sys, main_cur_usr, main_cur_sys;
struct timeval child_prev_usr, child_prev_sys, child_cur_usr, child_cur_sys;

/* 	note on file_array : recall that open() maps files to file_descriptors.  the file_array
	just keeps track of where things are mapped. this could lead to complicated mappings if
	the --close N option allowed that fd to be reused, however the specs account for this
	so that after --close N, that fd shall not be allowed to be again mapped to. hence the
	next available index in file_array shall always correspond to the next fd that open() 
	will map to */
	
struct timeval tv_subtract(struct timeval cur, struct timeval prev) {
	struct timeval update;
	long int msec_compare = cur.tv_usec - prev.tv_usec;	
	if(msec_compare < 0) {
		msec_compare += 1000000;
		update.tv_sec = cur.tv_sec - prev.tv_sec - 1;
		update.tv_usec = msec_compare;
	} else {
		update.tv_sec = cur.tv_sec - prev.tv_sec;
		update.tv_usec = cur.tv_usec - prev.tv_usec;
	}		
	return update;
}	
	
void pipe_function() {
	int zero; //used for error check	
	if ((size_of_fa -3) <= fa_index) {
		size_of_fa *= 2;
		file_array = realloc(file_array, size_of_fa);
		if (file_array == NULL) {
			fprintf(stderr, "ERROR: realloc failed! \n");
			exit(2);
		}
	}	
	zero = pipe(pipe_fds);	
	if(zero != 0) {
		printf("ERROR: in pipe function! \n");
		exit(errno);
	}	
	file_array[fa_index++] = pipe_fds[0];
	file_array[fa_index++] = pipe_fds[1];	
	if(verbose_flag) {
		printf("--pipe \n");
		fflush(stdout);
	}
}	

void print_usage(int who) {
	struct rusage cur;
	int zero;
	struct timeval current_usr, current_sys;
	zero = getrusage(who, &cur); 
	if(zero != 0) {
		printf("ERROR in getrusage! \n");
		exit(errno);
	}
	current_usr = cur.ru_utime;
	current_sys = cur.ru_stime;	
	if(who == RUSAGE_SELF) {
		main_cur_usr = tv_subtract(current_usr, main_prev_usr);
		main_cur_sys = tv_subtract(current_sys, main_prev_sys);		
		printf("total user time for all processes          = %ld.%06ld \n", current_usr.tv_sec, current_usr.tv_usec);
		printf("specific user time for current process     = %ld.%06ld \n", main_cur_usr.tv_sec, main_cur_usr.tv_usec);
		printf("total system time for all processes        = %ld.%06ld \n", current_sys.tv_sec, current_sys.tv_usec);
		printf("specific system time for current process   = %ld.%06ld \n\n", main_cur_sys.tv_sec, main_cur_sys.tv_usec);		
		main_prev_usr = current_usr;
		main_prev_sys = current_sys;
	} else {
		child_cur_usr = tv_subtract(current_usr, child_prev_usr);
		child_cur_sys = tv_subtract(current_sys, child_prev_sys);	
		printf("total child time for all processes         = %ld.%06ld \n", current_usr.tv_sec, current_usr.tv_usec);
		printf("specific child time for current process    = %ld.%06ld \n", child_cur_usr.tv_sec, child_cur_usr.tv_usec);
		printf("total system time for this child processes = %ld.%06ld \n", current_sys.tv_sec, current_sys.tv_usec);
		printf("specific system time for the child process = %ld.%06ld \n\n", child_cur_sys.tv_sec, child_cur_sys.tv_usec);	
		child_prev_usr = current_usr;
		child_prev_sys = current_sys;
	}
}

void wait_function() {
	int i, j, k;
	int pid;
	int status;
	int cmd_spot;
	int exit_code;
	if(verbose_flag){
		printf("--wait \n");
		fflush(stdout);
	}	
	for(j=0; j < fa_index; j++) {
			close(file_array[j]); 
			/**
			 *	note:  	for all j, file_array[j] should != fd[k] for k=0,1,2
			 *			this necessary so that pipes don't cause subprocess to stay open
			 **/
		}		
	for(i=0; i < pa_index; i++) {
		pid=wait(&status);			
			if(profile_flag) 
					print_usage(RUSAGE_CHILDREN);				
		exit_code = WEXITSTATUS(status);
		printf("%d ", exit_code);
		for(j=0; j< pa_index; j++) {
			if(pid_array[j] == pid)
				break;
		}
		for(k=0; k<cmd_size_array[j]; k++) {
			printf("%s ", cmd_list[j][k]);
		}
		printf("\n");
		fflush(stdout);	
		max_exit = exit_code > max_exit ? exit_code : max_exit;
	}	
	/* note: status is just for use with macros */	
	if(profile_flag){ 
		printf("~ parent process time for --wait: ~\n");
		print_usage(RUSAGE_SELF);
	}		
}

void abort_me() {
	if(verbose_flag) {
		printf("--abort \n");
		fflush(stdout);
	}
	int* seg_fault_me = NULL;
	*seg_fault_me = 12;
	exit(3);
	//note: exit(3) is only reached when catch has already defined the signal handling
}

void signal_handler(int sigNum) {
	fprintf(stderr, "%d caught \n", sigNum);
	exit(sigNum + 128);		
}

void open_function() {
	file_array[fa_index] = open(optarg, open_flags, 0644);
	//note: 0644 is of type mode_t and is only made use of when o_creat is one of the open_flags
	if (file_array[fa_index] < 0) {
		fprintf(stderr, "ERROR: opening file %s ! \n", optarg);
		exit(1);
	}
	if (verbose_flag) {
		if ((open_flags & O_RDWR) == O_RDWR) 
			printf("--rdwr %s \n", optarg);
		else if ((open_flags & O_WRONLY) == O_WRONLY)
			printf("--wronly %s \n", optarg);
		else if ((open_flags & O_RDONLY) == O_RDONLY)
			printf("--rdonly %s \n", optarg);
		else {
			fprintf(stderr, "EROOR: file is neither readable nor writable.\n");
			exit(1);  // note--will never hit this else statement!
		}
		fflush(stdout);
		/* note: lowest two bits in open_flags correspond to o_rdonly, o_wronly, and 
		o_rdwr, hence they are not mutually exclusive and require checking o_rdwr first */
	}
	++fa_index;
	open_flags = 0;
	//increment of file_array_index and reset of open_flags
}

void command_function (char **argv) {
	int oi = optind;	/* oi exists for easier to read code */
	int index_cmd = oi;
	int i, o, e, j; //i=input, o=output, e=error, j=index_j
	int pid;	
	while(1) {
		char *string = argv[index_cmd];
		if (string == NULL || string[0] == 0)
			break;
		else if (string[0] == '-' && string[1] == '-') 
			break;	
		/*  counting how many args follow the --command */		
		++index_cmd;
	}
	if (index_cmd - optind + 1 < 4) { 
		fprintf(stderr, "ERROR: not enough cmd args given!\n");
		exit(1);
	}		
	pid = fork();		// FORK HERE //
	if (pid == 0) {	
		i = atoi(argv[oi]);
		o = atoi(argv[oi + 1]);
		e = atoi(argv[oi + 2]);		
		if ( i > fa_index || o > fa_index || e > fa_index || file_array[i] == -1 || file_array[o] == -1 || file_array[e] == -1) {
			fprintf(stderr, "ERROR: trying to access a file descriptor that does not exist\n");
			exit(3);
		}
		int cmd_size = index_cmd - oi - 3; // if indexes are 1 2 3 then 3 - 1 = 2 which is off by minus one  //
		//but then index_cmd is off by a plus one since it is incremented before the stopping condition      //
		//they thus cancel each other out and we need only account for the difference of three ( i o e )     //	
		char **cmdv=malloc(sizeof(char*) * (cmd_size + 1)); // +1 to account for NULL termination	
		for(j=0; j< cmd_size; j++) 
			cmdv[j]=argv[oi + 3 + j];			
		cmdv[cmd_size] = NULL;		
		if(verbose_flag) {
			printf("--command %d %d %d ", i, o, e);
			for(j=0; j< cmd_size; j++) 
				printf("%s ", cmdv[j]);  //argv[oi + 3 + j]);
			printf("\n");
			fflush(stdout);
		}					
		int saved_err = dup(2);	//recall file descriptor 2 is STD_ERR	
			/* note: old_fd and new_fd in the man page are not obvious.  read man pg carefully */	
		if ( (dup2(file_array[i], 0) < 0) || (dup2(file_array[o], 1) < 0) || (dup2(file_array[e], 2) < 0) ) {
			dup2(saved_err, 2);
			fprintf(stderr, "ERROR: error in dup2\n");
			exit(errno);
		}  
		for(j=0; j < fa_index; j++) {
			close(file_array[j]); 
			/**
			 *	note:  	for all j, file_array[j] should != fd[k] for k=0,1,2
			 *			this necessary so that pipes don't cause subprocess to stay open
			 **/
		}		
		execvp(cmdv[0], cmdv);		// EXECVP HERE //
		dup2(saved_err, 2);
		fprintf(stderr, "ERROR in execvp! this print stmnt shouldn't have printed!\n");
		exit(errno);		
	}
	else if (pid < 0) {
		fprintf(stderr, "ERROR: fork failed! \n");
		exit(errno);
	}	
	//***        parent process        ***//	
	pid_array[pa_index] = pid;
	/**
	 *	note: pid here is actually the process id of the child 
	 **/	 
	int cmd_size = index_cmd - oi - 3; // if indexes are 1 2 3 then 3 - 1 = 2 which is off by minus one
		//but then index_cmd is off by a plus one since it is incremented before the stopping condition
		//they thus cancel each other out and we need only account for the difference of three ( i o e )		
		char **cmdv=malloc(sizeof(char*) * (cmd_size + 1)); // +1 to account for NULL termination	
		for(j=0; j< cmd_size; j++) 
			cmdv[j]=argv[oi + 3 + j];			
		cmdv[cmd_size] = NULL;		
		cmd_list[pa_index] = cmdv;		
		cmd_size_array[pa_index] = cmd_size;		
		++pa_index;		
		if(profile_flag)
			printf("~ parent process time for --command: ~\n");		
}

int main(int argc, char **argv) 
{
	verbose_flag = 0;
	wait_flag = 0;
	open_flags = 0;
	profile_flag = 0;
	fa_index = 0;
	size_of_fa = 16;
	size_of_pa = 16;
	pa_index = 0;
	int ret = 0;
	max_exit = 0;	
	//initializing structs
	struct timeval main_prev_usr, main_prev_sys, main_cur_usr, main_cur_sys;
	struct timeval child_prev_usr, child_prev_sys, child_cur_usr, child_cur_sys;
	main_prev_usr.tv_sec = 0;
	main_prev_usr.tv_usec = 0;
	main_prev_sys.tv_sec = 0;
	main_prev_sys.tv_usec = 0;
	child_prev_usr.tv_sec = 0;
	child_prev_usr.tv_usec = 0;
	child_prev_sys.tv_sec = 0;
	child_prev_sys.tv_usec = 0;
	//mallocs	
	file_array = malloc(size_of_fa * sizeof(int));
	pid_array = malloc(size_of_pa * sizeof(int));
	cmd_list = malloc(size_of_pa * sizeof(char**));
	cmd_size_array = malloc(size_of_pa * sizeof(int));
	//long options
	struct option long_options[] = {	
		{"append", no_argument, NULL, O_APPEND},
		{"cloexec", no_argument, NULL, O_CLOEXEC},
		{"creat", no_argument, NULL, O_CREAT},
		{"directory", no_argument, NULL, O_DIRECTORY},
		{"dsync", no_argument, NULL, O_DSYNC},
		{"excl", no_argument, NULL, O_EXCL}, 
		{"nofollow", no_argument, NULL, O_NOFOLLOW},
		{"nonblock", no_argument, NULL, O_NONBLOCK},
		{"rsync", no_argument, NULL, O_RSYNC},
		{"sync", no_argument, NULL, O_SYNC},
		{"trunc", no_argument, NULL, O_TRUNC},
		{"rdonly", required_argument, NULL, O_RDONLY},
		{"rdwr", required_argument, NULL, O_RDWR},
		{"wronly", required_argument, NULL, O_WRONLY},
		{"pipe", no_argument, NULL, PIPE},
		{"command", no_argument, NULL, COMMAND}, //actually req 4 args...
		{"wait", no_argument, NULL, WAIT},
		{"close", required_argument, NULL, CLOSE},
		{"verbose", no_argument, NULL, VERBOSE},
		{"profile", no_argument, NULL, PROFILE},
		{"abort", no_argument, NULL, ABORT},
		{"catch", required_argument, NULL, CATCH},
		{"ignore", required_argument, NULL, IGNORE},
		{"default", required_argument, NULL, DEFAULT},
		{"pause", no_argument, NULL, PAUSE},		
		{0, 0, 0, 0}
	};	
	while (1) {
		ret = getopt_long(argc, argv, "", long_options, NULL);	
		if (ret == -1) {
			break;
		}
		switch (ret) {
			case O_APPEND:
			case O_CLOEXEC:
			case O_CREAT:
			case O_DIRECTORY:
			case O_DSYNC:
			case O_EXCL:
			case O_NOFOLLOW:
			case O_NONBLOCK:
			//case O_RSYNC: //most linux distributions don't define this separately in their C library //note: Redhat is an exception //
			case O_SYNC:
			case O_TRUNC:
			{
				if (verbose_flag) {
					printf("%s ", argv[optind -1]);
					fflush(stdout);
					//note: optind defined in <unistd.h>; optind is index to NEXT option 
					//optind starts at 1 bc argv[0] is the executable.  by this point in the 
					//code optind will already have been incremented, hence [optind - 1]
				}
				open_flags |= ret; 
					/* int open_flags is a bitwise manipulated integer */
				break;
			}
			case O_RDONLY:
			case O_RDWR:
			case O_WRONLY:
			{
				open_flags |= ret;
				open_function(); 
				/* note: verbose and opening of file handled in open_function() */
				if(profile_flag) 
					print_usage(RUSAGE_SELF);
				break;
			}
			case PIPE:
			{
				pipe_function();
				if(profile_flag) 
					print_usage(RUSAGE_SELF);
				break;
			}
			case COMMAND:
			{
				command_function(argv); //argv here is the argv passed in main()
				//note: verbose and execution of command handled in command_function()
				if(profile_flag) 
					print_usage(RUSAGE_SELF);
				break;
			}
			case WAIT:
			{
				wait_flag = 1;
				break;
			}
			case CLOSE:
			{
				int N = atoi(optarg);
				if(verbose_flag) {
					printf("--close %d \n", N);
					fflush(stdout);
				}
				if(N > fa_index || N < 0 || file_array[N] < 0) {
					printf("ERROR: trying to close an fd that is not open!\n");
					exit(9); 	/* EBADF == 9 */
				}
				close(file_array[N]);
				file_array[N] = -1;				
				if(profile_flag) 
					print_usage(RUSAGE_SELF);				
				/**	
				 *	this function closes the fd corresponding to file_array[N]
				 *	and then sets file_array[N] to -1 flagging that this fd has
				 *	been previously allocated but is now closed and will remain so
				 **/				
				break;
			}
			case VERBOSE:
			{
				verbose_flag = 1;
				printf("--verbose\n");
				fflush(stdout);
				break;
			}
			case PROFILE:
			{
				if(verbose_flag) {
					printf("--profile \n");
					fflush(stdout);
				}
				profile_flag = 1;
				verbose_flag = 1;				
				/**
				 * setting verbose_flag = 1 so that when I print out profile info, it also prints
				 * out the corresponding info for which commands the time usage corresponds to
				 **/
				break;
			}
			case ABORT:
			{				
				abort_me();
				break;
			}
			case CATCH:
			{
				int N = atoi(optarg);
				if(verbose_flag) {
					printf("--catch %d \n", N);
					fflush(stdout);
				}			
				signal(N, signal_handler);
				if(profile_flag) 
					print_usage(RUSAGE_SELF);
				break;
			}
			case IGNORE:
			{
				int N = atoi(optarg);
				if(verbose_flag) {
					printf("--ignore %d \n", N);
					fflush(stdout);
				}				
				signal(N, SIG_IGN);	
				if(profile_flag) 
					print_usage(RUSAGE_SELF);
				break;
			}
			case DEFAULT:
			{
				int N = atoi(optarg);
				if(verbose_flag) {
					printf("--default %d \n", N);
					fflush(stdout);
				}
				signal(N, SIG_DFL);
				if(profile_flag) 
					print_usage(RUSAGE_SELF);
				break;
			}
			case PAUSE:
			{
				if(verbose_flag) {
					printf("--pause \n");
					fflush(stdout);
				}
				pause();
				if(profile_flag) 
					print_usage(RUSAGE_SELF);
				break;
			}			
		}		
		/**
		 *	Blocks below for realloc
		 **/		
		if ((size_of_fa - 1) == fa_index ) {
			size_of_fa *= 2;
			file_array = realloc(file_array, size_of_fa);
			if (file_array == NULL) {
				fprintf(stderr, "ERROR: realloc failed! \n");
				exit(2);
			}
		}
		if ((size_of_pa - 1) == pa_index) {
			size_of_pa *= 2;
			pid_array = realloc(pid_array, size_of_pa);
			if (pid_array == NULL) {
				fprintf(stderr, "ERROR: realloc failed! \n");
				exit(2);
			}
			cmd_list = realloc(cmd_list, (size_of_pa));
			if (cmd_list == NULL) {
				fprintf(stderr, "ERROR: realloc failed! \n");
				exit(2);
			}
			cmd_size_array = realloc(cmd_size_array, size_of_pa);
			if (cmd_size_array == NULL) {
				fprintf(stderr, "ERROR: realloc failed! \n");
				exit(2);
			}
		}
	}	
		/**
		 *	Blocks above for realloc
		 **/	
	if(wait_flag)
		wait_function();		
	max_exit = max_exit == 0 ? 0 : 1;			
	return max_exit;
} 