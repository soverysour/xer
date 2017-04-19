#ifndef HEADER_SYMBOL
#define HEADER_SYMBOL

#define NR_ATTRIBS 11
#define NR_COLORS 8
#define NR_IDS 5
#define NR_BUFFS 3

#define M_ROWS 28
#define M_COLS 80

struct symbol
{
  char *identity;
  char *attribs;
  char fg, bg;
  int y, x;

  struct symbol *next;
};

enum attributes
{
  AT_NORMAL,
  AT_STANDOUT,
  AT_UNDERLINE,
  AT_REVERSE,
  AT_BLINK,
  AT_DIM,
  AT_BOLD,
  AT_PROTECT,
  AT_INVIS,
  AT_ALTCHARSET,
  AT_CHARTEXT
};

enum colors
{
  C_BLACK,
  C_RED,
  C_GREEN,
  C_YELLOW,
  C_BLUE,
  C_MAGENTA,
  C_CYAN,
  C_WHITE
};

enum ids
{
  ID_QUIT,
  ID_PLAYER,
  ID_MONSTER,
  ID_WALL,
  ID_FLOOR
};

enum buffs
{
  B_BUFFED,
  B_POISONED,
  B_FORCE
};

struct object
{
  int x, y;
  int id;
  int hp;
  char *effects;

  struct object *next;
};
#endif
