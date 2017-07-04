#include "symbol.h"
#include "player.h"
#include "standard_objects.h"
#include "mapgen.h"
#include "monsters.h"
#include "guiutils.h"
#include "utils.h"

struct object *Omap;
struct object *Oplayer;
struct object *Ohud;
struct object *Oend_hud;

char loaded = 0;

enum
{
  STATE_WAIT,
  STATE_GAME
};

char current_state;

void init_logic( void )
{
  Omap = get_tile( 0, 0 );
  Oplayer = get_player();
  Ohud = &Ohp;
  Oend_hud = &Olevel;
  current_state = STATE_WAIT;
}

void go_next_level( void )
{
  loaded = 0;
}

void slap_together( void )
{
  get_tile( M_ROWS - 1, M_COLS - 1 )->next = Oplayer;
  Oplayer->next = Ohud;
  Oend_hud->next = get_monsters();
}

struct object *logic_update( char _input )
{ 
  char input = _input;

  alias_input(&input);

  if ( get_player()->entity->hp < 1 )
    return &Odead;

  if ( input == 'Q' )
    return &Oquit;

  if ( !loaded )
  {
    if ( next_level() )
      return &Owin;

    new_monsters();
    loaded = 1;
  }

  int status = update_player( input );

  if ( status > 0 )
  {
    current_state = STATE_WAIT;
    return &Owait;
  }

  if ( status < 0 )
  {
    slap_together();
    put_fov();

    if ( current_state == STATE_WAIT )
      update_monsters();

    current_state = STATE_GAME;
    return Omap;
  }

  current_state = STATE_GAME;
  slap_together();
  put_fov();
  update_monsters();
  return Omap;
}
