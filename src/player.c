#include "symbol.h"
#include "mapgen.h"
#include "standard_objects.h"
#include "monsters.h"
#include "logic.h"
#include "main.h"
#include "utils.h"

#define HP 1000
#define DAMAGE 100

#define START_POWER 'W'
#define END_POWER 'w'
#define GO_NEXT '>'

char ef_player[] = {};

struct entity entity_player =
{
  .hp = HP,
  .current_hp = HP,
  .damage = DAMAGE
};

struct object object_player =
{
  .id = ID_PLAYER,
  .x = 6,
  .y = 4,
  .visibility = V_SEEN,
  .effects = ef_player,
  .entity = &entity_player,
  .next = &Ohp
};

int update_player( char input )
{
  int x = 0, y = 0;
  int has_moved = move_unit( &x, &y, input );
  int is_input = has_moved;

  if ( !has_moved )
    switch ( input )
    {
      case START_POWER:
        is_input = 1;
        object_player.effects[B_BUFFED] = 1;
        break;

      case END_POWER:
        is_input = 1;
        object_player.effects[B_BUFFED] = 0;
        break;

      case GO_NEXT:
        if ( get_tile( object_player.y, object_player.x )->id == ID_EXIT )
        {
          go_next_level();
          return 1;
        }

        break;
    }

  if ( !is_input )
    return -1;

  struct object *monster = get_monster( object_player.y + y, object_player.x + x );

  if ( monster )
  {
    if ( object_player.effects[B_BUFFED] )
      damage_monster( monster, object_player.entity->damage * 2 );
    else
      damage_monster( monster, object_player.entity->damage );
  }
  else if ( get_tile( object_player.y + y, object_player.x + x )->id == ID_WALL )
    return -1;
  else
  {
    object_player.x += x;
    object_player.y += y;
  }

  return 0;
}

struct object *get_player( void )
{
  return &object_player;
}

void damage_player( int damage )
{
  object_player.entity->hp -= damage;
}

void teleport_player( int y, int x )
{
  object_player.x = x;
  object_player.y = y;
}
