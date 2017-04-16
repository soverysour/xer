#include "headers/symbol.h"
#include "headers/standard_objects.h"
#include "headers/main.h"
#include "headers/utils.h"

#define COLS 60
#define ROWS 28
#define NR_ROOMS 5

struct room {
	int x, y;
	int w, h;
} rooms[NR_ROOMS];

char level = 0;

struct object *map;

int check_rooms(struct room x, struct room y){
	int low_x = x.x > y.x ? y.x : x.x;
	int low_y = x.y > y.y ? y.y : x.y;

	return 0;
}

void load_map(void){
	level++;

	if ( !level ){
		end_engine("Congratulations.\n");
		return;
	}

	map = get_object();
	struct object *current = map;

	for ( int i = 0; i < COLS; i++ )
		for ( int j = 0; j < ROWS; j++ ){
			*current = Owall;
			current->x = i;
			current->y = j;
			current->next = get_object();
			current = current->next;
		}

	*current = Owall;
	current->y = ROWS - 1;
	current->x = COLS - 1;
	current->next = 0;

	generation:
	for ( int i = 0; i < NR_ROOMS; i++ ){
		rooms[i].h = get_rand(4) + 5;
		rooms[i].w = get_rand(3) + 3;
		rooms[i].x = get_rand(COLS) - 1;
		rooms[i].y = get_rand(ROWS) - 1;
	}

	for ( int i = 0; i < NR_ROOMS - 1; i++ )
		for ( int j = 0; j < NR_ROOMS; j++ )
			if ( check_rooms(rooms[i], rooms[j]) )
				goto generation;
}

struct object *get_map(void){
	return map;
}
