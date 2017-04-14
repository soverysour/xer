#include <stdio.h>

#include "headers/symbol.h"
#include "headers/gui.h"
#include "headers/utils.h"

char running = 1;

void init_engine(void){
  running = init_gui();

  if ( !running )
    return;

  init_seed();
}

void end_engine(char *text){
	if ( running ){
		end_gui(text);
		running = 0;
	}
}

char ida[] = "Ceva";
char idb[] = "altceva";
char attra[] = { 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0 };
char attrb[] = { 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0 };

struct symbol k = {
	.next = 0
}, a = {
	.identity = ida,
	.attribs = attra,
	.fg = red,
	.bg = green,
	.y = 4,
	.x = 5,
	.next = &k
}, b = {
	.identity = idb,
	.attribs = attrb,
	.fg = green,
	.bg = blue,
	.y = 5,
	.x = 5,
	.next = &a
};

void update_engine(void){
	char input = get_input();
	
	if ( input == 'Q' )
		end_engine("See you around...\n");
	else
		update_gui(&b);
}

int main(void){
	init_engine();

	while( running )
		update_engine();
	
  return 0;
}
