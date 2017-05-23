#include "headers/symbol.h"
#include "headers/mapgen.h"
#include "headers/utils.h"

#define MONSTER_COUNT 8

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

int damage_monster( struct object *monster, int damage )
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

void plonk_monster( struct object *m, int y, int x )
{
  m->x = x;
  m->y = y;
  m->id = ID_MONSTER;
  m->visibility = V_SEEN;
  m->effects = effects;
  m->entity = get_entity();
  m->entity->hp = 11;
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
    plonk_monster( current_monster, x[i].y, x[i].x );

    if ( i == MONSTER_COUNT - 1 )
      break;

    current_monster->next = get_object();
    current_monster = current_monster->next;
  }
}

struct object *get_monsters( void )
{
  return head_monster;
}
