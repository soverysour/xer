#include "headers/symbol.h"
#include "headers/standard_objects.h"
#include "headers/main.h"
#include "headers/utils.h"

#include <stdio.h>

#define NR_ROOMS 17

struct room
{
  int x, y;
  int w, h;
} rooms[NR_ROOMS];

char level = 0;
struct object map[M_ROWS][M_COLS];

int cluster_count = 0;
int clusters[NR_ROOMS][NR_ROOMS];

int check_intersect(struct room x, struct room y){
  if ( x.x + x.w < y.w ||
       y.x + y.w < x.x ||
       x.y + x.h < y.y ||
       y.y + y.h < x.y
     )
    return 0;

  return 1;
}

int check_rooms( struct room x, struct room y )
{
  if ( x.x < 2 || ( x.x + x.w > M_COLS - 3 ) )
    return 1;

  if ( y.x < 2 || ( y.x + y.w > M_COLS - 3 ) )
    return 1;

  if ( x.y < 2 || ( x.y + x.h > M_ROWS - 3 ) )
    return 1;

  if ( y.y < 2 || ( y.y + y.h > M_ROWS - 3 ) )
    return 1;

  if ( y.x <= x.x && y.x + y.w >= x.x + x.w &&
       y.y <= x.y && y.y + y.h >= x.y + x.h )
    return 1;

  if ( x.x <= y.x && x.x + x.w >= y.x + y.w &&
       x.y <= y.y && x.y + x.h >= y.y + y.h )
    return 1;

  return 0;
}

void load_map( void )
{
  for ( int i = 0; i < NR_ROOMS; i++ )
    for ( int j = 0; j < NR_ROOMS; j++ )
      clusters[i][j] = -1;

  level++;

  if ( !level )
  {
    end_engine( "Congratulations.\n" );
    return;
  }

  for ( int i = 0; i < M_ROWS; i++ )
    for ( int j = 0; j < M_COLS; j++ )
    {
      map[i][j] = Owall;
      map[i][j].x = j;
      map[i][j].y = i;

      if ( i == M_ROWS - 1 && j == M_COLS - 1 )
        map[i][j].next = 0;
      else if ( j == M_COLS - 1 )
        map[i][j].next = map[i + 1];
      else
        map[i][j].next = map[i] + j + 1;
    }

generation:

  for ( int i = 0; i < NR_ROOMS; i++ )
  {
    rooms[i].w = get_rand( 8 ) + 2;
    rooms[i].h = get_rand( 5 ) + 4;
    rooms[i].x = get_rand( M_COLS ) - 1;
    rooms[i].y = get_rand( M_ROWS ) - 1;
  }

  for ( int i = 0; i < NR_ROOMS - 1; i++ )
    for ( int j = i + 1; j < NR_ROOMS; j++ )
      if ( check_rooms( rooms[i], rooms[j] ) )
        goto generation;

  for ( int i = 0; i < NR_ROOMS; i++ ){
    for ( int j = rooms[i].y; j < rooms[i].y + rooms[i].h; j++ )
      for ( int k = rooms[i].x; k < rooms[i].x + rooms[i].w; k++ )
        map[j][k].id = Ofloor.id;

    int found = -1;
    for ( int j = 0; j < cluster_count; j++ ){  
      int k = 0;

      while ( clusters[j][k] != -1 ){
        if ( check_intersect(rooms[i], rooms[clusters[j][k]]) ){
          found = j;
          break;
        }
        k++;
      }
      if ( found != -1 )
        break;
    }
      
    if ( found != -1 ){
      int k = 0;
      while ( clusters[found][k] != -1 )
        k++;
      clusters[found][k] = i;
    }
    else {
      clusters[cluster_count][0] = i;
      cluster_count++;
    }
  }

  FILE *file = fopen("/home/sour/logaritmus.txt", "w");

  for ( int i = 0; i < NR_ROOMS; i++ ){
    for ( int j = 0; j < NR_ROOMS; j++ )
      fprintf(file, "%i ", clusters[i][j]);
    fprintf(file, "\n");
   }

  fclose(file);

}

struct object *get_map( void )
{
  return map[0];
}

struct object get_tile( int x, int y )
{
  return map[x][y];
}
