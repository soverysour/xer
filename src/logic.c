#include "headers/symbol.h"
#include "headers/player.h"
#include "headers/standard_objects.h"
#include "headers/mapgen.h"
#include "headers/monsters.h"
#include "headers/guiutils.h"

struct object *Omap;
struct object *Oplayer;
struct object *Ohud;

char loaded = 0;

void init_logic( void )
{
  Omap = get_tile( 0, 0 );
  Oplayer = get_player();
  Ohud = &Ohp;
}

void go_next_level( void )
{
  loaded = 0;
}

void slap_together( void )
{
  get_tile( M_ROWS - 1, M_COLS - 1 )->next = Oplayer;
  Oplayer->next = Ohud;
  struct object *k = Ohud;

  while ( k->next && k->next->id == ID_HUD )
    k = k->next;

  k->next = get_monsters();
}

struct object *logic_update( char input )
{
  if ( input == 'Q' )
    return &Oquit;

  if ( !loaded )
  {
    if ( next_level() )
      return &Oquit;

    new_monsters();
    loaded = 1;
  }

  if ( update_player( input ) )
    return &Owait;

  slap_together();
  put_fov();
  update_monsters();
  return Omap;
}
