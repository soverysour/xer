#include <stdlib.h>
#include <stdio.h>

#include "symbol.h"
#include "utils.h"
#include "player.h"
#include "mapgen.h"

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

char directions[ARRAY_WIDTH] = {};
char *get_directions( void )
{
  return directions;
}

int verify_directions( int px, int py )
{
  int x = px, y = py;
  int i = 0;

  while ( directions[i] )
  {
    move_unit( &x, &y, directions[i] );

    if ( get_tile( y, x )->id == ID_WALL )
    {
      if ( !directions[i + 1] )
        return 1;

      return 0;
    }

    i++;
  }

  return 1;
}

char btfov[ARRAY_WIDTH] = {};
char *get_pathline( void )
{
  return btfov;
}

void btwrite( int isDiag, int px, int py, int x, int y, int suffix, int step, int spot )
{
  int xs = absolute( px - x );
  int ys = absolute( py - y );
  int dirr;

  if ( xs > ys )
    dirr = get_direction( px, 1, x, 1 );
  else
    dirr = get_direction( 1, py, 1, y );

  if ( isDiag )
  {
    btfov[step] = DIAG;
    directions[spot] = get_direction( px, py, x, y );

    for ( int i = step + 1; i < spot + suffix + 1; i++ )
      directions[i] = dirr;

    return;
  }

  btfov[step] = STRAIGHT;
  directions[spot] = dirr;
}

int backtrack_fov( int diags, int suffix, int leftover, int px, int py, int x, int y, int step, int spot )
{
  if ( leftover )
    if ( !step || btfov[step - 1] != STRAIGHT )
    {
      btwrite( STRAIGHT, px, py, x, y, suffix, step, spot );

      if ( backtrack_fov( diags, suffix, leftover - 1, px, py, x, y, step + 1, spot + 1 ) )
        return 1;
    }

  if ( diags )
  {
    btwrite( DIAG, px, py, x, y, suffix, step, spot );

    if ( backtrack_fov( diags - 1, suffix, leftover, px, py, x, y, step + 1, spot + suffix + 1 ) )
      return 1;
  }

  if ( !leftover && !diags )
    return verify_directions( px, py );

  return 0;
}

int in_reach( int y, int x, int py, int px )
{
  int xs = absolute( px - x );
  int ys = absolute( py - y );
  int total = xs > ys ? xs : ys;
  int diags = xs > ys ? ys : xs;
  int straights = xs > ys ? xs - ys : ys - xs;
  int suffix = diags > 0 ? straights / diags : straights;
  int leftover = diags > 0 ? straights % diags : 0;

  for ( int i = 0; i < ARRAY_WIDTH; i++ )
    directions[i] = '\0';

  if ( !diags || !straights )
  {
    char dirr = get_direction( px, py, x, y );

    for ( int i = 0; i < total; i++ )
      directions[i] = dirr;

    return verify_directions( px, py );
  }

  return backtrack_fov( diags, suffix, leftover, px, py, x, y, 0, 0 );
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
      if ( i > -1 && j > -1 && i < M_ROWS && j < M_COLS && ( i != y || j != x ) )
        if ( in_reach( i, j, y, x ) )
          get_tile( i, j )->visibility = V_SEEN;
}
