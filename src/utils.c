#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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

struct entity *get_entity(void){
  return calloc( 1, sizeof( struct entity ) );
}

void give_entity(struct entity *k){
  free(k);
}

void silent_apply( struct object *a, struct symbol *b )
{
  if ( a->id == ID_HUD ){
    if ( a->y == 18 ){
      char *hold = calloc(10, sizeof(char));
      sprintf(hold, "HP: %i", get_player()->entity->hp);
      b->identity = hold;
      b->x = 0;
      b->y = 18;
      b->status = !EMPTY_SYMBOL;

      b->attribs = calloc(NR_ATTRIBS, sizeof(char));

      b->attribs[AT_BOLD] = 1;

      b->fg = C_WHITE;
      b->bg = C_BLACK;
    }

    return;
  }

  if ( a->visibility == V_UNSEEN )
  {
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

  switch ( a->visibility )
  {
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

int proc_unit( int target, int source )
{
  return ( source - target ) / absolute( source - target );
}

int backtrack_fov( int diags, int straights, int targety, int targetx, int cury, int curx )
{
  if ( curx == targetx && cury == targety )
    return 1;

  if ( get_tile( cury, curx )->id == ID_WALL )
    return 0;

  if ( diags == 0 && straights == 0 )
    return 0;

  if ( cury < 0 || cury > M_ROWS - 1 )
    return 0;

  if ( curx < 0 || curx > M_COLS - 1 )
    return 0;

  if ( cury != targety && straights > 0 )
    if ( backtrack_fov( diags,
                        straights - 1,
                        targety,
                        targetx,
                        cury - proc_unit( targety, cury ),
                        curx )
       )
      return 1;

  if ( curx != targetx && straights > 0 )
    if ( backtrack_fov( diags,
                        straights - 1,
                        targety,
                        targetx,
                        cury,
                        curx - proc_unit( targetx, curx ) )
       )
      return 1;

  if ( cury != targety && curx != targetx && diags > 0 )
    return backtrack_fov( diags - 1,
                          straights,
                          targety,
                          targetx,
                          cury - proc_unit( targety, cury ),
                          curx - proc_unit( targetx, curx )
                        );

  return 0;
}

void fov_line( int y, int x, int py, int px )
{
  int xs = absolute( px - x );
  int ys = absolute( py - y );
  int diags = xs > ys ? ys : xs;
  int straights = xs > ys ? xs - ys : ys - xs;
  
  if ( backtrack_fov( diags, straights, y, x, py, px ) )
    get_tile( y, x )->visibility = V_SEEN;
}

void put_fov( void )
{
  for ( int i = 0; i < M_ROWS; i++ )
    for ( int j = 0; j < M_COLS; j++ )
      if ( get_tile( i, j )->visibility == V_SEEN )
        get_tile( i, j )->visibility = V_FOG;

  int x = get_player()->x, y = get_player()->y;

  for ( int i = y - FOV_RADIUS; i < y + FOV_RADIUS + 1; i++ )
    for ( int j = x - FOV_RADIUS; j < x + FOV_RADIUS + 1; j++ )
      fov_line( i, j, y, x );
}
