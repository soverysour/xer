#ifndef HEADER_SYMBOL
#define HEADER_SYMBOL

#define NR_ATTRIBS 11
#define NR_COLORS 8
#define NR_IDS 6
#define NR_BUFFS 1
#define NR_VISIBILITY 3
#define EMPTY_SYMBOL 0

#define TERMINAL_WIDTH 80
#define TERMINAL_HEIGHT 24

#define M_ROWS TERMINAL_HEIGHT - 6
#define M_COLS 80

#define POS_HP 18
#define POS_LEVEL 19
#define POS_WAIT 12

#define DOWN '2'
#define VI_DOWN 'j'
#define LEFT '4'
#define VI_LEFT 'h'
#define	RIGHT '6'
#define VI_RIGHT 'l'
#define UP '8'
#define VI_UP 'k'
#define LDOWN '1'
#define VI_LDOWN 'b'
#define RDOWN '3'
#define VI_RDOWN 'n'
#define LUP '7'
#define VI_LUP 'y'
#define RUP '9'
#define VI_RUP 'u'
#define CENTER '5'
#define VI_CENTER '.'

struct room
{
  int x, y;
  int w, h;
};

struct symbol
{
  char *identity;
  char *attribs;
  char fg, bg;
  int y, x;
  char status;

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
  ID_WIN,
  ID_DEAD,
  ID_PLAYER,
  ID_MONSTER,
  ID_WALL,
  ID_FLOOR,
  ID_DOOR,
  ID_EXIT,
  ID_HUD
};

enum buffs
{
  B_BUFFED
};

enum visibility
{
  V_UNSEEN,
  V_FOG,
  V_SEEN
};

struct entity
{
  int hp;
  int current_hp;
  int damage;
};

struct object
{
  int x, y;
  int id;
  char visibility;
  char *effects;

  struct entity *entity;
  struct object *next;
};
#endif
