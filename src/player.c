#include "symbol.h"
#include "mapgen.h"
#include "standard_objects.h"
#include "monsters.h"
#include "logic.h"
#include "main.h"

#define HP 16

char ef_player[] = {};

struct entity entity_player =
{
  .hp = HP,
  .current_hp = HP,
  .damage = 5
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

  switch ( input )
  {
    case CENTER:
      break;

    case LEFT:
      x--;
      break;

    case RIGHT:
      x++;
      break;

    case UP:
      y--;
      break;

    case DOWN:
      y++;
      break;

    case LDOWN:
      x--;
      y++;
      break;

    case RDOWN:
      y++;
      x++;
      break;

    case LUP:
      y--;
      x--;
      break;

    case RUP:
      x++;
      y--;
      break;

    case 'W':
      object_player.effects[B_BUFFED] = 1;
      break;

    case 'w':
      object_player.effects[B_BUFFED] = 0;
      break;

    case '>':
      if ( get_tile( object_player.y, object_player.x )->id == ID_EXIT )
      {
        go_next_level();
        return 1;
      }

      break;
  }

  struct object *monster = get_monster( object_player.y + y, object_player.x + x );

  if ( monster )
  {
    if ( object_player.effects[B_BUFFED] )
      damage_monster( monster, object_player.entity->damage * 2 );
    else
      damage_monster( monster, object_player.entity->damage );
  }
  else  if ( get_tile( object_player.y + y, object_player.x + x )->id != ID_WALL )
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
