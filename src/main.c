#include "headers/symbol.h"
#include "headers/gui.h"
#include "headers/utils.h"
#include "headers/guiutils.h"
#include "headers/logic.h"
#include "headers/standard_objects.h"

char running = 1;

void init_engine( void )
{
  running = init_gui();

  if ( !running )
    return;

  init_seed();
  init_logic();
  struct symbol *b = do_apply( &Owait );
  update_gui( b );
  clean_symbol( b );
}

void end_engine( char *text )
{
  if ( running )
  {
    end_gui( text );
    running = 0;
  }
}

void update_engine( void )
{
  char input = get_input();
  struct object *a = logic_update( input );

  if ( a->id == ID_QUIT )
    end_engine( "See you soon...\n" );
  else
  {
    struct symbol *b = do_apply( a );
    update_gui( b );
    clean_symbol( b );
  }
}

int main( void )
{
  init_engine();

  while ( running )
    update_engine();

  return 0;
}
