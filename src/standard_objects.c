#include "headers/symbol.h"

struct object Oquit = {
	.id = ID_QUIT
};

char Ewallfloor[] = {1, 0};
struct object Owall = {
	.id = ID_WALL,
	.effects = Ewallfloor
};

struct object Ofloor = {
	.id = ID_FLOOR,
	.effects = Ewallfloor
};
