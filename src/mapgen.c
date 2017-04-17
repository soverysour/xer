#include "headers/symbol.h"
#include "headers/standard_objects.h"
#include "headers/main.h"
#include "headers/utils.h"

#define COLS 80
#define ROWS 28
#define NR_ROOMS 5

struct room {
	int x, y;
	int w, h;
} rooms[NR_ROOMS];

char level = 0;

struct object map[COLS][ROWS];

int check_rooms(struct room x, struct room y){
	

	return 0;
}

void load_map(void){
	level++;

	if ( !level ){
		end_engine("Congratulations.\n");
		return;
	}

	for ( int i = 0; i < COLS; i++ )
		for ( int j = 0; j < ROWS; j++ ){
			map[i][j] = Owall;
			map[i][j].x = i;
			map[i][j].y = j;

			if ( i == COLS - 1 && j == ROWS - 1 )
				map[i][j].next = 0;
			else if ( j == ROWS - 1 )
				map[i][j].next = map[i+1];
			else
				map[i][j].next = map[i] + j+1;
		}

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
	
	for ( int i = 0; i < NR_ROOMS; i++ )
		for ( int j = rooms[i].x; j <= rooms[i].x + rooms[i].w; j++ )
			for ( int k = rooms[i].y; k <= rooms[i].y + rooms[i].h; k++ )
				if ( j * k && j < COLS - 1 && k < ROWS - 1 )
					map[j][k].id = Ofloor.id;
}

struct object *get_map(void){
	return map[0];
}
