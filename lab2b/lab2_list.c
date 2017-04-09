#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <getopt.h> 
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <math.h>
#include <string.h>
#include <sys/stat.h> 
#include <fcntl.h>
#include "SortedList.h"

#define ITERATIONS 'a'
#define LISTS 'b'
#define SYNC 'c'
#define THREADS 'd'
#define YIELD 'e'

int threads = 1;		//default
int iterations = 1;		//default
int lists = 1;			//default
int opt_yield = 0;		//default
char sync_flag = 'd';	//default
SortedList_t *head_list = NULL;
SortedListElement_t *element_array = NULL;

pthread_mutex_t *mutex_array = NULL;  
int volatile *spin_array = NULL;		

unsigned long hash(const char *str) {
	/* djb2 hash algo */
    unsigned long hash = 5381;
    int c;
    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash;
}

char * sync_d_rename() {
	char * return_ptr = malloc(sizeof(char)*16);
	switch(opt_yield) {
		case 0:
			return_ptr="list-none-none";
			break;
		case 1:
			return_ptr="list-i-none";
			break;
		case 2:
			return_ptr="list-d-none";
			break;
		case 3:
			return_ptr="list-id-none";
			break;
		case 4:
			return_ptr="list-l-none";
			break;
		case 5:
			return_ptr="list-il-none";
			break;
		case 6:
			return_ptr="list-dl-none";
			break;
		case 7:
			return_ptr="list-idl-none";
			break;	
	}	
	return return_ptr;
}

char * sync_m_rename() {
	char * return_ptr = malloc(sizeof(char)*16);
	switch(opt_yield) {
		case 0:
			return_ptr="list-none-m";
			break;
		case 1:
			return_ptr="list-i-m";
			break;
		case 2:
			return_ptr="list-d-m";
			break;
		case 3:
			return_ptr="list-id-m";
			break;
		case 4:
			return_ptr="list-l-m";
			break;
		case 5:
			return_ptr="list-il-m";
			break;
		case 6:
			return_ptr="list-dl-m";
			break;
		case 7:
			return_ptr="list-idl-m";
			break;	
	}	
	return return_ptr;
}

char * sync_s_rename() {
	char * return_ptr = malloc(sizeof(char)*16);
	switch(opt_yield) {
		case 0:
			return_ptr="list-none-s";
			break;
		case 1:
			return_ptr="list-i-s";
			break;
		case 2:
			return_ptr="list-d-s";
			break;
		case 3:
			return_ptr="list-id-s";
			break;
		case 4:
			return_ptr="list-l-s";
			break;
		case 5:
			return_ptr="list-il-s";
			break;
		case 6:
			return_ptr="list-dl-s";
			break;
		case 7:
			return_ptr="list-idl-s";
			break;	
	}	
	return return_ptr;
}

void element_array_key_init(SortedListElement_t *array, int size){	
	int i, j, fd, read_it;
	char **random_string =  (char**)malloc(sizeof(char*) * size);	
	fd = open("/dev/urandom", O_RDONLY);
	for(i=0; i<size; i++){
		random_string[i] =  (char*)malloc(sizeof(char) * 26);
		//fd = open("/dev/urandom", O_RDONLY);
		read_it = read(fd, random_string[i], 25);
		if(read_it == -1) {
				fprintf(stderr, "ERROR: unsuccessful read from dev/urandom! size= %d\n", size);
				perror("exiting with errno: \n");
				exit(1);
		}				
		for(j=0; j<25; j++)
			random_string[i][j] = abs(random_string[i][j])%94 + 33;			
		array[i].key = (const char *)random_string[i];
	}
}

void list_init(SortedList_t *list) {
	list->next = list;
	list->prev = list;
	list->key = NULL;
}

void *wrapper_d(void *elem){
	int i;
	int length;
	int del;
	char *key_val;
	SortedListElement_t *list_elem;	
	int list_num;
	//insert
	SortedListElement_t *elem_ptr = &element_array[*((int*)elem)];	
	list_num = (int)(hash(elem_ptr->key)%((long long int)lists));
	
	for(i=0; i< iterations-1; i++){	
		SortedList_insert(&head_list[list_num], elem_ptr);	
		elem_ptr += threads;	
		list_num = (int)(hash(elem_ptr->key)%((long long int)lists));
	}
	SortedList_insert(&head_list[list_num], elem_ptr);	
	//length lookup
	length = SortedList_length(&head_list[list_num]);
	if(length == -1){
		fprintf(stderr, "ERROR in length: corrupted list!\n");
		exit(1);
	}
	//lookup and delete
	for(i=0; i< iterations-1; i++){	
		list_elem = SortedList_lookup(&head_list[list_num], elem_ptr->key);
		if(list_elem == NULL){
			fprintf(stderr, "ERROR in lookup: corrupted list!\n");
			exit(1);
		}
		del = SortedList_delete(list_elem);
		elem_ptr -= threads;
		list_num = (int)(hash(elem_ptr->key)%((long long int)lists));
		if(del == 1){
			fprintf(stderr, "ERROR in delete: corrupted list!\n");
			exit(1);
		}
	}
	list_elem = SortedList_lookup(&head_list[list_num], elem_ptr->key);
	if(list_elem == NULL){
			fprintf(stderr, "ERROR in lookup: corrupted list!\n");
			exit(1);
		}
	del = SortedList_delete(list_elem);
	if(del == 1){
			fprintf(stderr, "ERROR in delete: corrupted list!\n");
			exit(1);
		}	
	pthread_exit(NULL);
}

void *wrapper_m(void *elem){
	int i;
	int length;
	int del;
	char *key_val;
	SortedListElement_t *list_elem;	
	int list_num;
	//insert
	SortedListElement_t *elem_ptr = &element_array[*((int*)elem)];	
	list_num = (int)(hash(elem_ptr->key)%((long long int)lists));
	
	for(i=0; i< iterations-1; i++){
		pthread_mutex_lock (&mutex_array[list_num]);		
		SortedList_insert(&head_list[list_num], elem_ptr);	
		elem_ptr += threads;
		pthread_mutex_unlock (&mutex_array[list_num]);	
		list_num = (int)(hash(elem_ptr->key)%((long long int)lists));
		}
	pthread_mutex_lock (&mutex_array[list_num]);	
	SortedList_insert(&head_list[list_num], elem_ptr);
	pthread_mutex_unlock (&mutex_array[list_num]);	
	//length lookup
	pthread_mutex_lock (&mutex_array[list_num]);
	length = SortedList_length(&head_list[list_num]);
	pthread_mutex_unlock (&mutex_array[list_num]);
	if(length == -1){
		fprintf(stderr, "ERROR in length: corrupted list!\n");
		exit(1);
	}	
	//lookup and delete
	for(i=0; i< iterations-1; i++){
		pthread_mutex_lock (&mutex_array[list_num]);	
		list_elem = SortedList_lookup(&head_list[list_num], elem_ptr->key);
		if(list_elem == NULL){
			fprintf(stderr, "ERROR in lookup: corrupted list!\n");
			exit(1);
		}
		del = SortedList_delete(list_elem);
		elem_ptr -= threads;
		if(del == 1){
			fprintf(stderr, "ERROR in delete: corrupted list!\n");
			exit(1);
		}
		pthread_mutex_unlock (&mutex_array[list_num]);
		list_num = (int)(hash(elem_ptr->key)%((long long int)lists));
	}
	pthread_mutex_lock (&mutex_array[list_num]);
	list_elem = SortedList_lookup(&head_list[list_num], elem_ptr->key);
	if(list_elem == NULL){
			fprintf(stderr, "ERROR in lookup: corrupted list!\n");
			exit(1);
		}
	del = SortedList_delete(list_elem);
	pthread_mutex_unlock (&mutex_array[list_num]);
	if(del == 1){
			fprintf(stderr, "ERROR in delete: corrupted list!\n");
			exit(1);
		}	
	pthread_exit(NULL);
} 

void *wrapper_s(void *elem){
	int i;
	int length;
	int del;
	char *key_val;
	SortedListElement_t *list_elem;	
	int list_num;
	//insert
	SortedListElement_t *elem_ptr = &element_array[*((int*)elem)];	
	list_num = (int)(hash(elem_ptr->key)%((long long int)lists));
	
	for(i=0; i< iterations-1; i++){		
		while(__sync_lock_test_and_set(&spin_array[list_num], 1))
        	continue; 
		SortedList_insert(&head_list[list_num], elem_ptr);	
		elem_ptr += threads;	
		__sync_lock_release(&spin_array[list_num]);
		list_num = (int)(hash(elem_ptr->key)%((long long int)lists));
	}
	while(__sync_lock_test_and_set(&spin_array[list_num], 1))
        	continue; 
	SortedList_insert(&head_list[list_num], elem_ptr);
	__sync_lock_release(&spin_array[list_num]);	
	//length lookup
	while(__sync_lock_test_and_set(&spin_array[list_num], 1))
        	continue; 
	length = SortedList_length(&head_list[list_num]);
	__sync_lock_release(&spin_array[list_num]);
	if(length == -1){
		fprintf(stderr, "ERROR in length: corrupted list!\n");
		exit(1);
	}	
	//lookup and delete
	for(i=0; i< iterations-1; i++){	
		while(__sync_lock_test_and_set(&spin_array[list_num], 1))
        	continue; 
		list_elem = SortedList_lookup(&head_list[list_num], elem_ptr->key);
		if(list_elem == NULL){
			fprintf(stderr, "ERROR in lookup: corrupted list!\n");
			exit(1);
		}
		del = SortedList_delete(list_elem);
		elem_ptr -= threads;
		if(del == 1){
			fprintf(stderr, "ERROR in delete: corrupted list!\n");
			exit(1);
		}
		__sync_lock_release(&spin_array[list_num]);
		list_num = (int)(hash(elem_ptr->key)%((long long int)lists));
	}
	while(__sync_lock_test_and_set(&spin_array[list_num], 1))
        	continue; 
	list_elem = SortedList_lookup(&head_list[list_num], elem_ptr->key);
	if(list_elem == NULL){
			fprintf(stderr, "ERROR in lookup: corrupted list!\n");
			exit(1);
		}
	del = SortedList_delete(list_elem);
	if(del == 1){
			fprintf(stderr, "ERROR in delete: corrupted list!\n");
			exit(1);
		}	
	__sync_lock_release(&spin_array[list_num]);
	pthread_exit(NULL);	
}
	
long long int nano_time(struct timespec time_as_struc) {
	long long int nano_seconds = (long long int)(time_as_struc.tv_sec * 1000000000) + (long long int)time_as_struc.tv_nsec;
	return nano_seconds;
}

struct timespec ts_subtract(struct timespec finish, struct timespec start) {
	struct timespec update;
	long int nsec_compare = finish.tv_nsec - start.tv_nsec;	
	if(nsec_compare < 0) {
		nsec_compare += 1000000000;
		update.tv_sec = finish.tv_sec - start.tv_sec - 1;
		update.tv_nsec = nsec_compare;
	} else {
		update.tv_sec = finish.tv_sec - start.tv_sec;
		update.tv_nsec = finish.tv_nsec - start.tv_nsec;
	}		
	return update;
}		

int main(int argc, char** argv) {
	int ret = 0;
	int i;
	char *test_name = " ";
	int ops;
	
	//long options
	struct option long_options[] = {
		{"iterations", optional_argument, NULL, ITERATIONS},
		{"lists", optional_argument, NULL, LISTS},
		{"sync", optional_argument, NULL, SYNC},
		{"threads", optional_argument, NULL, THREADS},
		{"yield", optional_argument, NULL, YIELD},
		{0, 0, 0, 0}
	};	
	while (1) {
		ret = getopt_long(argc, argv, "", long_options, NULL);	
		if (ret == -1) {
			break;
		}
		switch (ret) {
			case ITERATIONS:
				if(optarg)
					iterations = atoi(optarg);	
				break;
			case LISTS:
				if(optarg)
					lists = atoi(optarg);
				break;
			case SYNC:
				if(optarg)
					sync_flag = *optarg;
				break;
			case THREADS:
				if(optarg)
					threads = atoi(optarg);
				break;
			case YIELD:
				if(strcmp(optarg,"i") == 0)
					opt_yield = 1;
				else if(strcmp(optarg,"d") == 0)
					opt_yield = 2;
				else if(strcmp(optarg,"l") == 0)
					opt_yield = 4;
				else if(strcmp(optarg,"id") == 0 || strcmp(optarg,"di") == 0)
					opt_yield = 3;
				else if(strcmp(optarg,"il") == 0 || strcmp(optarg, "li") == 0)
					opt_yield = 5;
				else if(strcmp(optarg,"dl") == 0 || strcmp(optarg, "ld") == 0)
					opt_yield = 6;
				else if(strcmp(optarg,"idl") == 0 || strcmp(optarg,"ild") == 0 || strcmp(optarg,"dil") == 0) 
					opt_yield = 7;
				else if(strcmp(optarg,"dli") == 0 || strcmp(optarg,"lid") == 0 || strcmp(optarg,"ldi") == 0)
					opt_yield = 7;								
				break;
		}
	}
	mutex_array = malloc(sizeof(pthread_mutex_t) * lists);
	spin_array = malloc(sizeof(int) * lists);
	head_list = malloc(sizeof(SortedList_t)*lists);			
	for(i=0; i<lists; i++){
		list_init(&head_list[i]);
		pthread_mutex_init(&mutex_array[i], NULL);
		spin_array[i] = 0;
	}
	element_array = malloc(sizeof(SortedListElement_t) * threads * iterations);
	element_array_key_init(element_array, threads*iterations);
		//^adding the random elements into the array of structs
	pthread_t *thread_array = malloc(threads * sizeof(pthread_t));	
	struct timespec time_start;
	struct timespec time_finish;
	int z;	
	int *num=malloc(sizeof(int)*threads);
	for(i=0;i<threads;i++)
		num[i]=i;
	switch(sync_flag) {
		case 'd':
			test_name = sync_d_rename();
			clock_gettime(CLOCK_MONOTONIC, &time_start); 
			for(i=0;i<threads;i++){	
				z = pthread_create(&thread_array[i], NULL, wrapper_d, &num[i]);	
				if(z != 0){
					fprintf(stderr, "ERROR: pthread create failed! \n");
					exit(1);
				}	
			}			
			break;
		case 'm':
			test_name = sync_m_rename();		
			clock_gettime(CLOCK_MONOTONIC, &time_start); 
			for(i=0;i<threads;i++){	
				z = pthread_create(&thread_array[i], NULL, wrapper_m, &num[i]);	
				if(z != 0){
					fprintf(stderr, "ERROR: pthread create failed! \n");
					exit(1);
				}	
			}			
			break;
		case 's':
			test_name = sync_s_rename();
			clock_gettime(CLOCK_MONOTONIC, &time_start); 
			for(i=0;i<threads;i++){	
				z = pthread_create(&thread_array[i], NULL, wrapper_s, &num[i]);	
				if(z != 0){
					fprintf(stderr, "ERROR: pthread create failed! \n");
					exit(1);
				}	
			}			
			break;			
	}
	
	for(i = 0; i < threads; i++){
    	pthread_join(thread_array[i], NULL);
    }
    
    clock_gettime(CLOCK_MONOTONIC, &time_finish);
    int total_len = 0;
    for(i=0; i<lists; i++)
    	total_len += SortedList_length(&head_list[i]);    
    if(total_len != 0){
    	fprintf(stderr, "ERROR in final length: length not zero --> corrupted list!\n");
		exit(1);
    }
    
    ops = threads*iterations*3;
    
    struct timespec time_total = ts_subtract(time_finish, time_start);
	
	long long int total_time = nano_time(time_total);
	
	long long int avg_op_time = total_time/((long long int)ops);
	
	printf("%s,%d,%d,%d,%d,%lld,%lld\n", test_name, threads, iterations, lists, ops, total_time, avg_op_time);
		
	return 0;
}

