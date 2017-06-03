#include "symbol.h"
#include "mapgen.h"
#include "utils.h"
#include "player.h"
#include "guiutils.h"

#define MONSTER_COUNT 8
#define MONSTER_FOV 2

struct object *head_monster;

char effects[NR_BUFFS] = {};

struct object *get_monster( int y, int x )
{
  if ( !head_monster )
    return 0;

  for ( struct object *current = head_monster; current; current = current->next )
    if ( current->x == x && current->y == y )
      return current;

  return 0;
}

void kill_monster( struct object *monster )
{
  give_entity( monster->entity );
  give_object( monster );
}

void destroy_monster( struct object *monster )
{
  if ( monster == head_monster )
  {
    head_monster = head_monster->next;
    kill_monster( monster );
    return;
  }

  struct object *p = head_monster, *k;

  while ( p->next != monster )
    p = p->next;

  k = p->next;
  p->next = p->next->next;
  kill_monster( k );
}

void damage_monster( struct object *monster, int damage )
{
  monster->entity->hp -= damage;

  if ( monster->entity->hp < 1 )
    destroy_monster( monster );
}

void kill_monsters( void )
{
  if ( !head_monster )
    return;

  struct object *current = head_monster, *previous = head_monster;

  while ( current->next )
  {
    give_entity( current->entity );
    current = current->next;
    give_object( previous );
    previous = current;
  }

  give_entity( current->entity );
  give_object( current );
}

void plonk_monster( struct object *m, int y, int x, int h, int w )
{
  int tx, ty;

  do
  {
    tx = x + get_rand( w ) - 1;
    ty = y + get_rand( h ) - 1;
  }
  while ( tx == get_player()->x && ty == get_player()->y );

  m->x = tx;
  m->y = ty;
  m->id = ID_MONSTER;
  m->visibility = V_UNSEEN;
  m->effects = effects;
  m->entity = get_entity();
  m->entity->hp = 11;
  m->entity->damage = 2;
}

void new_monsters( void )
{
  kill_monsters();
  head_monster = get_object();
  struct object *current_monster = head_monster;
  current_monster->entity = get_entity();
  struct room *x = get_rooms();

  for ( int i = 0; i < MONSTER_COUNT; i++ )
  {
    plonk_monster( current_monster, x[i].y, x[i].x, x[i].h, x[i].w );

    if ( i == MONSTER_COUNT - 1 )
      break;

    current_monster->next = get_object();
    current_monster = current_monster->next;
  }
}

void go_towards( struct object *monster )
{
  int x = 0, y = 0;

  if ( monster->x != get_player()->x )
    x = proc_unit( monster->x, get_player()->x );

  if ( monster->y != get_player()->y )
    y = proc_unit( monster->y, get_player()->y );

  if ( get_tile( monster->y + y, monster->x + x )->id == ID_WALL )
  {
    if ( get_tile( monster->y, monster->x + x )->id == ID_WALL )
      x = 0;
    else
      y = 0;
  }

  monster->x += x;
  monster->y += y;
}

void update_monsters( void )
{
  for ( struct object *monster = head_monster; monster; monster = monster->next )
  {
    if ( absolute( monster->y - get_player()->y ) <= FOV_RADIUS &&
         absolute( monster->x - get_player()->x ) <= FOV_RADIUS
       )
    {
      if ( in_reach( monster->y, monster->x, get_player()->y, get_player()->x ) )
        monster->visibility = V_SEEN;
      else
        monster->visibility = V_UNSEEN;
    }
    else
      monster->visibility = V_UNSEEN;

    if ( absolute( monster->y - get_player()->y ) <= MONSTER_FOV &&
         absolute( monster->x - get_player()->x ) <= MONSTER_FOV
       )
    {
      if (
          absolute( monster->y - get_player()->y ) <= 1 &&
          absolute( monster->x - get_player()->x ) <= 1
      )
      {
        damage_player( monster->entity->damage );
        continue;
      }

      if ( in_reach( monster->y, monster->x, get_player()->y, get_player()->x ) )
        go_towards( monster );
    }
  }
}

struct object *get_monsters( void )
{
  return head_monster;
}
