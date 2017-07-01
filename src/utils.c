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
  srandom( time ( NULL ) );
}

int get_rand( int x )
{
  return random() % x + 1;
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

void move_unit( int *x, int *y, char dir )
{
  switch ( dir )
  {
    case CENTER:
      break;

    case LEFT:
      ( *x )--;
      break;

    case RIGHT:
      ( *x )++;
      break;

    case UP:
      ( *y )--;
      break;

    case DOWN:
      ( *y )++;
      break;

    case LDOWN:
      ( *x )--;
      ( *y )++;
      break;

    case RDOWN:
      ( *y )++;
      ( *x )++;
      break;

    case LUP:
      ( *y )--;
      ( *x )--;
      break;

    case RUP:
      ( *x )++;
      ( *y )--;
      break;
  }
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
