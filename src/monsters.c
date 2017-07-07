#include "symbol.h"
#include "mapgen.h"
#include "utils.h"
#include "player.h"
#include "guiutils.h"
#include "utils.h"

#define MONSTER_RANDOM_INTERVAL 2
#define MONSTER_ROOMIE_INTERVAL 3
#define MONSTER_STALK_INTERVAL 0

#define MONSTER_STALK_RADIUS 90

#define MONSTER_COUNT 4

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

  struct object *p = head_monster;

  while ( p->next != monster )
    p = p->next;

  p->next = p->next->next;
  kill_monster( monster );
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

void plonk_monster( struct object *m, int y, int x, int h, int w, int type )
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
  m->entity->monster_type = type;

  if ( type == MONSTER_STALK )
    m->entity->radius = MONSTER_STALK_RADIUS;
}

#define is_time(x) (x >= MONSTER_COUNT - MONSTER_COUNT / 2 - MONSTER_COUNT % 2 ? MONSTER_RANDOM : MONSTER_ROOMIE )
void new_monsters( void )
{
  kill_monsters();
  head_monster = get_object();
  struct object *current_monster = head_monster;
  struct room *x = get_rooms();

  for ( int i = 0; i < MONSTER_COUNT; i++ )
  {
    plonk_monster( current_monster, x[i].y, x[i].x, x[i].h, x[i].w, is_time( i ) );

    if ( i == MONSTER_COUNT - 1 )
      break;

    current_monster->next = get_object();
    current_monster = current_monster->next;
  }
}

void update_monsters( void )
{
  int px = get_player()->x, py = get_player()->y;

  for ( struct object *monster = head_monster; monster; monster = monster->next )
  {
    if ( absolute( px - monster->x ) <= 1 && absolute( py - monster->y ) <= 1 )
      damage_player( monster->entity->damage );
    else if ( monster->entity->monster_type == MONSTER_RANDOM )
    {
      if ( !monster->entity->update_interval )
      {
        char dirs[8] = {};
        int pos = 0;

        for ( int i = -1; i < 2; i++ )
          for ( int j = -1; j < 2; j++ )
            if ( ( i || j ) && get_tile( monster->y + i, monster->x + j )->id != ID_WALL &&
                 !get_monster( monster->y + i, monster->x + j )
               )
            {
              dirs[pos] = get_direction( monster->y + i, monster->x + j, monster->y, monster->x );
              pos++;
            }

        int which = get_rand( pos );
        move_unit( &monster->x, &monster->y, dirs[which - 1] );
        monster->entity->update_interval = MONSTER_RANDOM_INTERVAL;
      }
      else
        monster->entity->update_interval--;
    }
    else if ( monster->entity->monster_type == MONSTER_ROOMIE )
    {
      struct room *x = find_inside( monster->y, monster->x ), *y = find_inside( py, px );

      if ( x == y && x )
      {
        monster->y += proc_unit( py, monster->y );
        monster->x += proc_unit( px, monster->x );
        monster->entity->update_interval = MONSTER_ROOMIE_INTERVAL;
      }
      else if ( !monster->entity->update_interval )
      {
        char dirs[8] = {};
        int pos = 0;

        for ( int i = -1; i < 2; i++ )
          for ( int j = -1; j < 2; j++ )
            if ( ( i || j ) && get_tile( monster->y + i, monster->x + j )->id != ID_WALL &&
                 find_inside( monster->y, monster->x ) == find_inside( monster->y + i, monster->x + j ) &&
                 !get_monster( monster->y + i, monster->x + j )
               )
            {
              dirs[pos] = get_direction( monster->y + i, monster->x + j, monster->y, monster->x );
              pos++;
            }

        int which = get_rand( pos );
        move_unit( &monster->x, &monster->y, dirs[which - 1] );
        monster->entity->update_interval = MONSTER_ROOMIE_INTERVAL;
      }
      else
        monster->entity->update_interval--;
    }
    else if ( monster->entity->monster_type == MONSTER_STALK )
    {
      if ( calculate_path( py, px, monster->y, monster->x, monster->entity->radius ) )
        move_unit( &monster->x, &monster->y, get_directions()[0] );

      monster->entity->update_interval = MONSTER_STALK_INTERVAL;
    }

    if ( monster->entity->monster_type == MONSTER_STALK )
      monster->visibility = V_SEEN;
    else
    {
      if ( in_fov( monster->y, monster->x, py, px ) )
        monster->visibility = V_SEEN;
      else
        monster->visibility = V_UNSEEN;
    }
  }
}

struct object *get_monsters( void )
{
  return head_monster;
}
