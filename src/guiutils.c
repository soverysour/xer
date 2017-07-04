#include <stdlib.h>
#include <stdio.h>

#include "symbol.h"
#include "utils.h"
#include "player.h"
#include "mapgen.h"
#include "monsters.h"

#define hp_x 0
#define hp_y 18
#define wait_x 20
#define wait_y 12
#define level_x 0
#define level_y 19
#define ARRAY_WIDTH TERMINAL_WIDTH + 1
#define DIAG 1
#define STRAIGHT 0

void silent_apply( struct object *a, struct symbol *b )
{
  if ( a->id == ID_HUD )
  {
    if ( a->y == POS_HP )
    {
      char *hold = calloc( 15, sizeof( char ) );
      sprintf( hold, "HP: %i(%i)", get_player()->entity->hp, get_player()->entity->current_hp );
      b->identity = hold;
      b->x = hp_x;
      b->y = hp_y;
      b->status = !EMPTY_SYMBOL;
      b->attribs = calloc( NR_ATTRIBS, sizeof( char ) );
      b->attribs[AT_BOLD] = 1;
      b->fg = C_WHITE;
      b->bg = C_BLACK;
    }

    if ( a->y == POS_WAIT )
    {
      char *hold = calloc( 95, sizeof( char ) );
      sprintf( hold, "Press any key to continue..." );
      b->identity = hold;
      b->x = wait_x;
      b->y = wait_y;
      b->status = !EMPTY_SYMBOL;
      b->attribs = calloc( NR_ATTRIBS, sizeof( char ) );
      b->attribs[AT_BOLD] = 1;
      b->fg = C_WHITE;
      b->bg = C_BLACK;
    }

    if ( a->y == POS_LEVEL )
    {
      char *hold = calloc( 15, sizeof( char ) );
      sprintf( hold, "Level: %i", get_level() );
      b->identity = hold;
      b->x = level_x;
      b->y = level_y;
      b->status = !EMPTY_SYMBOL;
      b->attribs = calloc( NR_ATTRIBS, sizeof( char ) );
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

    case ID_EXIT:
      b->identity = calloc( 2, sizeof( char ) );
      b->identity[0] = '>';
      b->identity[1] = '\0';
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

int in_fov( int y, int x )
{
  int px = get_player()->x, py = get_player()->y;

  if ( absolute( px - x ) <= 1 && absolute( py - y ) <= 1 )
    return 1;

  struct room *yes = find_inside( px, py ), *no = find_inside(x, y);

  if ( yes == no )
    return 1;

  return 0;
}

char steps[ARRAY_WIDTH] = {};
char *get_directions(void){
  return steps;
}

int in_path( int y, int x, int py, int px )
{
  for ( int i = 0; i < ARRAY_WIDTH; i++ )
    steps[i] = CENTER;

  return 1;//backtrack_fov( diags, straights, y, x, py, px, 0 );
}

void put_fov( void )
{
  for ( int i = 0; i < M_ROWS; i++ )
    for ( int j = 0; j < M_COLS; j++ )
      if ( get_tile( i, j )->visibility == V_SEEN )
        get_tile( i, j )->visibility = V_FOG;

  int x = get_player()->x, y = get_player()->y;
  struct room *yes = find_inside( x, y );

  if ( yes )
  {
    for ( int i = yes->x - 1; i <= yes->x + yes->w; i++ )
      for ( int j = yes->y - 1; j <= yes->y + yes->h; j++ )
        get_tile( j, i )->visibility = V_SEEN;
  }

  for ( int i = x - 1; i <= x + 1; i++ )
    for ( int j = y - 1; j <= y + 1; j++ )
      get_tile( j, i )->visibility = V_SEEN;
}
