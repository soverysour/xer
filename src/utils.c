#include <time.h>
#include <stdlib.h>
#include <string.h>

#include "headers/symbol.h"

int absolute(int x){
	if ( x < 0 )
		return -x;
	return x;
}

void init_seed(void){
	srandom(time(NULL));
}

int get_rand(int x){
	return rand() % x + 1;
}

char *get_char_array(int k){
	return calloc(k, sizeof(char));
}

void give_char_array(char *k){
	free(k);
}

struct object* get_object(void){
	return calloc(1, sizeof(struct object));
}

void give_object(struct object *k){
	free(k);
}

void silent_apply(struct object *a, struct symbol *b){
	switch ( a->id ){
		case ID_PLAYER:
			b->identity = calloc(2, sizeof(char));
			b->identity[0] = '@';
			b->identity[1] = '\0';
			break;
		case ID_MONSTER:
			b->identity = calloc(2, sizeof(char));
			b->identity[0] = 'M';
			b->identity[1] = '\0';
			break;
		case ID_WALL:
			b->identity = calloc(2, sizeof(char));
			b->identity[0] = '#';
			b->identity[1] = '\0';
			break;
		case ID_FLOOR:
			b->identity = calloc(2, sizeof(char));
			b->identity[0] = '.';
			b->identity[1] = '\0';
			break;
	}

	b->attribs = calloc(NR_ATTRIBS, sizeof(char));
	if ( a->effects[B_BUFFED] )
		b->attribs[AT_BOLD] = 1;
	if ( a->effects[B_POISONED] )
		b->attribs[AT_BLINK] = 1;

	b->y = a->y;
	b->x = a->x;
}

struct symbol *do_apply(struct object *k){
	struct object *a = k;
	struct symbol *b = calloc(1, sizeof(struct symbol)), *prev;
	struct symbol *head = b;

	silent_apply(a, b);
	
	while ( a->next ) {
		a = a->next;

		prev = b;
		b->next = calloc(1, sizeof(struct symbol));
		b = b->next;
		prev->next = b;

		silent_apply(a, b);
	}

	return head;
}

void clean_symbol(struct symbol *a){
	struct symbol *forward = a->next, *now = a;

	while ( now->next ){
		free(now);
		now = forward;
		forward = forward->next;
	}
	free(now);
}
