#include "headers/symbol.h"
#include "headers/player.h"
#include "headers/standard_objects.h"
#include "headers/mapgen.h"
#include "headers/monsters.h"
#include "headers/guiutils.h"

struct object *Omap;
struct object *Oplayer;
char loaded = 0;

void init_logic( void )
{
  Omap = get_tile( 0, 0 );
  Oplayer = get_player();
}

void put_monsters( void )
{
  Oplayer->next = get_monsters();
}

struct object *logic_update( char input )
{
  if ( input == 'Q' )
    return &Oquit;

  if ( input == 'R' )
    loaded = 0;

  if ( !loaded )
  {
    if ( next_level() )
      return &Oquit;

    new_monsters();
    loaded = 1;
  }

  update_player( input );
  put_monsters();
  put_fov();
  return Omap;
}
