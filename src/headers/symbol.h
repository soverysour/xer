#ifndef HEADER_SYMBOL
#define HEADER_SYMBOL

#define NR_ATTRIBS 11
#define NR_COLORS 8
#define NR_IDS 2
#define NR_BUFFS 2

struct symbol {
	char *identity;
	char *attribs;
	char fg, bg;
	int y, x;

	struct symbol *next;
};

enum attributes {
	normal,
	stando,
	underline,
	reverse,
	blink,
	dim,
	bold,
	protect,
	invis,
	altcharset,
	chartext
};

enum colors {
	black,
	red,
	green,
	yellow,
	blue,
	magenta,
	cyan,
	white
};

enum ids {
	player,
	monster
};

enum buffs {
	buffed,
	poisoned
};

struct object {
	int x, y;
	int id;
	int hp;
	char *effects;

	struct object *next;
};
#endif
