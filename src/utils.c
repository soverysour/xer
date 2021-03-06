#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "symbol.h"
#include "mapgen.h"

void logit ( char *s )
{
  FILE *logfile = fopen( "/home/sour/Repos/xer/bin/log", "a" );
  fprintf( logfile, "%s\n", s );
  fclose( logfile );
}

int absolute( int x )
{
  if ( x < 0 )
    return -x;

  return x;
}

void init_seed( void )
{
  srand( time ( NULL ) );
}

int get_rand( int x )
{
  return rand() % x + 1;
}

struct object *get_object( void )
{
  return calloc( 1, sizeof( struct object ) );
}

void give_object( struct object *k )
{
  free( k );
}

struct entity *get_entity( void )
{
  return calloc( 1, sizeof( struct entity ) );
}

void give_entity( struct entity *k )
{
  free( k );
}

int proc_unit( int target, int source )
{
  return source > target ? -1 : ( source < target ? 1 : 0 );
}

int move_unit( int *x, int *y, char dir )
{
  switch ( dir )
  {
    case CENTER:
      return 1;

    case LEFT:
      ( *x )--;
      return 1;

    case RIGHT:
      ( *x )++;
      return 1;

    case UP:
      ( *y )--;
      return 1;

    case DOWN:
      ( *y )++;
      return 1;

    case LDOWN:
      ( *x )--;
      ( *y )++;
      return 1;

    case RDOWN:
      ( *y )++;
      ( *x )++;
      return 1;

    case LUP:
      ( *y )--;
      ( *x )--;
      return 1;

    case RUP:
      ( *x )++;
      ( *y )--;
      return 1;
  }

  return 0;
}

char get_direction( int y, int x, int py, int px )
{
  if ( x == px && y == py )
    return CENTER;

  if ( x == px )
  {
    if ( y < py )
      return UP;

    return DOWN;
  }

  if ( y == py )
  {
    if ( x > px )
      return RIGHT;

    return LEFT;
  }

  if ( x < px && y < py )
    return LUP;

  if ( x > px && y < py )
    return RUP;

  if ( x > px && y > py )
    return RDOWN;

  if ( x < px && y > py )
    return LDOWN;

  return CENTER;
}

char alias_input( char *input )
{
  switch ( *input )
  {
    case VI_CENTER:
      *input = CENTER;
      break;

    case VI_LEFT:
      *input = LEFT;
      break;

    case VI_RIGHT:
      *input = RIGHT;
      break;

    case VI_UP:
      *input = UP;
      break;

    case VI_DOWN:
      *input = DOWN;
      break;

    case VI_LUP:
      *input = LUP;
      break;

    case VI_RUP:
      *input = RUP;
      break;

    case VI_LDOWN:
      *input = LDOWN;
      break;

    case VI_RDOWN:
      *input = RDOWN;
      break;
  }

  return CENTER;
}
