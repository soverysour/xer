#include "headers/logic.h"
#include "headers/symbol.h"

#define DOWN '2'
#define LEFT '4'
#define	RIGHT '6'
#define UP '8'
#define LDOWN '1'
#define RDOWN '3'
#define LUP '7'
#define RUP '9'
#define CENTER '5'

int x = 35, y = 15;

void update_player(char input){
	switch ( input ){
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
	}
	update_Oplayer(x, y);
}
