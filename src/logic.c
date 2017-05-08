#include "headers/symbol.h"
#include "headers/player.h"
#include "headers/standard_objects.h"
#include "headers/mapgen.h"
#include "headers/utils.h"

struct object *Omap;
struct object *Oplayer;
char loaded = 0;

void init_logic( void )
{
  Omap = get_tile( 0, 0 );
  Oplayer = get_player();
}

void slap_player( void )
{
  struct object *k = Omap;

  while ( k->next )
    k = k->next;

  k->next = Oplayer;
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

    loaded = 1;
    slap_player();
  }

  update_player( input );
  put_fov();
  return Omap;
}
