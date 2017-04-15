#include "headers/symbol.h"

char ef[] = { 1, 0 };
char fe[] = { 0, 1 };

struct object p = {
	.x = 15,
	.y = 6,
	.id = player,
	.hp = 5,
	.effects = ef,
	.next = 0
};

struct object m = {
	.x = 23,
	.y = 9,
	.id = monster,
	.hp = 7,
	.effects = fe,
	.next = &p
};

struct object* logic_update(char input){
	if ( input == 'Q' )
		m.y = -1;
	else 
		m.y = 6;

	return &m;
}
