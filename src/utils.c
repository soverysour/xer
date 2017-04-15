#include <time.h>
#include <stdlib.h>
#include <string.h>

#include "headers/symbol.h"

void init_seed(void){
	srandom(time(NULL));
}

int get_random(int x){
	return rand() % x + 1;
}

char *get_char_array(int k){
	return calloc(k, sizeof(char));
}

void give_char_array(char *k){
	free(k);
}

void silent_apply(struct object *a, struct symbol *b){
	switch ( a->id ){
		case player:
			b->identity = calloc(2, sizeof(char));
			b->identity[0] = '@';
			b->identity[1] = '\0';
			break;
		case monster:
			b->identity = calloc(2, sizeof(char));
			b->identity[0] = 'M';
			b->identity[1] = '\0';
			break;
	}

	b->attribs = calloc(NR_ATTRIBS, sizeof(char));
	if ( a->effects[buffed] )
		b->attribs[bold] = 1;
	if ( a->effects[poisoned] )
		b->attribs[blink] = 1;

	b->y = a->y;
	b->x = a->x;
}

struct symbol* do_apply(struct object *k){
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

void clean_object(struct object *a){
	struct object *k = a->next, *reserve = a;
	//free(reserve);

	while ( k->next ) {
		reserve = k;
		k = k->next;
		free(reserve);
	}
}

void clean_symbol(struct symbol *a){
	struct symbol *k = a->next, *reserve = a;
	//free(reserve);

	while ( k->next ) {
		reserve = k;
		k = k->next;
		free(reserve);
	}
}
