#include "headers/symbol.h"
#include "headers/gui.h"
#include "headers/utils.h"
#include "headers/logic.h"

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

void update_engine(void){
	char input = get_input();
	
	struct object *a = logic_update(input);

	if ( a->y < 0 )
		end_engine("See you soon...\n");
	
	else {
		struct symbol *b = do_apply(a);
		update_gui(b);
		clean_symbol(b);
	}
	clean_object(a);
}

int main(void){
	init_engine();

	while( running )
		update_engine();
	
  return 0;
}
