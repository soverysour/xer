#include "headers/symbol.h"
#include "headers/mapgen.h"
#include "headers/standard_objects.h"
#include "headers/monsters.h"

#define DOWN 'j'
#define LEFT 'h'
#define	RIGHT 'l'
#define UP 'k'
#define LDOWN 'b'
#define RDOWN 'n'
#define LUP 'y'
#define RUP 'u'
#define CENTER '.'

#define IMPASSABLE 0
#define PASSABLE 1

char ef[] = {};

struct entity entity_player =
{
  .hp = 16,
  .damage = 5
};

struct object object_player =
{
  .id = ID_PLAYER,
  .x = 6,
  .y = 4,
  .visibility = V_SEEN,
  .effects = ef,
  .entity = &entity_player,
  .next = &Ohp
};

void update_player( char input )
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
}

struct object *get_player( void )
{
  return &object_player;
}

void teleport_player( int y, int x )
{
  object_player.x = x;
  object_player.y = y;
}
