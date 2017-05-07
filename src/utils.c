#include <time.h>
#include <stdlib.h>
#include <string.h>

#include "headers/symbol.h"
#include "headers/mapgen.h"
#include "headers/player.h"

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

char *get_char_array( int k )
{
  return calloc( k, sizeof( char ) );
}

void give_char_array( char *k )
{
  free( k );
}

struct object *get_object( void )
{
  return calloc( 1, sizeof( struct object ) );
}

void give_object( struct object *k )
{
  free( k );
}

void silent_apply( struct object *a, struct symbol *b )
{
  if ( a->visibility == V_UNSEEN ){
    b->status = EMPTY_SYMBOL;
    return;
  }

  b->status = !EMPTY_SYMBOL;
  switch ( a->id )
  {
    case ID_PLAYER:
      b->identity = calloc( 2, sizeof( char ) );
      b->identity[0] = '@';
      b->identity[1] = '\0';
      break;

    case ID_MONSTER:
      b->identity = calloc( 2, sizeof( char ) );
      b->identity[0] = 'M';
      b->identity[1] = '\0';
      break;

    case ID_WALL:
      b->identity = calloc( 2, sizeof( char ) );
      b->identity[0] = '#';
      b->identity[1] = '\0';
      break;

    case ID_FLOOR:
      b->identity = calloc( 2, sizeof( char ) );
      b->identity[0] = '.';
      b->identity[1] = '\0';
      break;
  }

  b->attribs = calloc( NR_ATTRIBS, sizeof( char ) );

  if ( a->effects[B_BUFFED] )
    b->fg = C_RED;
  else
    b->fg = C_WHITE;
  b->bg = C_BLACK;

  switch ( a->visibility ){
    case V_FOG:
      b->attribs[AT_DIM] = 1;
      break;
    case V_SEEN:
      b->attribs[AT_BOLD] = 1;
      break;
  }

  b->y = a->y;
  b->x = a->x;
}

struct symbol *do_apply( struct object *k )
{
  struct object *a = k;
  struct symbol *b = calloc( 1, sizeof( struct symbol ) ), *prev;
  struct symbol *head = b;
  silent_apply( a, b );

  while ( a->next )
  {
    a = a->next;
    prev = b;
    b->next = calloc( 1, sizeof( struct symbol ) );
    b = b->next;
    prev->next = b;
    silent_apply( a, b );
  }

  return head;
}

void clean_symbol( struct symbol *a )
{
  struct symbol *forward = a->next, *now = a;

  while ( now->next )
  {
    free( now->identity );
    free( now->attribs );
    free( now );
    now = forward;
    forward = forward->next;
  }

  free( now->identity );
  free( now->attribs );
  free( now );
}

void put_fov(void){
  for ( int i = 0; i < M_ROWS; i++ )
    for ( int j = 0; j < M_COLS; j++ )
      if ( get_tile(i, j)->visibility == V_SEEN )
        get_tile(i, j)->visibility = V_FOG;

  int x = get_player()->x, y = get_player()->y;

  for ( int i = x - 1; i < x + 2; i++ )
    for ( int j = y - 1; j < y + 2; j++ )
      get_tile(j, i)->visibility = V_SEEN;
}
