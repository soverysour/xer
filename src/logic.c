#include "headers/symbol.h"
#include "headers/player.h"
#include "headers/standard_objects.h"
#include "headers/mapgen.h"

char ef[] = { 1, 1 };
struct object Oplayer = {
	.id = ID_PLAYER,
	.hp = 1,
	.x = 1,
	.y = 1,
	.effects = ef,
	.next = 0
};

struct object *Omap;
char loaded = 0;

void update_Oplayer(int x, int y){
	Oplayer.x = x;
	Oplayer.y = y;
}

struct object *logic_update(char input){
	if ( input == 'Q' )
		return &Oquit;

	if ( !loaded ){
		load_map();
		Omap = get_map();
		
		struct object *k = Omap;

		while ( k->next )
			k = k->next;

		k->next = &Oplayer;
		loaded = 1;
	}
	
	update_player(input);

	return Omap;
}
