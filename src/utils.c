#include <time.h>
#include <stdlib.h>

#include "headers/symbol.h"
#include "headers/mapgen.h"

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
