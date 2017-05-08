#include "headers/symbol.h"
#include "headers/standard_objects.h"
#include "headers/main.h"
#include "headers/utils.h"

#define NR_ROOMS 17

struct room
{
  int x, y;
  int w, h;
} rooms[NR_ROOMS];

char level = 0;
struct object map[M_ROWS][M_COLS];

int check_intersect( int i, int j )
{
  if ( rooms[i].x + rooms[i].w < rooms[j].w ||
       rooms[j].x + rooms[j].w < rooms[i].x ||
       rooms[i].y + rooms[i].h < rooms[j].y ||
       rooms[j].y + rooms[j].h < rooms[i].y
     )
    return 0;

  return 1;
}

int check_room( int i )
{
  if ( rooms[i].x < 1 || ( rooms[i].x + rooms[i].w > M_COLS - 2 ) )
    return 1;

  if ( rooms[i].y < 1 || ( rooms[i].y + rooms[i].h > M_ROWS - 2 ) )
    return 1;

  return 0;
}

void random_room( int i )
{
  rooms[i].x = get_rand( M_COLS ) - 1;
  rooms[i].y = get_rand( M_ROWS ) - 1;

  do
  {
    rooms[i].w = get_rand ( 7 );
  }
  while ( rooms[i].w < 2 );

  do
  {
    rooms[i].h = get_rand ( 7 );
  }
  while ( rooms[i].h < 2 );
}

void generate_map( void )
{
  for ( int i = 0; i < M_ROWS; i++ )
    for ( int j = 0; j < M_COLS; j++ )
    {
      map[i][j] = Owall;
      map[i][j].x = j;
      map[i][j].y = i;
      map[i][j].visibility = V_UNSEEN;

      if ( i == M_ROWS - 1 && j == M_COLS - 1 )
        map[i][j].next = 0;
      else if ( j == M_COLS - 1 )
        map[i][j].next = map[i + 1];
      else
        map[i][j].next = map[i] + j + 1;
    }
}

void generate_rooms( void )
{
  for ( int i = 0; i < NR_ROOMS; i++ )
  {
    int k = 0;
  checks:
    random_room( i );

    if ( k == 50 )
    {
      k = 0;
      i = 0;
      goto checks;
    }

    while ( check_room( i ) )
      random_room( i );

    for ( int j = 0; j < i; j++ )
      if ( check_intersect( i, j ) )
      {
        k++;
        goto checks;
      }
  }
}
void generate_rooms_in_map( void )
{
  for ( int i = 0; i < NR_ROOMS; i++ )
    for ( int j = rooms[i].y; j < rooms[i].y + rooms[i].h; j++ )
      for ( int k = rooms[i].x; k < rooms[i].x + rooms[i].w; k++ )
        map[j][k].id = Ofloor.id;
}

void path_rooms( int i, int j )
{
  int x1 = rooms[i].x + get_rand( rooms[i].w ) - 1;
  int x2 = rooms[j].x + get_rand( rooms[j].w ) - 1;
  int y1 = rooms[i].y + get_rand( rooms[i].h ) - 1;
  int y2 = rooms[j].y + get_rand( rooms[j].h ) - 1;
  int diffx = x2 - x1;

  if ( x1 < x2 )
  {
    for ( int kk = x1; kk <= x2; kk++ )
      map[y1][kk].id = Ofloor.id;

    if ( y1 < y2 )
      for ( int kk = y1; kk <= y2; kk++ )
        map[kk][x2].id = Ofloor.id;
    else
      for ( int kk = y2; kk <= y1; kk++ )
        map[kk][x2].id = Ofloor.id;
  }
  else
  {
    for ( int kk = x2; kk <= x1; kk++ )
      map[y2][kk].id = Ofloor.id;

    if ( y1 < y2 )
      for ( int kk = y1; kk <= y2; kk++ )
        map[kk][x1].id = Ofloor.id;
    else
      for ( int kk = y2; kk <= y1; kk++ )
        map[kk][x1].id = Ofloor.id;
  }
}

void generate_paths( void )
{
  for ( int i = 0; i < NR_ROOMS - 1; i++ )
    path_rooms( i, i + 1 );
}

int next_level( void )
{
  level++;

  if ( !level )
  {
    end_engine( "Congratulations.\n" );
    return 1;
  }

  generate_map();
  generate_rooms();
  generate_rooms_in_map();
  generate_paths();
  return 0;
}

struct object *get_tile( int y, int x )
{
  return map[y] + x;
}
