#include "headers/symbol.h"
#include "headers/player.h"
#include "headers/standard_objects.h"
#include "headers/mapgen.h"

struct object *Omap;
struct object *Oplayer;
char loaded = 0;

void init_logic( void )
{
  Omap = get_map();
  Oplayer = get_player();
}

struct object *logic_update( char input )
{
  if ( input == 'Q' )
    return &Oquit;

  if ( input == 'R' )
    loaded = 0;

  if ( !loaded )
  {
    load_map();
    loaded = 1;
    struct object *k = Omap;

    while ( k->next )
      k = k->next;

    k->next = Oplayer;
  }

  update_player( input );
  return Omap;
}
