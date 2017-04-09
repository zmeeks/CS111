#define _GNU_SOURCE

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "SortedList.h"
#include <time.h>

void SortedList_insert(SortedList_t *list, SortedListElement_t *element){
 	if(opt_yield & INSERT_YIELD)
 	 	pthread_yield();
 /**
  *		NOTE: entire function is critical bc another thread's insertion can change the point of 
  *			  insertion and similarly for switching the pointers around for the insertion
  */
  	if(list->next->key != NULL){
		while(strcmp(list->next->key, element->key) < 0) {
			list = list->next;
			if(list->next->key == NULL)
				break;
		}
	}
	SortedListElement_t *next_node = list->next;
	list->next = element;
	element->prev = list;
	element->next = next_node;	
	next_node->prev = element;
}

int SortedList_delete( SortedListElement_t *element) {
	if(element == NULL)
		return 1; //implies element already deleted... "corruption" seems like best way to deal with this scenario	
	if(opt_yield & DELETE_YIELD)
 	 	pthread_yield();	
 /**
  *		NOTE: critical section starts here bc it's possible that another thread could start deletion
  *			  without having changed all of the pointers around yet, and the pointers are obvs critical.
  *			  the if(element == NULL) could arguably be critical too, but is redundant since the same
  *			  result occurs by placing the critical section here instead
  */		
	if(element->next->prev != element)
		return 1; //implies corrupted list
	if(element->prev->next != element)
		return 1; //implies corrupted list
	element->prev->next = element->next;
	element->next->prev = element->prev;
	element = NULL;
	return 0;
}

SortedListElement_t *SortedList_lookup(SortedList_t *list, const char *key){	
	if(list->next->key == NULL) 
		return NULL;		
	if(opt_yield & LOOKUP_YIELD)
 	 	pthread_yield();	
 /**
  *  NOTE:	critical section here corresponds to looking up and deletion, thus cs begins here
  */	
	while(strcmp(list->next->key, key) < 0) {
		list = list->next;
		if(list->next->key == NULL) 
			return NULL;
	}
	if(strcmp(key, list->next->key) != 0) 
		return NULL;	
	return list->next; 				
}

int SortedList_length(SortedList_t *list){	
	int c = 0; //count	
	if(opt_yield & LOOKUP_YIELD)
 	 	pthread_yield();	
 /**
  *  NOTE:	critical section here is arbitrary since multiple threads don't affect length look up.
  *			If length occurred alongside other ops such as insert or delete, then cs would begin here
  */		 	 	
	while(list->next->key != NULL) {
		if(list->next->prev != list || list->prev->next != list)
			return -1;
		list = list->next;
		c++;
	}
	if(list->next->prev != list || list->prev->next != list)
		return -1;
	return c;
}
