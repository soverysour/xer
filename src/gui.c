#include <ncurses.h>

#include "headers/symbol.h"
#include "headers/main.h"
#include "headers/utils.h"

#define get_pair(a, b) ( COLOR_PAIR(a * 10 + b) )

int rows, cols;

int get_attribs(char *a){
	int to_return = 0;

	if ( a[normal] ) to_return |= A_NORMAL;
	if ( a[stando] ) to_return |= A_STANDOUT;
	if ( a[underline] ) to_return |= A_UNDERLINE;
	if ( a[reverse] ) to_return |= A_REVERSE;
	if ( a[blink] ) to_return |= A_BLINK;
	if ( a[dim] ) to_return |= A_DIM;
	if ( a[bold] ) to_return |= A_BOLD;
	if ( a[protect] ) to_return |= A_PROTECT;
	if ( a[invis] ) to_return |= A_INVIS;
	if ( a[altcharset] ) to_return |= A_ALTCHARSET;
	if ( a[chartext] ) to_return |= A_CHARTEXT;

	return to_return;
}

int get_color(int a){
	switch ( a ){
		case white:
			return COLOR_WHITE;
		case red:
			return COLOR_RED;
		case green:
			return COLOR_GREEN;
		case blue:
			return COLOR_BLUE;
		case yellow:
			return COLOR_YELLOW;
		case cyan:
			return COLOR_CYAN;
		case magenta:
			return COLOR_MAGENTA;
		case black:
			return COLOR_BLACK;
	}

	return COLOR_BLACK;
}

int init_gui(void){
	initscr();
	getmaxyx(stdscr, rows, cols);
	
	if ( rows < 31 || cols < 61 ){
		end_engine("Current terminal size is too small. Minimum is 30x60.\n");
		return 0;
	}

	ESCDELAY = 25;
	noecho();
	raw();
	keypad(stdscr, TRUE);
	curs_set(0);
	start_color();

	for ( int i = 0; i < NR_COLORS; i++ )
		for ( int j = 0; j < NR_COLORS; j++ )
		  init_pair(i * 10 + j, get_color(i), get_color(j));

	return 1;
}

void print_symbol(struct symbol k){
	attron(get_attribs(k.attribs) | get_pair(k.fg, k.bg));
	mvprintw(k.y, k.x, "%s", k.identity);
	attroff(get_attribs(k.attribs) | get_pair(k.fg, k.bg));
}

void update_gui(struct symbol *x){
	erase();
	struct symbol k = *x;

	while ( k.next ) {
		print_symbol(k);
		k = *k.next;
	}

	print_symbol(k);
	refresh();
}

void end_gui(char *text){
	endwin();
	printf("%s", text);
}

char get_input(void){
	return getch();
}
