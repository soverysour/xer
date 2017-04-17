#include "headers/symbol.h"
#include "headers/standard_objects.h"
#include "headers/main.h"
#include "headers/utils.h"

#define NR_ROOMS 17

struct room {
	int x, y;
	int w, h;
} rooms[NR_ROOMS];

char level = 0;
struct object map[M_ROWS][M_COLS];

int check_rooms(struct room x, struct room y){
	if ( x.x < 2 || ( x.x + x.w > M_COLS - 3 ) )
		return 1;

	if ( y.x < 2 || ( y.x + y.w > M_COLS - 3 ) )
		return 1;

	if ( x.y < 2 || ( x.y + x.h > M_ROWS - 3 ) )
		return 1;

	if ( y.y < 2 || ( y.y + y.h > M_ROWS - 3 ) )
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

	for ( int i = 0; i < M_ROWS; i++ )
		for ( int j = 0; j < M_COLS; j++ ){
			map[i][j] = Owall;
			map[i][j].x = j;
			map[i][j].y = i;

			if ( i == M_ROWS - 1 && j == M_COLS - 1 )
				map[i][j].next = 0;
			else if ( j == M_COLS - 1 )
				map[i][j].next = map[i+1];
			else map[i][j].next = map[i] + j + 1;
		}

	generation:
	for ( int i = 0; i < NR_ROOMS; i++ ){
		rooms[i].w = get_rand(8) + 3;
		rooms[i].h = get_rand(4) + 5;
		rooms[i].x = get_rand(M_COLS) - 4;
		rooms[i].y = get_rand(M_ROWS) - 4;
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

struct object get_tile(int x, int y){
	return map[x][y];
}
