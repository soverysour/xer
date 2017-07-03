#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "symbol.h"
#include "mapgen.h"

FILE *logfile;

void init_log( void )
{
  logfile = fopen( "/home/sour/Repos/xer/bin/log.txt", "w" );
}

void logit (char *s){
  fprintf(logfile, "%s", s);
}

void end_log( void )
{
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
  return source > target ? 1 : -1;
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

char get_direction( int px, int py, int x, int y )
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
