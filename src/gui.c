#include <ncurses.h>

#include "symbol.h"
#include "main.h"

#define get_pair(a, b) ( COLOR_PAIR(a * 10 + b) )

int rows, cols;

int get_attribs( char *a )
{
  int to_return = 0;

  if ( a[AT_NORMAL] )
    to_return |= A_NORMAL;

  if ( a[AT_STANDOUT] )
    to_return |= A_STANDOUT;

  if ( a[AT_UNDERLINE] )
    to_return |= A_UNDERLINE;

  if ( a[AT_REVERSE] )
    to_return |= A_REVERSE;

  if ( a[AT_BLINK] )
    to_return |= A_BLINK;

  if ( a[AT_DIM] )
    to_return |= A_DIM;

  if ( a[AT_BOLD] )
    to_return |= A_BOLD;

  if ( a[AT_PROTECT] )
    to_return |= A_PROTECT;

  if ( a[AT_INVIS] )
    to_return |= A_INVIS;

  if ( a[AT_ALTCHARSET] )
    to_return |= A_ALTCHARSET;

  if ( a[AT_CHARTEXT] )
    to_return |= A_CHARTEXT;

  return to_return;
}

int get_color( int a )
{
  switch ( a )
  {
    case C_WHITE:
      return COLOR_WHITE;

    case C_RED:
      return COLOR_RED;

    case C_GREEN:
      return COLOR_GREEN;

    case C_BLUE:
      return COLOR_BLUE;

    case C_YELLOW:
      return COLOR_YELLOW;

    case C_CYAN:
      return COLOR_CYAN;

    case C_MAGENTA:
      return COLOR_MAGENTA;

    case C_BLACK:
      return COLOR_BLACK;

    default:
      return COLOR_WHITE;
  }
}

int init_gui( void )
{
  initscr();
  getmaxyx( stdscr, rows, cols );

  if ( rows < TERMINAL_HEIGHT || cols < TERMINAL_HEIGHT )
  {
    end_engine( "Current terminal size is too small. Minimum is 80x24.\n" );
    return 0;
  }

  ESCDELAY = 25;
  noecho();
  raw();
  keypad( stdscr, TRUE );
  curs_set( 0 );
  start_color();

  for ( int i = 0; i < NR_COLORS; i++ )
    for ( int j = 0; j < NR_COLORS; j++ )
      init_pair( i * 10 + j, get_color( i ), get_color( j ) );

  return 1;
}

void print_symbol( struct symbol k )
{
  if ( k.status == EMPTY_SYMBOL )
    return;

  attron( get_attribs( k.attribs ) | get_pair( k.fg, k.bg ) );
  mvprintw( k.y, k.x, "%s", k.identity );
  attroff( get_attribs( k.attribs ) | get_pair( k.fg, k.bg ) );
}

void update_gui( struct symbol *x )
{
  erase();
  struct symbol k = *x;

  while ( k.next )
  {
    print_symbol( k );
    k = *k.next;
  }

  print_symbol( k );
  refresh();
}

void end_gui( const char *text )
{
  endwin();
  printf( "%s", text );
}

char get_input( void )
{
  return getch();
}
