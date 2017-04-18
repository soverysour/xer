#include "headers/logic.h"
#include "headers/symbol.h"
#include "headers/mapgen.h"

#define DOWN '2'
#define LEFT '4'
#define	RIGHT '6'
#define UP '8'
#define LDOWN '1'
#define RDOWN '3'
#define LUP '7'
#define RUP '9'
#define CENTER '5'

#define IMPASSABLE 0
#define PASSABLE 1

char ef[] = { 1, 0, 0 };
struct object object_player = {
	.id = ID_PLAYER,
	.hp = 1,
	.x = 6,
	.y = 4,
	.effects = ef,
	.next = 0
};

void update_player(char input){
	int x = 0, y = 0;

	switch ( input ){
		case CENTER:
			break;
		case LEFT:
			x--;
			break;
		case RIGHT:
			x++;
			break;
		case UP:
			y--;
			break;
		case DOWN:
			y++;
			break;
		case LDOWN:
			x--;
			y++;
			break;
		case RDOWN:
			y++;
			x++;
			break;
		case LUP:
			y--;
			x--;
			break;
		case RUP:
			x++;
			y--;
			break;
		case 'B':
			object_player.effects[B_FORCE] = 1;
			break;
		case 'b':
			object_player.effects[B_FORCE] = 0;
			break;
	}

	if ( get_tile(object_player.y + y, object_player.x + x).id != ID_WALL ){
		object_player.x += x;
		object_player.y += y;
	}
}

struct object *get_player(void){
	return &object_player;
}
