#include <time.h>
#include <stdlib.h>

#include "headers/symbol.h"

void init_seed(void){
	srandom(time(NULL));
}

int get_random(int x){
	return rand() % x + 1;
}
