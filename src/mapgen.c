#include "headers/symbol.h"
#include "headers/standard_objects.h"
#include "headers/main.h"
#include "headers/utils.h"

#define COLS 80
#define ROWS 28
#define NR_ROOMS 17

struct room {
	int x, y;
	int w, h;
} rooms[NR_ROOMS];

char level = 0;

struct object map[ROWS][COLS];

int check_rooms(struct room x, struct room y){
	if ( x.x < 2 || ( x.x + x.w > COLS - 3 ) )
		return 1;

	if ( y.x < 2 || ( y.x + y.w > COLS - 3 ) )
		return 1;

	if ( x.y < 2 || ( x.y + x.h > ROWS - 3 ) )
		return 1;

	if ( y.y < 2 || ( y.y + y.h > ROWS - 3 ) )
		return 1;

	if ( x.x == y.x && x.y == y.y )
		return 1;

	return 0; 
}

void load_map(void){
	level++;

	if ( !level ){
		end_engine("Congratulations.\n");
		return;
	}

	for ( int i = 0; i < ROWS; i++ )
		for ( int j = 0; j < COLS; j++ ){
			map[i][j] = Owall;
			map[i][j].x = j;
			map[i][j].y = i;

			if ( i == ROWS - 1 && j == COLS - 1 )
				map[i][j].next = 0;
			else if ( j == COLS - 1 )
				map[i][j].next = map[i+1];
			else map[i][j].next = map[i] + j + 1;
		}

	generation:
	for ( int i = 0; i < NR_ROOMS; i++ ){
		rooms[i].w = get_rand(8) + 3;
		rooms[i].h = get_rand(4) + 5;
		rooms[i].x = get_rand(COLS) - 4;
		rooms[i].y = get_rand(ROWS) - 4;
	}

	for ( int i = 0; i < NR_ROOMS - 1; i++ )
		for ( int j = i + 1; j < NR_ROOMS; j++ )
			if ( check_rooms(rooms[i], rooms[j]) )
				goto generation;

	for ( int i = 0; i < NR_ROOMS; i++ )
		for ( int j = rooms[i].y; j < rooms[i].y + rooms[i].h; j++ )
			for ( int k = rooms[i].x; k < rooms[i].x + rooms[i].w; k++ )
				map[j][k].id = Ofloor.id;
}

struct object *get_map(void){
	return map[0];
}
