#include "symbol.h"
#include "standard_objects.h"
#include "main.h"
#include "utils.h"
#include "player.h"

#define NR_ROOMS 8
#define LAST_LEVEL 3

struct room rooms[NR_ROOMS];

char level = 0;
struct object map[M_ROWS][M_COLS];

int check_intersect( int i, int j )
{
  if ( rooms[i].x + rooms[i].w + 1 < rooms[j].x ||
       rooms[j].x + rooms[j].w + 1 < rooms[i].x ||
       rooms[i].y + rooms[i].h + 1 < rooms[j].y ||
       rooms[j].y + rooms[j].h + 1 < rooms[i].y
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

int dist_room( struct room a, struct room b )
{
  int x = ( a.x - b.x ) * ( a.x - b.x );
  int y = ( a.y - b.y ) * ( a.y - b.y );
  return x + y;
}

void random_room( int i, int ind )
{
  rooms[i].x = get_rand( M_COLS ) - 1;
  rooms[i].y = get_rand( M_ROWS ) - 1;

  do
  {
    rooms[i].w = get_rand( 3 * ind );
  }
  while ( rooms[i].w < 2 * ind );

  do
  {
    rooms[i].h = get_rand( 2 * ind + ind / 2 );
  }
  while ( rooms[i].h < ind );
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

#define is_time(x) ( NR_ROOMS - x > NR_ROOMS / 2 - NR_ROOMS % 2 ? 2 : 3 )
void generate_rooms( void )
{
  for ( int i = 0; i < NR_ROOMS; i++ )
  {
    int k = 0;
  checks:

    if ( k == 1000 )
    {
      k = 0;
      i = 0;
      goto checks;
    }

    do
    {
      random_room( i, is_time( i ) );
    }
    while ( check_room( i ) );

    for ( int j = 0; j < i; j++ )
      if ( check_intersect( i, j ) )
      {
        k++;
        goto checks;
      }
  }
}

void sort_rooms( void )
{
  for ( int i = 0; i < NR_ROOMS - 1; i++ )
  {
    int k = i + 1;

    for ( int j = i + 2; j < NR_ROOMS; j++ )
      if ( dist_room( rooms[i], rooms[j] ) < dist_room( rooms[i], rooms[k] ) )
        k = j;

    if ( k != i + 1 )
    {
      struct room x = rooms[i];
      rooms[i] = rooms[k];
      rooms[k] = x;
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

void generate_paths( void )
{
  for ( int i = 0; i < NR_ROOMS - 1; i++ )
  {
    int x1 = rooms[i].x + get_rand( rooms[i].w / 2 );
    int x2 = rooms[i + 1].x + get_rand( rooms[i + 1].w / 2 );
    int y1 = rooms[i].y + get_rand( rooms[i].h / 2 );
    int y2 = rooms[i + 1].y + get_rand( rooms[i + 1].h / 2 );
    int startx = x1 < x2 ? x1 : x2;
    int endx   = x1 < x2 ? x2 : x1;
    int starty = x1 < x2 ? y1 : y2;
    int endy   = x1 < x2 ? y2 : y1;

    while ( startx != endx )
    {
      int maybe = get_rand( 10 );

      if ( maybe < 3 && starty < endy )
        starty++;
      else if ( maybe < 3 && starty > endy )
        starty--;

      map[starty][startx].id = Ofloor.id;
      startx++;
    }

    while ( starty != endy )
    {
      if ( starty > endy )
        starty--;
      else
        starty++;

      map[starty][endx].id = Ofloor.id;
    }
  }
}

void do_on(int i, int j, int which){
  if ( i == 0 || j == 0 || i == M_ROWS - 1 || j == M_COLS - 1 )
    return;

  if ( which ){
    if ( map[i][j].id == ID_FLOOR && map[i][j+1].id == ID_FLOOR ){
      map[i][j].id = ID_WALL;
    }
  }
  else {
    if ( map[i][j].id == ID_FLOOR && map[i+1][j].id == ID_FLOOR ){
      map[i][j].id = ID_WALL;
    }
  }
}

void choke_rooms(void)
{
  for ( int i = 0; i < NR_ROOMS; i++ )
  {
    for ( int j = rooms[i].x; j < rooms[i].x + rooms[i].w; j++ ){
      do_on(rooms[i].y - 1, j, 1);
      do_on(rooms[i].y + rooms[i].h, j, 1);
    }
    for ( int j = rooms[i].y; j < rooms[i].y + rooms[i].h; j++ ){
      do_on(j, rooms[i].x - 1, 0);
      do_on(j, rooms[i].x + rooms[i].w, 0);
    }
  }
}

void set_player( void )
{
  int which = get_rand( NR_ROOMS ) - 1;
  int x = rooms[which].x + get_rand( rooms[which].w ) - 1;
  int y = rooms[which].y + get_rand( rooms[which].h ) - 1;
  teleport_player( y, x );
}

void set_exit( void )
{
  int which = get_rand( NR_ROOMS ) - 1;
  int x = rooms[which].x + get_rand( rooms[which].w ) - 1;
  int y = rooms[which].y + get_rand( rooms[which].h ) - 1;
  map[y][x].id = ID_EXIT;
}

int next_level( void )
{
  level++;

  if ( level > LAST_LEVEL )
    return 1;

  generate_map();
  generate_rooms();
  sort_rooms();
  generate_rooms_in_map();
  generate_paths();
  choke_rooms();
  set_player();
  set_exit();
  return 0;
}

struct object *get_tile( int y, int x )
{
  return map[y] + x;
}

char get_level( void )
{
  return level;
}

struct room *get_rooms( void )
{
  return rooms;
}

struct room *find_inside( int x, int y )
{
  for ( int i = 0; i < NR_ROOMS; i++ )
  {
    if ( x >= rooms[i].x && x <= rooms[i].x + rooms[i].w - 1 &&
         y >= rooms[i].y && y <= rooms[i].y + rooms[i].h - 1
       )
      return rooms + i;
  }

  return 0;
}
