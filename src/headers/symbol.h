#ifndef HEADER_SYMBOL
#define HEADER_SYMBOL
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
#endif
