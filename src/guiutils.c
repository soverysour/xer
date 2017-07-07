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

    case ID_DOOR:
      b->identity = calloc( 2, sizeof( char ) );
      b->identity[0] = '+';
      b->identity[1] = '\0';
      break;

    case ID_EXIT:
      b->identity = calloc( 2, sizeof( char ) );
      b->identity[0] = '>';
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

  if ( a->id == ID_MONSTER && a->entity->monster_type == MONSTER_STALK )
  {
    b->attribs[AT_DIM] = 1;
    b->attribs[AT_BOLD] = 1;
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

  while ( forward )
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

struct node_path
{
  char path_so_far[MAX_PATH];
  int size;
  int y, x;

  struct node_path *next;
};

struct node_path *to_check, *to_grab;
char visit_map[M_ROWS][M_COLS] = {};
char saved_path[MAX_PATH] = {};

struct node_path *create_node( void )
{
  return calloc( 1, sizeof( struct node_path ) );
}

void destroy_node( struct node_path *n )
{
  free( n );
}

void add_direction( struct node_path *which, char dir )
{
  which->path_so_far[which->size] = dir;
  which->size++;
}

void pop_check( void )
{
  struct node_path *n = to_check;
  to_check = to_check->next;
  destroy_node( n );
}

void kill_list( struct node_path *l )
{
  if ( !l )
    return;

  if ( !l->next )
  {
    destroy_node( l );
    return;
  }

  struct node_path *forward = l->next, *now = l;

  while ( forward )
  {
    destroy_node( now );
    now = forward;
    forward = forward->next;
  }

  destroy_node( now );
}

void harvest( struct node_path *n )
{
  visit_map[n->y][n->x] = 1;

  for ( int i = -1; i < 2; i++ )
    for ( int j = -1; j < 2; j++ )
      if ( i || j )
      {
        if ( !visit_map[n->y + i][n->x + j] && get_tile( n->y + i, n->x + j )->id != ID_WALL &&
             !get_monster( n->y + i, n->x + j, 0 )
           )
        {
          struct node_path *new_node = create_node();
          new_node->y = n->y + i;
          new_node->x = n->x + j;
          new_node->size = n->size + 1;

          for ( int k = 0; k < MAX_PATH && n->path_so_far[k]; k++ )
            new_node->path_so_far[k] = n->path_so_far[k];

          new_node->path_so_far[n->size] = get_direction( n->y + i, n->x + j, n->y, n->x );
          new_node->next = to_grab;
          to_grab = new_node;
        }
      }
}

void set_solution( struct node_path *s )
{
  for ( int i = 0; i < s->size; i++ )
    saved_path[i] = s->path_so_far[i];

  saved_path[s->size] = CENTER;
}

int calculate_path( int y, int x, int py, int px, int _max_dist )
{
  saved_path[0] = CENTER;

  if ( y == py && x == px )
    return 1;

  int max_dist = _max_dist < 1 ? MAX_PATH : _max_dist;

  if ( absolute(py - y) < max_dist || absolute(px - x) < max_dist )
    return 0;

  for ( int i = 0; i < M_ROWS; i++ )
    for ( int j = 0; j < M_COLS; j++ )
      visit_map[i][j] = 0;

  to_grab = create_node();
  to_grab->x = px;
  to_grab->y = py;
  to_grab->size = 0;

  while ( max_dist )
  {
    to_check = to_grab;
    to_grab = 0;

    for ( struct node_path *n = to_check; n; n = n->next )
    {
      if ( n->x == x && n->y == y )
      {
        set_solution( n );
        kill_list( to_check );
        kill_list( to_grab );
        return 1;
      }

      harvest( n );
    }

    kill_list( to_check );
    max_dist--;
  }

  kill_list( to_check );
  kill_list( to_grab );
  return 0;
}

char *get_directions( void )
{
  return saved_path;
}

int in_fov( int y, int x, int py, int px )
{
  if ( absolute( px - x ) <= 1 && absolute( py - y ) <= 1 )
    return 1;

  struct room *yes = find_inside( py, px ), *no = find_inside( y, x );

  if ( yes && yes == no )
    return 1;

  return 0;
}

void put_fov( void )
{
  for ( int i = 0; i < M_ROWS; i++ )
    for ( int j = 0; j < M_COLS; j++ )
      if ( get_tile( i, j )->visibility == V_SEEN )
        get_tile( i, j )->visibility = V_FOG;

  int x = get_player()->x, y = get_player()->y;
  struct room *yes = find_inside( y, x );

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
